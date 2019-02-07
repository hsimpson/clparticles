#include "openclinfo.h"
#include <iostream>


void OpenCLInfo::showInfo() {
  std::cout << "OpenCL Info:" << std::endl;

  // get all platforms
  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);

  for (int i = 0; i < platforms.size(); ++i) {
    cl::Platform platform = platforms[i];
    std::cout << "Platform[" << i << "] NAME: " << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;
    std::cout << "Platform[" << i << "] VENDOR: " << platform.getInfo<CL_PLATFORM_VENDOR>() << std::endl;
    std::cout << "Platform[" << i << "] VERSION: " << platform.getInfo<CL_PLATFORM_VERSION>() << std::endl;
    std::cout << "Platform[" << i << "] PROFILE: " << platform.getInfo<CL_PLATFORM_PROFILE>() << std::endl;
    //std::cout << "Platform[" << i << "] EXTENSIONS: " << platform.getInfo<CL_PLATFORM_EXTENSIONS>() << std::endl;

    // get all devices for this platform
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

    for (int j = 0; j < devices.size(); ++j) {
      cl::Device device = devices[j];
      std::cout << "    Device[" << j << "] NAME: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
      std::cout << "    Device[" << j << "] VENDOR: " << device.getInfo<CL_DEVICE_VENDOR>() << std::endl;
      std::cout << "    Device[" << j << "] TYPE: " << OpenCLInfo::getDeviceType(device.getInfo<CL_DEVICE_TYPE>()) << std::endl;
      std::cout << "    Device[" << j << "] DRIVER_VERSION: " << device.getInfo<CL_DRIVER_VERSION>() << std::endl;
      std::cout << "    Device[" << j << "] PROFILE: " << device.getInfo<CL_DEVICE_PROFILE>() << std::endl;
      std::cout << "    Device[" << j << "] VERSION: " << device.getInfo<CL_DEVICE_VERSION>() << std::endl;
      std::cout << "    Device[" << j << "] MAX_WORK_GROUP_SIZE: " << device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << std::endl;
      std::cout << "    Device[" << j << "] MAX_WORK_ITEM_DIMENSIONS: " << device.getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>() << std::endl;
      std::cout << "    Device[" << j << "] MAX_WORK_ITEM_SIZES: ";

      std::vector<size_t> work_item_sizes = device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
      for (int k = 0; k < work_item_sizes.size(); ++k) {
        std::cout << work_item_sizes[k];
        if (k < work_item_sizes.size() - 1) {
          std::cout << " x ";
        }
      }
      std::cout << std::endl;

      std::cout << "    Device[" << j << "] EXTENSIONS: " << device.getInfo<CL_DEVICE_EXTENSIONS>() << std::endl;
    }
  }



}

std::string OpenCLInfo::getDeviceType(cl_device_type type) {
  std::string typeString;
  switch (type) {
    case CL_DEVICE_TYPE_DEFAULT:
      typeString = "DEFAULT";
      break;
    case CL_DEVICE_TYPE_CPU:
      typeString = "CPU";
      break;
    case CL_DEVICE_TYPE_GPU:
      typeString = "GPU";
      break;
    case CL_DEVICE_TYPE_ACCELERATOR:
      typeString = "ACCELERATOR";
      break;
    case CL_DEVICE_TYPE_CUSTOM:
      typeString = "CUSTOM";
      break;
    case CL_DEVICE_TYPE_ALL:
      typeString = "ALL";
      break;
    default:
      typeString = "UNKNOWN";
      break;
  }
  return typeString;
}
