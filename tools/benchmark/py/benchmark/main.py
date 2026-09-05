import argparse
import socket
import sys

from .benchmark import Benchmark, BenchmarkResult, list_benchmarks, run_benchmarks
from .environment import collect_repo_state, list_configurations


def main():
    arg_parser = argparse.ArgumentParser(
        description="Run benchmark tools",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    arg_parser.add_argument(
        "-m", "--machine", default=socket.gethostname(), help="Machine identification"
    )
    subparsers = arg_parser.add_subparsers(dest="command", required=True)

    list_parser = subparsers.add_parser(
        "list",
        help="List available benchmarks",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    list_parser.set_defaults(func=lambda args: list_benchmarks())

    status_parser = subparsers.add_parser(
        "status",
        help="Show environment status and available build configurations",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    status_parser.set_defaults(func=lambda args: print_environment(args))

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
        "-c",
        "--config",
        action="store",
        default=None,
        required=True,
        help="Build configuration to use",
    )
    run_parser.set_defaults(func=lambda args: run_and_report_benchmarks(args))

    args = arg_parser.parse_args()
    args.func(args)


def print_environment(args):
    print("Machine information:")
    print(f"  Machine-Id: {args.machine}")
    repo_state = collect_repo_state()
    print("Repository state:")
    print(f"  Commit hash       : {repo_state.commit_hash}")
    print(f"  Uncommited changes: {repo_state.uncommited_changes}")
    print("Available configurations:")
    print("  " + "\n  ".join(list_configurations()))


def run_and_report_benchmarks(args):
    if not args.allow_uncommitted:
        repo_state = collect_repo_state()
        if repo_state.uncommited_changes:
            print(
                "Uncommited changes detected. Use --allow_uncommitted to run benchmarks anyway"
            )
            sys.exit(1)

    results = run_benchmarks(args)
    record_results(results, args)


def record_results(results: list[tuple[Benchmark, BenchmarkResult]], args):
    # TODO
    for res in results:
        print(f"{res[0].name}:")
        for k, v in res[1].items():
            print(f"  {k}: {v}")


if __name__ == "__main__":
    main()
