#include "cmdlineparser.h"
#include <iostream>
#include <cstring>
#include "ap_int.h"

// XRT includes
#include <xrt/xrt_bo.h>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>
#include <xrt/xrt_uuid.h>

//#include "../../../include/xclhal2.h"
#include <xrt.h>
//#include <xrt.h>
#include <array>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
using namespace std;
using std::array;
using std::map;
using std::vector;

#define DATA_SIZE 1000
#define DWIDTH 512
#define DEST_0 0
#define DEST_1 1



int main(int argc, char** argv) {
    // Command Line Parser
    sda::utils::CmdLineParser parser;

    // Switches
    //**************//"<Full Arg>",  "<Short Arg>", "<Description>", "<Default>"
    parser.addSwitch("--xclbin_file", "-x", "input binary file string", "");
    parser.addSwitch("--xclbin_file2", "-x2", "input binary file string", "");

    parser.addSwitch("--device_id", "-d", "device index", "1");
    parser.parse(argc, argv);


    vector<std::string> binaryFile(2);   
    binaryFile[0] = parser.value("xclbin_file");
    binaryFile[1] = parser.value("xclbin_file2");
  
   //int device_index = stoi(parser.value("device_id"));


    if (argc < 5) {
        parser.printHelp();
        return EXIT_FAILURE;
    }



    // Read settings
 
   auto device_count = xclProbe(); 
   std::cout << "Devices found on System : " << device_count << std::endl;
  


   // TODO : Device Name //
/*   xclDeviceHandle handle; 
   struct xclDeviceInfo2 *info;
   const char* unused1; enum xclVerbosityLevel unused2;

for(int d=0; d<device_count; ++d)
  {

     handle = xclOpen(d,unused1,unused2);
     info = xclDeviceInfo2();
     xclGetDeviceInfo2(handle, info);
     std::cout << "error : " << std::endl;
    // std::cout << "Devices: " << info->mMagic << std::endl;
  
  }
*/



   vector<xrt::device> devices(device_count);
   vector<xrt::uuid> uuids(device_count);
   
  vector<xrt::kernel> krnl_mm2s(device_count);
  vector<xrt::kernel> krnl_s2mm(device_count);
  
  vector<xrt::bo> bo_in(device_count);
  vector<xrt::bo> bo_out(device_count);

  vector<xrt::run> run_tx(device_count);
  vector<xrt::run> run_rx(device_count);


    size_t vector_size_bytes = sizeof(ap_uint<DWIDTH>) * DATA_SIZE;

vector < ap_uint<DWIDTH>* > bo_in_map(device_count);
vector < ap_uint<DWIDTH>* > bo_out_map(device_count);


  for(int d=0; d<device_count; ++d) 
  {
     std::cout << "Open the device " << d << std::endl;
      devices[d] = xrt::device(d);
     std::cout << "Load the xclbin " << binaryFile[d] << std::endl;
      uuids[d] = devices[d].load_xclbin(binaryFile[d]);    
  }

  for(int d=0; d<device_count; ++d) 
  {

    krnl_mm2s[d] = xrt::kernel(devices[d], uuids[d], "krnl_mm2s_0");
    krnl_s2mm[d] = xrt::kernel(devices[d], uuids[d], "krnl_s2mm_0");

    std::cout << "Allocate Buffer in Global Memory for device " <<d<<endl;
    bo_in[d]= xrt::bo(devices[d], vector_size_bytes, krnl_mm2s[d].group_id(0));
    bo_out[d] = xrt::bo(devices[d], vector_size_bytes, krnl_s2mm[d].group_id(0));
   
    // Map the contents of the buffer object into host memory

	bo_in_map[d] = bo_in[d].map<ap_uint<DWIDTH>*>();
	bo_out_map[d] = bo_out[d].map<ap_uint<DWIDTH>*>();
   
    std::cout << "Creating Data for device " <<d<<endl;;

    // Create the test data
    for (ap_uint<DWIDTH> i = 0; i < DATA_SIZE; ++i)
        bo_in_map[d][i] = d*i+1;

    // Synchronize buffer content with device side
       std::cout << "synchronize input buffer data to device global memory for 0\n";

       bo_in[d].sync(XCL_BO_SYNC_BO_TO_DEVICE);

  }

 

  for(int d=0; d<device_count; ++d) 
  {
    std::cout << "Execution of the kernel for device" <<d<<endl;

    run_tx[d] = xrt::run(krnl_mm2s[d]);
    run_tx[d].set_arg(0, bo_in[d]);
    run_tx[d].set_arg(2, vector_size_bytes);
    run_tx[d].set_arg(3, DEST_0);
    run_tx[d].start();
 }


//---------------------------------------------

  for(int d=0; d<device_count; ++d) 
  {
    run_rx[d] = xrt::run(krnl_s2mm[d]);
    run_rx[d].set_arg(0, bo_out[d]);
    run_rx[d].set_arg(2, vector_size_bytes);
    run_rx[d].start();
  }


  std::cout << "waiting for tx data"<<endl;

  for(int d=0; d<device_count; ++d) 
  {
    run_tx[d].wait();
  }

 std::cout << "waiting for rx data"<<endl;
  for(int d=0; d<device_count; ++d) 
  {
    run_rx[d].wait();
  }

    // Get the output;

  for(int d=0; d<device_count; ++d) 
  {
    std::cout << "Get the output data from the device 0 " << std::endl;
    bo_out[d].sync(XCL_BO_SYNC_BO_FROM_DEVICE);

   for(ap_uint<DWIDTH> i=0;i<DATA_SIZE;i++)
      std::cout<<"Sent="<<bo_in_map[d][i]<<"\t\t"<<"Received="<<bo_out_map[d][i]<<std::endl;
  }
 
  for(int d=0,d2=device_count - 1; d<device_count; ++d, --d2 ) 
  {
    if (std::memcmp(bo_out_map[d], bo_in_map[d2], DATA_SIZE))
        throw std::runtime_error("Value read back does not match reference"); 
   }
    std::cout << "TEST PASSED\n";
    return 0;
}
