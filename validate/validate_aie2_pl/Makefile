# Copyright 2019-2021 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# vitis makefile-generator v2.0.4

############################## Help Section ##############################
.PHONY: help

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all TARGET=<hw/hw_emu/> PLATFORM=<FPGA platform> HOST_ARCH=<>"
	$(ECHO) "      Command to generate the design for specified Target and Shell."
	$(ECHO) "      By default, HOST_ARCH=x86 HOST_ARCH is required for SoC shells"
	$(ECHO) ""
	$(ECHO) "  make sd_card TARGET=<hw/hw_emu/> PLATFORM=<FPGA platform> HOST_ARCH=<>"
	$(ECHO) "      Command to prepare sd_card files."
	$(ECHO) "      By default, HOST_ARCH=x86 HOST_ARCH is required for SoC shells"
	$(ECHO) ""
	$(ECHO) "  make run TARGET=<hw/hw_emu/> PLATFORM=<FPGA platform> HOST_ARCH=<>"
	$(ECHO) "      Command to run application in emulation."
	$(ECHO) "      By default, HOST_ARCH=x86 HOST_ARCH required for SoC shells"
	$(ECHO) ""
	$(ECHO) "  make build TARGET=<hw/hw_emu/> PLATFORM=<FPGA platform> HOST_ARCH=<>"
	$(ECHO) "      Command to build xclbin application."
	$(ECHO) "      By default, HOST_ARCH=x86 HOST_ARCH is required for SoC shells"
	$(ECHO) ""
	$(ECHO) "  make xclbin TARGET=<hw/hw_emu/> PLATFORM=<FPGA platform> HOST_ARCH=<>"
	$(ECHO) "      Command to build xclbin application."
	$(ECHO) "      By default, HOST_ARCH=x86 HOST_ARCH is required for SoC shells"
	$(ECHO) ""
	$(ECHO) "  make host HOST_ARCH=<hw/hw_emu/>"
	$(ECHO) "      Command to build host application."
	$(ECHO) "      By default, HOST_ARCH=x86 HOST_ARCH is required for SoC shells"
	$(ECHO) ""
	$(ECHO) "  NOTE: For embedded devices, e.g. zcu102/zcu104/vck190, env variable EDGE_COMMON_SW and PERL need to be set first, and HOST_ARCH is either aarch32 or aarch64.  "
	$(ECHO) "  Please download and use the pre-built image from  - "
	$(ECHO) "  https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html. For example,"
	$(ECHO) "       export EDGE_COMMON_SW=< path-to-rootfs-and-Image-files >"
	$(ECHO) ""
	$(ECHO) "  make clean "
	$(ECHO) "      Command to remove the generated non-hardware files."
	$(ECHO) ""
	$(ECHO) "  make cleanall"
	$(ECHO) "      Command to remove all the generated files."
	$(ECHO) ""
	$(ECHO) $(MK_PATH)
	$(ECHO) $(CUR_DIR)
	$(ECHO) $(XFLIB_DIR)

############################## Setting up Project Variables ##############################

MK_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CUR_DIR := $(patsubst %/,%,$(dir $(MK_PATH)))
COMMON_REPO ?= $(shell bash -c 'export MK_PATH=$(MK_PATH); echo $${MK_PATH%validate/validate_aie2_pl/*}')
XF_PROJ_ROOT = $(shell readlink -f $(COMMON_REPO))

# setting devault value
TARGET ?= hw_emu
HOST_ARCH ?= x86

#setting PLATFORM
ifeq ($(PLATFORM),)
PLATFORM := $(DEVICE)
endif
ifeq ($(PLATFORM),)
PLATFORM := v70
endif

# #################### Checking if PLATFORM in allowlist ############################
PLATFORM_ALLOWLIST +=  v70
PLATFORM_BLOCKLIST +=  other

