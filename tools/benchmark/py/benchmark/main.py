import argparse
import socket
import sys
from pathlib import Path

import tomllib

from .benchmark import (
    Benchmark,
    BenchmarkResult,
    check_requested_benchmarks,
    list_benchmarks,
    run_benchmarks,
)
from .environment import (
    Environment,
    collect_repo_state,
    get_repo_base_path,
    list_configurations,
)


def main():
    arg_parser = argparse.ArgumentParser(
        description="Run benchmarks and store results",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    arg_parser.add_argument(
        "-c",
        "--config",
        type=Path,
        default=Path("config.toml"),
        help="Configuration file",
    )

    subparsers = arg_parser.add_subparsers(dest="command", required=True)

    list_parser = subparsers.add_parser(
        "list",
        help="List available benchmarks",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    list_parser.set_defaults(func=lambda _1, _2: list_benchmarks())

    status_parser = subparsers.add_parser(
        "status",
        help="Show environment status and available build configurations",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    status_parser.set_defaults(func=lambda args, env: print_environment(args, env))

    run_parser = subparsers.add_parser(
        "run",
        help="Run benchmarks",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    run_parser.add_argument(
        "-b",
        "--benchmarks",
        action="append",
        default=[],
        help="Specify the benchmark (ids) to execute",
    )
    run_parser.add_argument(
        "-a",
        "--allow-uncommitted",
        action="store_true",
        default=False,
        help="Run benchmarks even if there are uncommited changes",
    )
    run_parser.add_argument(
        "-C",
        "--build-config",
        default=None,
        help="Build configuration to use",
    )
    run_parser.add_argument(
        "-m",
        "--machine",
        default=socket.gethostname(),
        help="Machine to run benchmarks on",
    )
    run_parser.set_defaults(func=lambda args, env: run_and_report_benchmarks(args, env))

    temp_args, _ = arg_parser.parse_known_args()
    config = load_config(temp_args.config)
    run_parser.set_defaults(**config)

    args = arg_parser.parse_args()
    check_required_args(args)
    environment = Environment(
        machine_id=args.machine if hasattr(args, "machine") else socket.gethostname(),
        repo_state=collect_repo_state(),
    )
    args.func(args, environment)


def load_config(config_path: Path) -> dict:
    if not config_path.is_absolute():
        repo_base = get_repo_base_path()
        benchmark_base = repo_base / "tools" / "benchmark" / "py" / "benchmark"
        config_file = benchmark_base / config_path
    else:
        config_file = config_path
    if not config_file.is_file():
        print(f"Config file not found: {config_file}")
        return {}

    config_data = {}
    with open(config_file, "rb") as f:
        config_data = tomllib.load(f)

    configuration = {}
    if "machine" in config_data:
        machine_data = config_data["machine"]
        if "id" in machine_data:
            configuration["machine"] = machine_data["id"]
    if "benchmark" in config_data:
        benchmark_data = config_data["benchmark"]
        if "default_config" in benchmark_data:
            configuration["build_config"] = benchmark_data["default_config"]

    return configuration


def check_required_args(args):
    if args.command == "run":
        if not hasattr(args, "build_config") or args.build_config is None:
            print("Missing required argument: --build_config")
            sys.exit(1)
        if not check_requested_benchmarks(args.benchmarks):
            sys.exit(1)


def print_environment(_, env: Environment):
    print("Machine information:")
    print(f"  Machine-Id: {env.machine_id}")
    print("Repository state:")
    print(f"  Commit hash       : {env.repo_state.commit_hash}")
    print(f"  Uncommited changes: {env.repo_state.uncommited_changes}")
    print("Available configurations:")
    print("  " + "\n  ".join(list_configurations()))


def run_and_report_benchmarks(args, env: Environment):
    if not args.allow_uncommitted and env.repo_state.uncommited_changes:
        print(
            "Uncommited changes detected. Use --allow_uncommitted to run benchmarks anyway"
        )
        sys.exit(1)

    results = run_benchmarks(args)
    record_results(results, args, env)


def record_results(
    results: list[tuple[Benchmark, BenchmarkResult]], args, env: Environment
):
    # TODO
    for res in results:
        print(f"{res[0].name}:")
        for k, v in res[1].items():
            print(f"  {k}: {v}")


if __name__ == "__main__":
    main()
