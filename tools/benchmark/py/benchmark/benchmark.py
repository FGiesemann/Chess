import subprocess
import sys

from .benchmark_types import Benchmark, BenchmarkResult
from .environment import get_build_folder, get_repo_base_path
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


def run_benchmarks(args) -> list[tuple[Benchmark, BenchmarkResult]]:
    benchmarks = (
        BENCHMARKS
        if len(args.benchmarks) == 0
        else [b for b in BENCHMARKS if b.id in args.benchmarks]
    )
    results = [(b, run_benchmark(b, args)) for b in benchmarks]
    return results


def subsititue_vars(arg):
    return arg.replace("$REPO", str(get_repo_base_path()))


def run_benchmark(benchmark: Benchmark, args) -> BenchmarkResult:
    print(f"Running Benchmark {benchmark.name}")

    build_folder = get_build_folder(args.build_config)
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
