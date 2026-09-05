# ##############################################################################
# Chess Project benchmarking script
# ##############################################################################

import subprocess
from dataclasses import dataclass
from pathlib import Path


@dataclass
class RepoState:
    commit_hash: str
    uncommited_changes: bool


def get_repo_base_path() -> Path:
    return Path(__file__).parent.parent.parent.parent.parent


def list_configurations() -> list[str]:
    build_path = get_repo_base_path() / "build"
    return [f.name for f in build_path.iterdir() if f.is_dir()]


def get_build_folder(config: str) -> Path:
    return get_repo_base_path() / "build" / config


def collect_repo_state() -> RepoState:
    hash_cmd = ["git", "rev-parse", "HEAD"]
    commit_hash = subprocess.check_output(hash_cmd, text=True).strip()
    status_cmd = ["git", "status", "--porcelain"]
    dirty = len(subprocess.check_output(status_cmd, text=True).strip()) > 0
    return RepoState(commit_hash, dirty)


def print_repo_state(state: RepoState):
    print(f"Commit hash: {state.commit_hash}")
    print(f"Uncommited changes: {state.uncommited_changes}")
    print(f"Available configurations:\n  {'\n  '.join(list_configurations())}")
    print()