include ./utils.mk
TEMP_DIR := _x_temp.$(TARGET).$(PLATFORM_NAME)
TEMP_REPORT_DIR := $(CUR_DIR)/reports/_x.$(TARGET).$(PLATFORM_NAME)
BUILD_DIR := build_dir.$(TARGET).$(PLATFORM_NAME)
BUILD_REPORT_DIR := $(CUR_DIR)/reports/_build.$(TARGET).$(PLATFORM_NAME)
EMCONFIG := $(BUILD_DIR)/emconfig.json
XCLBIN_DIR := $(CUR_DIR)/$(BUILD_DIR)
export XCL_BINDIR = $(XCLBIN_DIR)

EXE_FILE_DEPS :=
BINARY_CONTAINERS_DEPS :=
RUN_DEPS :=

# get global setting
ifeq ($(HOST_ARCH), x86)
CXXFLAGS += -fmessage-length=0 -I$(CUR_DIR)/sw -I$(XILINX_XRT)/include -I$(XILINX_HLS)/include -std=c++14 -O3 -Wall -Wno-unknown-pragmas -Wno-unused-label  
LDFLAGS += -pthread -L$(XILINX_XRT)/lib -L$(XILINX_HLS)/lnx64/tools/fpo_v7_1  -Wl,--as-needed -lOpenCL -lxrt_coreutil -lgmp -lmpfr -lIp_floating_point_v7_1_bitacc_cmodel 
VPP_FLAGS += -t $(TARGET) --platform $(XPLATFORM) --save-temps 
VPP_LDFLAGS += --optimize 2 -R 2 
else ifeq ($(HOST_ARCH), aarch64)
CXXFLAGS += -I$(CUR_DIR)/sw -fmessage-length=0 --sysroot=$(SYSROOT)  -I$(SYSROOT)/usr/include/xrt -I$(XILINX_HLS)/include -std=c++14 -O3 -Wall -Wno-unknown-pragmas -Wno-unused-label 
LDFLAGS += -pthread -L$(SYSROOT)/usr/lib -L$(XILINX_VITIS_AIETOOLS)/lib/aarch64.o -Wl,--as-needed -lxilinxopencl -lxrt_coreutil 
VPP_FLAGS += -t $(TARGET) --platform $(XPLATFORM) --save-temps 
VPP_LDFLAGS += --optimize 2 -R 2 
endif
CXXFLAGS += $(EXTRA_CXXFLAGS)
VPP_FLAGS += $(EXTRA_VPP_FLAGS)

# aie template
XILINX_VITIS_AIETOOLS ?=
XILINX_VITIS_AIELIBS ?=
ifneq ($(wildcard $(XILINX_VITIS)/aietools/.*),)
XILINX_VITIS_AIETOOLS := $(XILINX_VITIS)/aietools
else
XILINX_VITIS_AIETOOLS := $(XILINX_VITIS)/cardano
XILINX_VITIS_AIELIBS:= -lcardano_api_xrt -lxaiengine -lxrt_core
endif
CXXFLAGS += -I$(XILINX_VITIS_AIETOOLS)/include
LDFLAGS += $(XILINX_VITIS_AIELIBS)
# Setting customized_params in aiecompiler

############################ setting AIE Compiler ###########################
ifneq ($(filter aiesim hw_emu, $(TARGET)),)
AIETARGET := hw
else
AIETARGET := $(TARGET)
endif
AIE_CXXFLAGS += --target=$(AIETARGET) --platform=$(XPLATFORM) --pl-freq=1000 --dataflow -v --aiearch=aie-ml
AIE_CXXFLAGS += -I $(XILINX_VITIS_AIETOOLS)/include
AIE_CONTAINER = $(TEMP_DIR)/libadf.a
AIE_CXXFLAGS += -I $(CUR_DIR)/aie
AIE_CXXFLAGS += -I $(CUR_DIR)/aie/kernels
AIE_CXXFLAGS += -I $(CUR_DIR)/pl_kernels
$(CUR_DIR)/Work/ps/c_rts/aie_control_xrt.cpp: $(AIE_CONTAINER)

