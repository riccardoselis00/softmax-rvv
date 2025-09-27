import argparse
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchies.classic.no_cache import NoCache
from gem5.components.memory.single_channel import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.isas import ISA
from gem5.simulate.simulator import Simulator
from gem5.resources.resource import CustomResource

p = argparse.ArgumentParser()
p.add_argument("--bin", required=True, help="Path to RISC-V ELF binary")
args = p.parse_args()

board = SimpleBoard(
    clk_freq="2GHz",
    processor=SimpleProcessor(cpu_type=CPUTypes.TIMING, num_cores=1, isa=ISA.RISCV),
    memory=SingleChannelDDR3_1600("1GiB"),
    cache_hierarchy=NoCache(),
)

# Just run the binary, no arguments needed
board.set_se_binary_workload(CustomResource(args.bin))

sim = Simulator(board=board)
sim.run()
