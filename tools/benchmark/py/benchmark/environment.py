# ##############################################################################
# Chess Project benchmarking script
# ##############################################################################

from dataclasses import dataclass
from pathlib import Path

from .git import get_git_output


@dataclass
class RepoState:
    commit_hash: str
    uncommited_changes: bool


@dataclass
class Environment:
    machine_id: str
    repo_path: Path
    repo_state: RepoState


def get_repo_base_path() -> Path:
    return Path(__file__).parent.parent.parent.parent.parent


def get_build_folder(repo_path: Path, config: str) -> Path:
    return repo_path / "build" / config


def list_configurations() -> list[str]:
    build_path = get_repo_base_path() / "build"
    return [f.name for f in build_path.iterdir() if f.is_dir()]


def resolve_repo_path(path: Path) -> Path:
    if not path.is_absolute():
        return (get_repo_base_path() / path).resolve()
    else:
        return path.resolve()


def collect_repo_state(repo_path: Path) -> RepoState:
    commit_hash = get_git_output(["rev-parse", "HEAD"], repo_path).strip()
    dirty = len(get_git_output(["status", "--porcelain"], repo_path)) > 0
    return RepoState(commit_hash, dirty)


def print_environment(_, env: Environment):
    print("Machine information:")
    print(f"  Machine-Id: {env.machine_id}")
    print("Repository:")
    print(f"  Path              : {env.repo_path}")
    print(f"  Commit hash       : {env.repo_state.commit_hash}")
    print(f"  Uncommited changes: {env.repo_state.uncommited_changes}")
    print("Available configurations:")
    print("  " + "\n  ".join(list_configurations()))
