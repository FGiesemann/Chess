# ##############################################################################
# Chess Project benchmarking script
# ##############################################################################

from .benchmark_types import BenchmarkResult


def parse_perft_benchmark(output: str):
    # Detect a line like "Summary: 12.724 MNPS (Average of Medians)" and extract the number
    result = BenchmarkResult()
    for line in output.splitlines():
        if line.startswith("Summary: "):
            result["mnps"] = float(line.split(" ")[1].split("(")[0])
    return result
