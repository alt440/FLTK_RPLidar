#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "rplidar.h" //RPLIDAR standard sdk, all-in-one header

//a simple count...
#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

//to recognize the RPLIDAR driver...
using namespace rp::standalone::rplidar;

//from scanView.h
struct scanDot {
    _u8   quality;
    float angle;
    float dist;
};

int main(int argc, const char * argv[]){
  //so first, where is our device?
  const char * device_path = NULL;
  _u32         baudrate = 115200;
  u_result     result;

  if (argc < 2) {
      //print_usage(argc, argv);
      return -1;
  }

  //set the device path with the first argument.
  device_path = argv[1];
  //if there is a second argument, set the baud rate.
  if (argc>2) baudrate = strtoul(argv[2], NULL, 10);

  // create the driver instance
  RPlidarDriver * drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);

  // try to connect to the device.
  if (IS_FAIL(drv->connect(device_path, baudrate))) {
      fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n"
          , device_path);

      return -2;
  }


  //these store the status.
  rplidar_response_device_health_t healthinfo;
  rplidar_response_device_info_t devinfo;

  //retrieving device info from successful connection.
  result = drv->getDeviceInfo(devinfo);

  if (IS_FAIL(result)) {
      if (result == RESULT_OPERATION_TIMEOUT) {
          // you can check the detailed failure reason
          fprintf(stderr, "Error, operation time out.\n");
      } else {
          fprintf(stderr, "Error, unexpected error, code: %x\n", result);
          // other unexpected result
      }
      return -3;
  }

  //check the device health to see if there are any errors.
  result = drv->getHealth(healthinfo);

  // the macro IS_OK is the preperred way to judge whether the operation is succeed.
  if (IS_OK(result)) {
      printf("RPLidar health status : ");
      switch (healthinfo.status) {
      case RPLIDAR_STATUS_OK:
          printf("OK.");
          break;
      case RPLIDAR_STATUS_WARNING:
          printf("Warning.");
          break;
      case RPLIDAR_STATUS_ERROR:
          printf("Error.");
          break;
      }
      printf(" (errorcode: %d)\n", healthinfo.error_code);

  } else {
      fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", result);
      return -4;
  }

  if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
      fprintf(stderr, "Error, rplidar internal error detected. Please reboot the device to retry.\n");
      // enable the following code if you want rplidar to be reboot by software
      // drv->reset();
      return -5;
  }

  //finally, start the motor.
  drv->startMotor();

  if (IS_FAIL(drv->startScan( 0,1 ))) // you can force rplidar to perform scan operation regardless whether the motor is rotating
  {
      fprintf(stderr, "Error, cannot start the scan operation.\n");
      return -6;
  }

  //get my first spin! Contains 8192 data points for full 360 turn.
  rplidar_response_measurement_node_hq_t nodes[8192];
  size_t   count = _countof(nodes);
  std::vector<scanDot> _scan_data;

  //the answer of oour data extraction
  u_result ans;

  printf("waiting for data...\n");

  // fetech extactly one 0-360 degrees' scan
  // thus 8192 data info is 360 degree scan.
  // WARNING: The full 360 degrees will not show if some items are too near!
  ans = drv->grabScanDataHq(nodes, count);

  for (int pos = 0; pos < (int)count; ++pos) {
      scanDot dot;
      if (!nodes[pos].dist_mm_q2) continue;

      dot.quality = nodes[pos].quality;
      dot.angle = nodes[pos].angle_z_q14 * 90.f / 16384.f;
      //https://www.seeedstudio.com/document/pdf/robopeak_2d_lidar_brief_en.pdf
      //tells you that distance is in millimeters. Is dist in mm with /4? I feel so,
      //feels more accurate with my setup.
      dot.dist = nodes[pos].dist_mm_q2 /4.0f;
      //sprintf(array_float1, "Data angle: %.2f", dot.angle);
      std::cout << "Angle:" << dot.angle << " Distance: " << dot.dist << std::endl;
      //sprintf(array_float2, "Data distance: %.2f", dot.dist);
      _scan_data.push_back(dot);
  }

  // then just stop. dont even print results.
  drv->stop();
  drv->stopMotor();
  //dont forget to delete the driver.
  RPlidarDriver::DisposeDriver(drv);

  return 0;
}
