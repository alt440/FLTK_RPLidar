//this code will display the data from RPLidar
/*
In this file I create the setup for my data from the RPLidar sensor to be shown.
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//to draw on the bmp file
#include "methodsDrawing.h"
//to compound multiple scans
#include "transformations.h"

#define PI 3.14159265
#include <math.h>

#include "rplidar.h" //RPLIDAR standard sdk, all-in-one header

//include the _dl_x86_cpu_features, which did not allow me to 'make' this
//file
#include "_dl_x86_cpu_features.c"

#ifndef XSIZE
#define XSIZE 1000
#define YSIZE 1000
#endif

//a simple count...
#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

//from scanView.h
struct scanDot {
    _u8   quality;
    float angle;
    float dist;
};

//creating my data arrays.
std::vector<scanDot> vectorDots;
int size_val=0;

//identifies if the scan is compound or not.
int scan_nb=0;

/*
  start_x is the coordinate x from which we want to start writing data. Default: 500
  start_y is the coordinate y from which we want to start writing data. Default: 500
*/
void drawDataPoints(int start_x, int start_y){

  //then I set all my points.
  for(int x=0;x<size_val;x++){
    //distance of scan dot is at vectorDots.at(x).dist
    //angle of scan dot is at vectorDots.at(x).angle
    //remember distance is in millimeters... so you need to convert it first.
    //since 1 pixel is 5 cm, and I measure in mm, I divide by 50. 1 pixel is 5 cm
    //because I have set lines to show meters.
    double distance = (vectorDots.at(x).dist/50);
    int adjacentVal = (int)(cos(vectorDots.at(x).angle*PI/180.0)*distance);
    int oppositeVal = (int)(sin(vectorDots.at(x).angle*PI/180.0)*distance);

    /*bool notAddLine = analyzeMissingRoomInformation(vectorDots.at(x).angle, distance, start_x, start_y);
    if(!notAddLine){
      //then add line.
      draw_line(start_x+adjacentVal_registered, start_y+oppositeVal_registered, start_x+adjacentVal, start_y+oppositeVal, 255,255,255);
      adjacentVal_registered=adjacentVal;
      oppositeVal_registered=oppositeVal;
    }*/
    draw_point(start_x+adjacentVal, start_y+oppositeVal, 255,255,255);
  }
}

/*
  start_x is the coordinate x from which we want to start writing data. Default: 500
  start_y is the coordinate y from which we want to start writing data. Default: 500
*/
void drawData(int start_x, int start_y){

  //registered information for starting point line.
  int adjacentVal_registered = (int)(cos(vectorDots.at(0).angle*PI/180.0)*(vectorDots.at(0).dist/50));
  int oppositeVal_registered = (int)(sin(vectorDots.at(0).angle*PI/180.0)*(vectorDots.at(0).dist/50));

  //previous values, so that I do not add to the line some outlier
  int adjacentVal_previous = (int)(cos(vectorDots.at(0).angle*PI/180.0)*(vectorDots.at(0).dist/50));
  int oppositeVal_previous = (int)(sin(vectorDots.at(0).angle*PI/180.0)*(vectorDots.at(0).dist/50));
  //then I set all my points.
  for(int x=0;x<size_val;x++){
    //distance of scan dot is at vectorDots.at(x).dist
    //angle of scan dot is at vectorDots.at(x).angle
    //remember distance is in millimeters... so you need to convert it first.
    //since 1 pixel is 5 cm, and I measure in mm, I divide by 50. 1 pixel is 5 cm
    //because I have set lines to show meters.
    double distance = (vectorDots.at(x).dist/50);
    int adjacentVal = (int)(cos(vectorDots.at(x).angle*PI/180.0)*distance);
    int oppositeVal = (int)(sin(vectorDots.at(x).angle*PI/180.0)*distance);

    bool notAddLine = analyzeMissingRoomInformation(vectorDots.at(x).angle, distance, start_x, start_y);
    if(!notAddLine){
      //then add line.
      draw_line(start_x+adjacentVal_previous, start_y+oppositeVal_previous, start_x+adjacentVal_registered, start_y+oppositeVal_registered, 255,255,255);
      adjacentVal_registered=adjacentVal;
      oppositeVal_registered=oppositeVal;
    }

    //update previous values
    adjacentVal_previous=adjacentVal;
    oppositeVal_previous=oppositeVal;
    //draw_point(start_x+adjacentVal, start_y+oppositeVal, 255,255,255);
  }
}