EXE_FILE_DEPS += $(AIE_CONTAINER)
BINARY_CONTAINERS_DEPS += $(AIE_CONTAINER)
########################## Setting up Host Variables ##########################
ifeq ($(TARGET),sw_emu)
CXXFLAGS += -D SW_EMU_TEST
endif
ifeq ($(TARGET),hw_emu)
CXXFLAGS += -D HW_EMU_TEST
endif
ifeq ($(TARGET),hw)
CXXFLAGS += -D HW_TEST
endif

#Inclue Required Host Source Files
HOST_SRCS += $(CUR_DIR)//sw/host.cpp $(CUR_DIR)//sw/pl_controller.cpp 
CXXFLAGS +=  -I $(CUR_DIR)//pl_kernels -I $(CUR_DIR)//sw 
LDFLAGS += -lgcc -lc -lxrt_coreutil -lpthread -lrt -ldl -lcrypt -lstdc++ -L$(XILINX_XRT)/lib/  -Wl,-rpath-link,$(XILINX_XRT)/lib

EXE_NAME := host.exe
EXE_FILE := $(BUILD_DIR)/$(EXE_NAME)
EXE_FILE_DEPS := $(HOST_SRCS) $(EXE_FILE_DEPS)

########################## Kernel compiler global settings ##########################
VPP_FLAGS +=  -I $(CUR_DIR)//pl_kernels -I $(XILINX_VITIS)/aietools/include
VPP_LDFLAGS +=   --config $(CUR_DIR)/system.cfg

######################### binary container global settings ##########################
VPP_FLAGS_pl_controller_top += --verbose
VPP_FLAGS_pl_controller_top += --hls.clock 250000000:pl_controller_top
VPP_FLAGS_sender_receiver += --verbose
VPP_FLAGS_sender_receiver += --hls.clock 250000000:sender_receiver
ifneq ($(HOST_ARCH), x86)
VPP_LDFLAGS_v70_pcie_pl_controller.xclbin += --clock.defaultFreqHz 250000000
else
VPP_LDFLAGS_v70_pcie_pl_controller.xclbin += --kernel_frequency 250
endif

ifeq ($(HOST_ARCH), x86)
BINARY_CONTAINERS += $(BUILD_DIR)/v70_pl_controller.xsa
BINARY_CONTAINERS_PKG += $(BUILD_DIR)/v70_pl_controller.xclbin
else
BINARY_CONTAINERS += $(BUILD_DIR)/v70_pl_controller.xsa
BINARY_CONTAINERS_PKG += $(BUILD_DIR)/v70_pl_controller.xclbin
endif

# ################ Setting Rules for Binary Containers (Building Kernels) ################
$(TEMP_DIR)/pl_controller_top.xo: $(CUR_DIR)//pl_kernels/pl_controller_top.cpp 
	$(ECHO) "Compiling Kernel: pl_controller_top"
	mkdir -p $(TEMP_DIR)
	$(VPP) -c $(VPP_FLAGS_pl_controller_top) $(VPP_FLAGS) -k pl_controller_top -I'$(<D)' --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) -o'$@' '$<'
BINARY_CONTAINER_v70_pcie_pl_controller.xclbin_OBJS += $(TEMP_DIR)/pl_controller_top.xo
$(TEMP_DIR)/sender_receiver.xo: $(CUR_DIR)//pl_kernels/sender_receiver.cpp 
	$(ECHO) "Compiling Kernel: sender_receiver"
	mkdir -p $(TEMP_DIR)
	$(VPP) -c $(VPP_FLAGS_sender_receiver) $(VPP_FLAGS) -k sender_receiver -I'$(<D)' --temp_dir $(TEMP_DIR) --report_dir $(TEMP_REPORT_DIR) -o'$@' '$<'
