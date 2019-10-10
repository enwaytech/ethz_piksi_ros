#include "piksi_multi_cpp/device/device_usb.h"

#include <ros/console.h>
#include <memory>
#include <vector>
#include "piksi_multi_cpp/device/device_usb.h"

namespace piksi_multi_cpp {

Device::Device(const SerialNumber& sn) : serial_number_(sn) {}

int32_t Device::read_redirect(uint8_t* buff, uint32_t n, void* context) {
  if (!context) {
    ROS_ERROR_STREAM("No context set.");
    return 0;
  }
  // Cast context to instance.
  Device* instance = static_cast<Device*>(context);
  // Execute instance's read function.
  return instance->read(buff, n);
}

}  // namespace piksi_multi_cpp
