# configs/rv-hello-world.py
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchies.classic.no_cache import NoCache
from gem5.components.memory.single_channel import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.isas import ISA
from gem5.simulate.simulator import Simulator
from gem5.resources.resource import CustomResource
from pathlib import Path

# pick your CPU model:
CPU = CPUTypes.TIMING   # or CPUTypes.ATOMIC for faster bring-up

board = SimpleBoard(
    clk_freq="2GHz",
    processor=SimpleProcessor(cpu_type=CPU, num_cores=1, isa=ISA.RISCV),
    memory=SingleChannelDDR3_1600("1GiB"),
    cache_hierarchy=NoCache(),
)

# point to your RISC-V static hello
hello_path = Path(__file__).resolve().parent.parent / "benches" / "rv-hello-world"
board.set_se_binary_workload(CustomResource(str(hello_path)))

sim = Simulator(board=board)
sim.run()