BINARY_CONTAINER_v70_pcie_pl_controller.xclbin_OBJS += $(TEMP_DIR)/sender_receiver.xo
BINARY_CONTAINERS_DEPS += $(BINARY_CONTAINER_v70_pcie_pl_controller.xclbin_OBJS)
BINARY_CONTAINERS_DEPS += $(AIE_CONTAINER)
$(BINARY_CONTAINERS): $(BINARY_CONTAINERS_DEPS)
	mkdir -p $(BUILD_DIR)
	$(VPP) -l $(VPP_FLAGS) --temp_dir $(TEMP_DIR) --report_dir $(BUILD_REPORT_DIR) $(VPP_LDFLAGS)  $(VPP_LDFLAGS_v70_pcie_pl_controller.xclbin) -o $@ $^ 

$(BINARY_CONTAINERS_PKG): $(AIE_CONTAINER) $(BINARY_CONTAINERS)
	${VPP} -p -t ${TARGET} -f ${XPLATFORM} ${AIE_CONTAINER} ${BINARY_CONTAINERS} -o ./${BINARY_CONTAINERS_PKG}

############################## Setting Rules for AIE (Building Kernels) ##################
$(AIE_CONTAINER): $(CUR_DIR)//aie/graph.cpp 
	$(ECHO) "Compiling: libadf.a"
	mkdir -p $(dir $@)
	$(AIECXX) $(AIE_CXXFLAGS) $^
	cp ./libadf.a $(AIE_CONTAINER)

############################## Setting Rules for Host (Building Host Executable) ##############################
ifeq ($(HOST_ARCH), x86)
$(EXE_FILE): $(EXE_FILE_DEPS) |  check_xrt
	mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

else
$(EXE_FILE): $(EXE_FILE_DEPS) |  check_sysroot
	mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

endif

$(EMCONFIG):
	emconfigutil --platform $(XPLATFORM) --od $(BUILD_DIR)
############################## Preparing sdcard folder ##############################
RUN_SCRIPT := $(BUILD_DIR)/run_script.sh
$(RUN_SCRIPT):
	rm -rf $(RUN_SCRIPT)
	@echo 'export LD_LIBRARY_PATH=/mnt:/tmp:$(LIBRARY_PATH)' >> $(RUN_SCRIPT)
ifneq ($(filter sw_emu hw_emu, $(TARGET)),)
	@echo 'export XCL_EMULATION_MODE=$(TARGET)' >> $(RUN_SCRIPT)
endif
	@echo 'export XILINX_VITIS=/mnt' >> $(RUN_SCRIPT)
	@echo 'export XILINX_XRT=/usr' >> $(RUN_SCRIPT)
	@echo 'if [ -f platform_desc.txt  ]; then' >> $(RUN_SCRIPT)
	@echo '        cp platform_desc.txt /etc/xocl.txt' >> $(RUN_SCRIPT)
	@echo 'fi' >> $(RUN_SCRIPT)
	@echo './$(EXE_NAME) $(PKG_HOST_ARGS)' >> $(RUN_SCRIPT)
	@echo 'return_code=$$?' >> $(RUN_SCRIPT)
	@echo 'if [ $$return_code -ne 0 ]; then' >> $(RUN_SCRIPT)
	@echo '        echo "ERROR: Embedded host run failed, RC=$$return_code"' >> $(RUN_SCRIPT)
	@echo 'else' >> $(RUN_SCRIPT)
	@echo '        echo "INFO: TEST PASSED, RC=0"' >> $(RUN_SCRIPT)
	@echo 'fi' >> $(RUN_SCRIPT)
	@echo 'echo "INFO: Embedded host run completed."' >> $(RUN_SCRIPT)
	@echo 'exit $$return_code' >> $(RUN_SCRIPT)
