# ##############################################################################
# Chess Project benchmarking script
# ##############################################################################

import argparse
import json
import subprocess
import sys
from pathlib import Path

from .benchmark_types import Benchmark, BenchmarkResult
from .environment import Environment, get_build_folder, get_repo_base_path
from .git import run_git
from .perft_benchmark import parse_perft_benchmark

BENCHMARKS = [
    Benchmark(
        id="perft-10",
        name="Perft 10",
        description="Perft over a set of positions with 10 iterations per position",
        command=["./tools/benchmark/benchmark"],
        args=[
            "perft",
            "--iterations",
            "10",
            "$REPO/tools/benchmark/data/perft-bench.epd",
        ],
        parser=parse_perft_benchmark,
    ),
]


def list_benchmarks():
    print("id         | Name       | Description")
    print("-----------+------------+-------------------------------------------")
    for benchmark in BENCHMARKS:
        print(f"{benchmark.id:<10} | {benchmark.name:<10} | {benchmark.description}")


def check_requested_benchmarks(benchmarks: list[str]):
    ok: bool = True
    for benchmark in benchmarks:
        if benchmark not in [b.id for b in BENCHMARKS]:
            print(f"Unknown benchmark: {benchmark}")
            ok = False
    return ok


def run_and_report_benchmarks(args, env: Environment):
    print("####################")
    print("# Running benchmarks\n")
    results = run_benchmarks(args, env)
    print("\n#################")
    print("Benchmark results\n")
    record_results(results, args, env)


def run_benchmarks(
    args, env: Environment
) -> list[tuple[Benchmark, str, BenchmarkResult]]:
    if not args.allow_uncommitted and env.repo_state.uncommited_changes:
        print(
            "Uncommited changes detected. Use --allow_uncommitted to run benchmarks anyway"
        )
        sys.exit(1)
    benchmarks = (
        BENCHMARKS
        if len(args.benchmarks) == 0
        else [b for b in BENCHMARKS if b.id in args.benchmarks]
    )
    results = [
        (b, build_config, run_benchmark(b, build_config, env.repo_path))
        for b in benchmarks
        for build_config in args.build_configs
    ]
    return results


def subsititue_vars(arg):
    return arg.replace("$REPO", str(get_repo_base_path()))


def run_benchmark(
    benchmark: Benchmark, build_config: str, repo_path: Path
) -> BenchmarkResult:
    print(f"Running Benchmark {benchmark.name} [{build_config}]")

    build_folder = get_build_folder(repo_path, build_config)
    benchmark_file = build_folder.joinpath(*benchmark.command)

    if sys.platform == "win32":
        benchmark_file = benchmark_file.with_suffix(".exe")

    if not benchmark_file.exists():
        print(f"Could not find benchmark file: {benchmark_file}")
        sys.exit(1)

    benchmark_args = [subsititue_vars(a) for a in benchmark.args]

    cmd = [str(benchmark_file)] + benchmark_args

    return benchmark.parser(
        subprocess.check_output(
            cmd,
            cwd=build_folder,
            text=True,
        )
    )


def record_results(
    results: list[tuple[Benchmark, str, BenchmarkResult]],
    args,
    env: Environment,
):
    results_db = BenchmarkDB()
    results_db.save_result(results, args, env)
    for res in results:
        print(
            f"{res[0].name} [{res[1]}] @ {env.repo_state.commit_hash} x {env.machine_id}:"
        )
        for k, v in res[2].items():
            print(f"  {k}: {v}")


class BenchmarkDB:
    WORKTREE_DIR = get_repo_base_path() / ".benchmark-results"
    BRANCH = "benchmark-results"

    def __init__(self) -> None:
        self.ensure_worktree()

    def save_result(
        self,
        results: list[tuple[Benchmark, str, BenchmarkResult]],
        args: argparse.Namespace,
        env: Environment,
    ) -> None:
        self.pull()

        data = self._get_result_data(env.machine_id)
        commit_indicator = env.repo_state.commit_hash + (
            "-dirty" if env.repo_state.uncommited_changes else ""
        )
        for run in results:
            self._add_benchmark_run(data, commit_indicator, run)
        self._write_result_data(env.machine_id, data)

        if not args.no_commit:
            self._commit_results_file(env.machine_id)
            self.push()

    def ensure_worktree(self) -> None:
        """Prüft, ob der Worktree existiert, und legt ihn andernfalls an."""
        if self.WORKTREE_DIR.exists() and (self.WORKTREE_DIR / ".git").exists():
            return

        run_git(
            ["fetch", "origin", BenchmarkDB.BRANCH],
            cwd=get_repo_base_path(),
            check=False,
        )
        try:
            run_git(
                ["worktree", "add", str(BenchmarkDB.WORKTREE_DIR), BenchmarkDB.BRANCH],
                cwd=get_repo_base_path(),
            )
        except subprocess.CalledProcessError as err:
            raise RuntimeError(
                f"Worktree konnte nicht erstellt werden. "
                f"Existiert der Branch '{BenchmarkDB.BRANCH}' bereits lokal oder remote?\n"
                f"Git-Fehler: {err.stderr}"
            ) from err

    def pull(self) -> None:
        """Holt die neuesten Benchmark-Daten vom Remote-Repository."""
        try:
            run_git(
                ["pull", "--rebase", "origin", BenchmarkDB.BRANCH],
                cwd=BenchmarkDB.WORKTREE_DIR,
            )
        except subprocess.CalledProcessError as err:
            print(f"Hinweis: Pull nicht erfolgreich ({err.stderr.strip()}).")

    def push(self) -> None:
        """Pusht lokale Benchmark-Commits zum Remote-Repository."""
        run_git(
            ["push", "origin", BenchmarkDB.BRANCH],
            cwd=BenchmarkDB.WORKTREE_DIR,
            check=True,
        )

    def get_results_file(self, machine_id: str) -> Path:
        return BenchmarkDB.WORKTREE_DIR / "results" / f"{machine_id}.json"

    def _get_result_data(self, machine_id: str) -> dict:
        results_path = self.get_results_file(machine_id)
        if results_path.exists():
            return json.loads(results_path.read_text())
        else:
            return {}

    def _write_result_data(self, machine_id: str, data: dict):
        results_path = self.get_results_file(machine_id)
        results_path.write_text(json.dumps(data, indent=4))

    def _add_benchmark_run(self, data, commit_indicator, run):
        benchmark, build_config, result = run

        benchmark_run = {
            "benchmark": benchmark.id,
            "build_config": build_config,
            "results": result,
        }

        if commit_indicator in data:
            stored_runs = data[commit_indicator]
            for stored_run in stored_runs:
                if (
                    stored_run["benchmark"] == benchmark.id
                    and stored_run["build_config"] == build_config
                ):
                    stored_run["results"] = result
                    break
            else:
                stored_runs.append(benchmark_run)
        else:
            data[commit_indicator] = [benchmark_run]

    def _commit_results_file(self, machine_id: str) -> None:
        run_git(
            ["add", str(self.get_results_file(machine_id))],
            cwd=BenchmarkDB.WORKTREE_DIR,
            check=True,
        )
        run_git(
            ["commit", "-m", f"Update results for {machine_id}"],
            cwd=BenchmarkDB.WORKTREE_DIR,
            check=True,
        )
