AIE App with PL as HLS functional kernels
=========================================

This example demonstrates the flow on how to connect aie Kernel to PL HLS functional Kernels and validate the design running on the AI Engine.

**KEY CONCEPTS:** `Emulation Functional mode <https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Working-with-Functional-Model-of-the-HLS-Kernel>`__, `AI Engine <https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/AI-Engine/Programmable-Logic-Integration>`__, `Adaptive Data Flow <https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Adaptive-Data-Flow-Graph-Specification-Reference>`__, `Graph <https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/graph>`__

**KEYWORDS:** `emulationMode=func <https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Working-with-Functional-Model-of-the-HLS-Kernel>`__, `aiecompiler <https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Compiling-an-AI-Engine-Graph-Application>`__, `aiesimulator <https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Simulating-an-AI-Engine-Graph-Application>`__, `stream_connect <https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Specifying-Streaming-Connections-between-Compute-Units>`__

.. raw:: html

 <details>

.. raw:: html

 <summary> 

 <b>EXCLUDED PLATFORMS:</b>

.. raw:: html

 </summary>
|
..

 - All Embedded Zynq Platforms, i.e zc702, zcu102 etc
 - AWS VU9P F1
 - Samsung SmartSSD Computation Storage Drive
 - Samsung U.2 SmartSSD
 - Versal VCK5000
 - All Xilinx DMA Platforms

.. raw:: html

 </details>

.. raw:: html

DESIGN FILES
------------

Application code is located in the src directory. Accelerator binary files will be compiled to the xclbin directory. The xclbin directory is required by the Makefile and its contents will be filled during compilation. A listing of all the files in this example is shown below

::

   data/golden.txt
   data/input.txt
   data/input1.txt
   src/aie/graph.cpp
   src/aie/graph.h
   src/aie/include.h
   src/aie/kernels.h
   src/aie/kernels/classifiers/classify.cc
   src/aie/kernels/decimators/hb27.cc
   src/aie/kernels/interpolators/hb27_2i.cc
   src/pl_kernels/mm2s.cpp
   src/pl_kernels/polar_clip.cpp
   src/pl_kernels/s2mm.cpp
   src/sw/golden.h
   src/sw/host.cpp
   src/sw/host.h
   src/sw/input.h
   
COMMAND LINE ARGUMENTS
----------------------

Once the environment has been configured, the application can be executed by

::

   ./aie_func_mode <aie-pl_func XCLBIN>

DETAILS
-------

This example demonstrates the flow on how to connect aie Kernel to PL HLS functional Kernels and validate the design running on the AI Engine. The PL kernels in this AIE design are simulated in the functional mode. The functional mode in v++ generates the SCTLM code for the HLS kernel. The functional model of the PL HLS kernel is applicable when user wants to speedup the emulation by compiling desired kernels in functional mode instead of the default RTL. 
In this example, there is one AIE kernel and three PL HLS kernels ``pl_kernels/mm2s.cpp``, ``pl_kernels/s2mm.cpp`` and ``pl_kernels/polar_clip.cpp`` compiled in functional mode.
For more details on PL HLS functional mode in HW emulation, please refer the example `MM Stream Func Mode <../mm_stream_func_mode>`__

XO generation
--------------
During v++ compile flow, while creating hw_emu XO, the user provides a switch describing the intention to do a functional simulation 
that will generate XO with the SystemC wrapper on C code.

Input from the user
--------------------
1. Create a config file config <hw_emu_func.cfg> with following ``advanced`` compiler options: 

.. code:: cpp

   [advanced]
   param=compiler.emulationMode=func

2. In the Makefile, following flag is added in the v++ flow while compiling kernel:

::

      VPP_XO_FLAGS := --config hw_emu_func.cfg  

Speedup Benchmark   
------------------

Below is the runtime recorded for each suite of PL HLS kernels when compiled in functional mode vs the RTL. The speedup is observed as of 3x (approx).
 
============================= ========================
     Kernel                    Runtime (s) 
============================= ========================
  mm2s-polar_clip-s2mm FUNC        127.8 
----------------------------- ------------------------   
  mm2s-polar_clip-s2mm RTL         414.26
----------------------------- ------------------------
  Speedup:                         3.24x  
============================= ======================== 

To visit github.io of this repository, `click here <http://xilinx.github.io/Vitis_Accel_Examples>`__.
