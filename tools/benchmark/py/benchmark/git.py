# ##############################################################################
# Chess Project benchmarking script
# ##############################################################################

import subprocess
from pathlib import Path


def run_git(
    args: list[str], cwd: Path, check: bool = True
) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        ["git"] + args, cwd=cwd, check=check, capture_output=True, text=True
    )


def get_git_output(args: list[str], cwd: Path, check: bool = True) -> str:
    return run_git(args, cwd, check).stdout