void drawDataCompound(bool drawLines){
  //I need to pass through the points, and notice possible lines but not draw them.
  //I take notice of lines with maximum length, because they are the most determinant.

  //now using 500,500, but this is subject to change
  int start_x=500;
  int start_y=500;
  //registered information for starting point line.
  int adjacentVal_registered = (int)(cos(vectorDots.at(0).angle*PI/180.0)*(vectorDots.at(0).dist/50));
  int oppositeVal_registered = (int)(sin(vectorDots.at(0).angle*PI/180.0)*(vectorDots.at(0).dist/50));

  //previous values, so that I do not add to the line some outlier
  int adjacentVal_previous = (int)(cos(vectorDots.at(0).angle*PI/180.0)*(vectorDots.at(0).dist/50));
  int oppositeVal_previous = (int)(sin(vectorDots.at(0).angle*PI/180.0)*(vectorDots.at(0).dist/50));
  //then I set all my points.
  for(int x=0;x<size_val;x++){
    //distance of scan dot is at vectorDots.at(x).dist
    //angle of scan dot is at vectorDots.at(x).angle
    //remember distance is in millimeters... so you need to convert it first.
    //since 1 pixel is 5 cm, and I measure in mm, I divide by 50. 1 pixel is 5 cm
    //because I have set lines to show meters.
    double distance = (vectorDots.at(x).dist/50);
    int adjacentVal = (int)(cos(vectorDots.at(x).angle*PI/180.0)*distance);
    int oppositeVal = (int)(sin(vectorDots.at(x).angle*PI/180.0)*distance);

    bool notAddLine = analyzeMissingRoomInformationCompound(vectorDots.at(x).angle, distance, start_x, start_y);
    if(!notAddLine){
      //print out predicted translation/rotation angle
      std::cout<<"Translation: "<<getTranslationX()<<", "<<getTranslationY()<<" Rotation: "<<getRotationAngle()<<" Line length: "<<
      getEuclidDistance(start_x+adjacentVal_previous, start_y+oppositeVal_previous, start_x+adjacentVal_registered, start_y+oppositeVal_registered)<<std::endl;
      //blue lines --> before transform.
      if(drawLines){
        draw_line(start_x+adjacentVal_previous, start_y+oppositeVal_previous, start_x+adjacentVal_registered, start_y+oppositeVal_registered, 255,0,0);
      }

      adjacentVal_registered=adjacentVal;
      oppositeVal_registered=oppositeVal;
    }

    if(!drawLines){
      draw_point(start_x+adjacentVal, start_y+oppositeVal, 255,0,0);
    }


    //update previous values
    adjacentVal_previous=adjacentVal;
    oppositeVal_previous=oppositeVal;
    //draw_point(start_x+adjacentVal, start_y+oppositeVal, 255,255,255);
  }

  std::cout<<"Applied transformation: ("<<getTranslationX()<<", "<<getTranslationY()<<") and rotation: "<<getRotationAngle()<<std::endl;

  int* point_coor = new int[2];

  bool isMessagePrinted = false;
  //using transformation suggested by longest line.
  for(int x=0;x<size_val;x++){
    //distance of scan dot is at vectorDots.at(x).dist
    //angle of scan dot is at vectorDots.at(x).angle
    //remember distance is in millimeters... so you need to convert it first.
    //since 1 pixel is 5 cm, and I measure in mm, I divide by 50. 1 pixel is 5 cm
    //because I have set lines to show meters.
    double distance = (vectorDots.at(x).dist/50);
    int adjacentVal = (int)(cos(vectorDots.at(x).angle*PI/180.0)*distance);
    int oppositeVal = (int)(sin(vectorDots.at(x).angle*PI/180.0)*distance);

    point_coor = applyTransformation(start_x+adjacentVal, start_y+oppositeVal);
    if(point_coor[0]<0 || point_coor[1]<0){
      //is it because there was no transformation?
      if(!isMessagePrinted){
        std::cout<<"Attained negative coordinates. Is it because there is no transformation?"<<std::endl;
        isMessagePrinted=true;
      }

      draw_point(start_x+adjacentVal, start_y+oppositeVal, 0,0,255);
      continue;
    }
    draw_point(point_coor[0], point_coor[1], 0,0,255);

    /*if(!notAddLine){
      int* init_point;
      int* final_point;
      init_point = applyTransformation(start_x+adjacentVal_previous, start_y+oppositeVal_previous);
      final_point = applyTransformation(start_x+adjacentVal_registered, start_y+oppositeVal_registered);
      std::cout<<"Coors init: "<<init_point[0]<<","<<init_point[1]<<" to "<<final_point[0]<<","<<final_point[1]<<std::endl;
      draw_line(init_point[0], init_point[1], final_point[0], final_point[1], 0,0,255);
      adjacentVal_registered=adjacentVal;
      oppositeVal_registered=oppositeVal;
    }*/

    //update previous values
    //adjacentVal_previous=adjacentVal;
    //oppositeVal_previous=oppositeVal;
    //draw_point(start_x+adjacentVal, start_y+oppositeVal, 255,255,255);
  }
}

