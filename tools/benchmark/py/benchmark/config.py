# ##############################################################################
# Chess Project benchmarking script
# ##############################################################################

from pathlib import Path

import tomllib

from .environment import get_repo_base_path


def get_config_path(config_path: Path) -> Path:
    if not config_path.is_absolute():
        repo_base = get_repo_base_path()
        benchmark_base = repo_base / "tools" / "benchmark" / "py" / "benchmark"
        return benchmark_base / config_path
    else:
        return config_path


def load_config(config_path: Path) -> dict:
    config_file = get_config_path(config_path)
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
        if "configs" in benchmark_data:
            configuration["build_configs"] = benchmark_data["configs"]

    return configuration
