# ##############################################################################
# Chess Project benchmarking script
# ##############################################################################

import socket

from .arguments import check_required_args, create_argparser
from .config import load_config
from .environment import Environment, collect_repo_state, resolve_repo_path


def main():
    arg_parser, subparsers = create_argparser()
    temp_args, _ = arg_parser.parse_known_args()
    config = load_config(temp_args.config)
    if temp_args.command in subparsers:
        subparsers[temp_args.command].set_defaults(**config)

    args = arg_parser.parse_args()
    check_required_args(args)
    repo_path = resolve_repo_path(args.directory)
    environment = Environment(
        machine_id=args.machine if hasattr(args, "machine") else socket.gethostname(),
        repo_path=repo_path,
        repo_state=collect_repo_state(repo_path),
    )
    args.func(args, environment)


if __name__ == "__main__":
    main()
