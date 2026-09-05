# ##############################################################################
# Chess Project benchmarking script
# ##############################################################################

from collections import OrderedDict
from collections.abc import Callable
from dataclasses import dataclass


@dataclass
class Benchmark:
    id: str
    name: str
    description: str
    command: list[str]
    args: list[str]
    parser: Callable


BenchmarkResult = OrderedDict