//to recognize the RPLIDAR driver...
using namespace rp::standalone::rplidar;

int main(int argc, char **argv){

  //initialize the array
  init_pixbuf();

  //I set my circles first. (before receiving any data...)
  //the center of bmp image is (500,500)
  for(int i=0;i<20;i++){
    draw_circle(500,500,i*20+20, 0,128,0);
  }

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

  ////////////////////////////////////////////////////////////////////////////
  //                  On to the good stuff: Collecting Data                 //
  ////////////////////////////////////////////////////////////////////////////

  char* input = new char[80];
  //get my first spin! Contains 8192 data points for full 360 turn.
  rplidar_response_measurement_node_hq_t nodes[8192];
  size_t   count = _countof(nodes);
  std::vector<scanDot> _scan_data;

  //the answer of oour data extraction
  u_result ans;

  while(1){

    std::cin>>input;
    if(input[0]=='r'){
      //clear our scan data first, as we will be compounding scans.
      _scan_data.clear();
      //finally, start the motor.
      drv->startMotor();

      if (IS_FAIL(drv->startScan( 0,1 ))) // you can force rplidar to perform scan operation regardless whether the motor is rotating
      {
          fprintf(stderr, "Error, cannot start the scan operation.\n");
          return -6;
      }

      //printf("waiting for data...\n");
      std::cout<<"Waiting for data..."<<std::endl;
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
          //std::cout << "Angle:" << dot.angle << " Distance: " << dot.dist << std::endl;
          //sprintf(array_float2, "Data distance: %.2f", dot.dist);
          _scan_data.push_back(dot);
      }

      // then just stop. dont even print results.
      drv->stop();
      drv->stopMotor();



      //set _scan_data as my vector data.
      vectorDots = _scan_data;
      //NOTE : This limits size of vector to MAX_INT
      size_val = static_cast<int>(_scan_data.size());

      //find the starting point (in cm)
      //use the input to tell.
      //draw the data we got from scan.
      if(scan_nb==0){
        init_pixbuf();
        for(int i=0;i<20;i++){
          draw_circle(500,500,i*20+20, 0,128,0);
        }
        drawDataPoints(500,500);
        write_bitmap("RPLidarPoints.bmp", XSIZE,YSIZE);

        delete_pixbuf();
        init_pixbuf();
        for(int i=0;i<20;i++){
          draw_circle(500,500,i*20+20, 0,128,0);
        }
        drawData(500, 500);
        write_bitmap("RPLidar.bmp",XSIZE,YSIZE);
        delete_pixbuf();
        scan_nb++;
      }
      else{
        init_pixbuf();
        //get previous data.
        read_bitmap("RPLidarPoints.bmp",XSIZE,YSIZE);
        //to not draw lines
        drawDataCompound(false);
        write_bitmap("RPLidarCompound.bmp", XSIZE,YSIZE);
        delete_pixbuf();

        init_pixbuf();
        //get previous data lines.
        read_bitmap("RPLidar.bmp",XSIZE,YSIZE);
        //to draw lines
        drawDataCompound(true);
        write_bitmap("RPLidarCompoundLines.bmp", XSIZE,YSIZE);
        delete_pixbuf();
      }

      //reinitialize pixbuf.

    }

    else if(input[0]=='s'){
      //dont forget to delete the driver.
      RPlidarDriver::DisposeDriver(drv);
      delete_pixbuf();
      break;
    }

    std::cin.clear();
  }

	return 0;
}