DATA_FILE := 
DATA_DIR := 
SD_FILES += $(RUN_SCRIPT)
SD_FILES += $(EXE_FILE)
SD_FILES += $(EMCONFIG)
SD_FILES += xrt.ini
SD_FILES += $(DATA_FILE)# where define DATAFILE in json
SD_FILES_WITH_PREFIX = $(foreach sd_file,$(SD_FILES), $(if $(filter $(sd_file),$(wildcard $(sd_file))), --package.sd_file $(sd_file)))
SD_DIRS_WITH_PREFIX = $(foreach sd_dir,$(DATA_DIR),--package.sd_dir $(sd_dir))
PACKAGE_FILES := $(BINARY_CONTAINERS)
SD_CARD := $(CUR_DIR)/package_$(TARGET)
$(SD_CARD): $(EXE_FILE) $(BINARY_CONTAINERS) $(RUN_SCRIPT) $(EMCONFIG)
	@echo "Generating sd_card folder...."
	mkdir -p $(SD_CARD)
	chmod a+rx $(BUILD_DIR)/run_script.sh
	${VPP} -p -t ${TARGET} -f ${XPLATFORM} ${AIE_CONTAINER} ${BINARY_CONTAINERS} -o ./pl_controller_aie.xclbin --package.boot_mode=ospi --package.defer_aie_run
	@echo "### ***** sd_card generation done! ***** ###"

.PHONY: sd_card
sd_card: $(SD_CARD)

############################## Setting Essential Checks and Building Rules ##############################
ifneq (,$(filter x86sim aiesim, $(TARGET)))
RUN_DEPS += $(AIE_CONTAINER)
else
RUN_DEPS += $(EXE_FILE) $(BINARY_CONTAINERS) $(EMCONFIG)
RUN_DEPS += $(SD_CARD)
endif
HOST_ARGS += $(CUR_DIR)/pl_controller_aie.xclbin
run: check_device  $(RUN_DEPS)
#x86sim
ifeq ($(TARGET), x86sim)
	$(X86SIMULATOR) --pkg-dir=./Work 

endif
#aiesim
ifeq ($(TARGET), aiesim)
	$(AIESIMULATOR) --pkg-dir=./Work --profile 

endif
#hw_emu
ifneq (,$(filter hw_emu, $(TARGET)))
ifeq ($(HOST_ARCH), x86)
	cp Work/reports/dma_lock_report.json ./
	cp Work/ps/c_rts/aie_control_config.json ./
	LD_LIBRARY_PATH=$(LIBRARY_PATH):$$LD_LIBRARY_PATH \
	XCL_EMULATION_MODE=$(TARGET) $(EXE_FILE) $(HOST_ARGS)

else
	@echo $(RUN_DEPS)
	$(SD_CARD)/launch_$(TARGET).sh -no-reboot -run-app $(notdir $(RUN_SCRIPT)) 
	grep "TEST PASSED, RC=0" $(SD_CARD)/qemu_output.log || exit 1

endif
endif
#sw_emu
ifneq (,$(filter sw_emu, $(TARGET)))
ifeq ($(HOST_ARCH), x86)
	LD_LIBRARY_PATH=$(LIBRARY_PATH):$$LD_LIBRARY_PATH \
	XCL_EMULATION_MODE=$(TARGET) $(EXE_FILE) $(HOST_ARGS) 

else
	@echo $(RUN_DEPS)
	$(SD_CARD)/launch_$(TARGET).sh -no-reboot -run-app $(notdir $(RUN_SCRIPT)) 
	grep "TEST PASSED, RC=0" $(SD_CARD)/qemu_output.log || exit 1

endif
endif
#hw
ifeq ($(TARGET), hw)
ifeq ($(HOST_ARCH), x86)
	$(EXE_FILE) $(HOST_ARGS)

else
	$(ECHO) "Please copy the content of sd_card folder and data to an SD Card and run on the board"
endif
endif

############################## Setting Targets ##############################

.PHONY: all clean cleanall emconfig
emconfig: $(EMCONFIG)
ifeq ($(HOST_ARCH), x86)
all:  check_vpp check_platform check_xrt $(EXE_FILE) $(BINARY_CONTAINERS) emconfig $(BINARY_CONTAINERS_PKG)
else
all:  check_vpp check_platform check_sysroot $(EXE_FILE) $(BINARY_CONTAINERS) emconfig $(BINARY_CONTAINERS_PKG) sd_card
endif

