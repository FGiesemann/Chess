# ##############################################################################
# Chess Project benchmarking script
# ##############################################################################

import argparse
import socket
import sys
from pathlib import Path

from .benchmark import (
    check_requested_benchmarks,
    list_benchmarks,
    run_and_report_benchmarks,
)
from .environment import print_environment


def create_argparser() -> tuple[
    argparse.ArgumentParser, dict[str, argparse.ArgumentParser]
]:
    arg_parser = argparse.ArgumentParser(
        description="Run benchmarks and store results",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    arg_parser.add_argument(
        "-c",
        "--config",
        type=Path,
        default=Path("config.toml"),
        help="Path to or name of the configuration file",
    )
    arg_parser.add_argument(
        "-d",
        "--directory",
        type=Path,
        default=Path("."),
        help="Path to the repository that should be benchmarked",
    )

    subparsers = arg_parser.add_subparsers(dest="command", required=True)

    subparser_dict = {}

    list_parser = subparsers.add_parser(
        "list",
        help="List available benchmarks",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    list_parser.set_defaults(func=lambda _1, _2: list_benchmarks())
    subparser_dict["list"] = list_parser

    status_parser = subparsers.add_parser(
        "status",
        help="Show environment status and available build configurations",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    status_parser.set_defaults(func=lambda args, env: print_environment(args, env))
    subparser_dict["status"] = status_parser

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
    subparser_dict["run"] = run_parser

    return arg_parser, subparser_dict


def check_required_args(args):
    if args.command == "run":
        if not hasattr(args, "build_config") or args.build_config is None:
            print("Missing required argument: --build_config")
            sys.exit(1)
        if not check_requested_benchmarks(args.benchmarks):
            sys.exit(1)
