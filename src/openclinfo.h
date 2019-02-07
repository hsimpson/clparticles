#pragma once

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

class OpenCLInfo {

  public:
    static void showInfo();
   static std::string getDeviceType(cl_device_type type);
};