.PHONY: host
ifeq ($(HOST_ARCH), x86)
host:  check_xrt $(EXE_FILE)
else
host:  check_sysroot $(EXE_FILE)
endif

.PHONY: build
ifeq ($(HOST_ARCH), x86)
build:  check_vpp $(BINARY_CONTAINERS) $(AIE_CONTAINER) $(BINARY_CONTAINERS_PKG)
else
build:  check_vpp check_sysroot $(BINARY_CONTAINERS) $(AIE_CONTAINER) $(BINARY_CONTAINERS_PKG)
endif

.PHONY: xclbin
ifeq ($(HOST_ARCH), x86)
xclbin:  check_vpp check_device $(BINARY_CONTAINERS) $(AIE_CONTAINER) $(BINARY_CONTAINERS_PKG) $(RUN_DEPS)
else
xclbin:  check_vpp check_sysroot $(BINARY_CONTAINERS) $(AIE_CONTAINER) $(BINARY_CONTAINERS_PKG)
endif

.PHONY: x86sim
x86sim: $(AIE_CONTAINER)
	$(X86SIMULATOR) --pkg-dir=./Work

############################## Cleaning Rules ##############################
cleanh:
	-$(RMDIR) $(EXE_FILE) vitis_* TempConfig system_estimate.xtxt *.rpt .run/
	-$(RMDIR) src/*.ll _xocc_* .Xil dltmp* xmltmp* *.log *.jou *.wcfg *.wdb sample_link.ini sample_compile.ini obj*  bin* *.csv *.jpg *.jpeg *.png

cleank:
	-$(RMDIR) $(BUILD_DIR)/*.xclbin _vimage *xclbin.run_summary qemu-memory-_* emulation/ _vimage/ pl*start_simulation. sh *.xclbin
	-$(RMDIR) _x_temp.*/_x.* _x_temp.*/.Xil _x_temp.*/profile_summary.* xo_* _x*
	-$(RMDIR) _x_temp.*/dltmp* _x_temp.*/kernel_info.dat _x_temp.*/*.log
	-$(RMDIR) _x_temp.* 

cleanall: cleanh cleank
	-$(RMDIR) $(BUILD_DIR) sd_card* build_dir.* emconfig.json *.html $(TEMP_DIR) $(CUR_DIR)/reports *.csv *.run_summary  $(CUR_DIR)/*.raw package_*   $(BUILD_DIR)/run_script.sh .ipcache *.str
	-$(RMDIR) $(CUR_DIR)/common/data/*.xe2xd* $(CUR_DIR)/common/data/*.orig*
	-$(RMDIR) $(AIE_CONTAINER) aiesimulator_output .AIE_SIM_CMD_LINE_OPTIONS $(CUR_DIR)/Work $(CUR_DIR)/*.xpe $(CUR_DIR)/hw.o $(CUR_DIR)/*.xsa $(CUR_DIR)/xnwOut
	-$(RMDIR) $(CUR_DIR)/BOOT*
	-$(RMDIR) $(CUR_DIR)/aie_control_config.json
	-$(RMDIR) $(CUR_DIR)/dma_lock_report.json
	-$(RMDIR) $(CUR_DIR)/launch_hw_emu.sh
	-$(RMDIR) $(CUR_DIR)/*.txt
	-$(RMDIR) $(CUR_DIR)/aie.merged.cdo.bin
	-$(RMDIR) $(CUR_DIR)/pl_controller_aie.xclbin.package_summary
	-$(RMDIR) $(CUR_DIR)/*.bin
	-$(RMDIR) $(CUR_DIR)/libadf.a
	-$(RMDIR) $(CUR_DIR)/boot_image.bif
	-$(RMDIR) $(CUR_DIR)/sim
	-$(RMDIR) $(CUR_DIR)/cfg
	-$(RMDIR) $(CUR_DIR)/emulation_data

clean: cleanh
