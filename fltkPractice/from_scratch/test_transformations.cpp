#include "transformations.h"
#include "methodsDrawing.h"
#include <iostream>

#define XSIZE 1000
#define YSIZE 1000

unsigned int*** pixbuf_main;

int testEuclid(){
  //test euclid distance function.
  int length = (int)getEuclidDistance(500,500,520,500);
  //std::cout<<"Euclid distance: "<<length<<std::endl;
  if(length!=20){
    return -1;
  }
  length = (int)getEuclidDistance(520,520,500,500);
  if(length!=28){
    return -1;
  }
  return 1;
}

void delete_pixbuf_main(){
	for(int i=0;i<XSIZE;i++){
		for(int j=0;j<YSIZE;j++){
			delete[] pixbuf_main[i][j];
		}
		delete[] pixbuf_main[i];
	}

	delete[] pixbuf_main;

}

int main(){

  if(testEuclid()!=1){
    std::cout<<"There was an error in the Euclid tests"<<std::endl;
  }

  //test line transformation function.
  //init_pixbuf();

  //draw two lines.
  //draw_line(500,500,520,520, 255,255,255);
  /*addLineToList(500,500,520,520);
  //draw_line(500,500,480,480, 255,0,0);
  computeTransformationLine(500,500,480,480);

  std::cout<<"Translation: ("<<getTranslationX()<<","<<getTranslationY()<<") Rotation: "<<getRotationAngle()<<std::endl;

  computeTransformationLine(500,500,528,500);
  //should have a rotation of 45 degrees
  std::cout<<"Translation: ("<<getTranslationX()<<","<<getTranslationY()<<") Rotation: "<<getRotationAngle()<<std::endl;

  computeTransformationLine(500,500,472,500);
  //should have a rotation of -45 degrees or 315 degrees
  std::cout<<"Translation: ("<<getTranslationX()<<","<<getTranslationY()<<") Rotation: "<<getRotationAngle()<<std::endl;

  computeTransformationLine(500,500,480,520);
  //should have a rotation of 90 degrees
  std::cout<<"Translation: ("<<getTranslationX()<<","<<getTranslationY()<<") Rotation: "<<getRotationAngle()<<std::endl;

  computeTransformationLine(500,340,480,360);
  //should have a rotation of 90 degrees
  std::cout<<"Translation: ("<<getTranslationX()<<","<<getTranslationY()<<") Rotation: "<<getRotationAngle()<<std::endl;

  computeTransformationLine(230,340,210,360);
  //should have a rotation of 90 degrees
  std::cout<<"Translation: ("<<getTranslationX()<<","<<getTranslationY()<<") Rotation: "<<getRotationAngle()<<std::endl;*/

  init_pixbuf();

  //addLineToList(500,500,500,520);
  //90 degrees
  //computeTransformationLine(500,500,520,500);
  //180 degrees
  //computeTransformationLine(500,500,500,480);
  //drawing square
  draw_line(500,500,520,500, 255,255,255);
  draw_line(520,500,520,520, 255,255,255);
  draw_line(520,520,500,520, 255,255,255);
  draw_line(500,520,500,500, 255,255,255);

  std::cout<<"Translation: ("<<getTranslationX()<<","<<getTranslationY()<<") Rotation: "<<getRotationAngle()<<std::endl;

  //manually apply rotation
  set_rotation_angle(90);

  //now apply the transformation on some point.
  /*int* coors = applyTransformation(10,0);
  std::cout<<coors[0]<<" "<<coors[1]<<std::endl;*/
  //cant do it this way: returns a reference to the array, but all this changes when
  //I run another applyTransformation
  //so I need deep copy!
  //this projection should give me the same thing (500,500) since it is the rotation point.
  int* transformation = applyTransformation(500,500);
  int* coors1 = new int[2];
  coors1[0]=transformation[0];
  coors1[1]=transformation[1];
  //this projection should give me (500,520)
  transformation = applyTransformation(520,500);
  int* coors2 = new int[2];
  coors2[0]=transformation[0];
  coors2[1]=transformation[1];
  //this projection should give me (480,500)
  transformation = applyTransformation(500,520);
  int* coors3 = new int[2];
  coors3[0]=transformation[0];
  coors3[1]=transformation[1];
  //this projection should give me (480,520)
  transformation = applyTransformation(520,520);
  int* coors4 = new int[2];
  coors4[0]=transformation[0];
  coors4[1]=transformation[1];

  /*draw_point(coors1[0], coors1[1], 0,0,255);
  draw_point(coors2[0], coors2[1], 0,0,255);
  draw_point(coors3[0], coors3[1], 0,0,255);
  draw_point(coors4[0], coors4[1], 0,0,255);*/

  draw_line(coors1[0],coors1[1],coors2[0],coors2[1], 0,0,255);
  draw_line(coors2[0],coors2[1],coors4[0],coors4[1], 0,0,255);
  draw_line(coors4[0],coors4[1],coors3[0],coors3[1], 0,0,255);
  draw_line(coors3[0],coors3[1],coors1[0],coors1[1], 0,0,255);

  /* This should be the final result, if 90 degree transformation.
  draw_line(500,500,500,520, 0,0,255);
  draw_line(500,520,480,520, 0,0,255);
  draw_line(480,520,480,500, 0,0,255);
  draw_line(480,500,500,500, 0,0,255);
  */

  write_bitmap("test_transformations.bmp",XSIZE,YSIZE);

  delete_pixbuf();
  delete[] coors1;
  delete[] coors2;
  delete[] coors3;
  delete[] coors4;

  //now do a transformation on this image (of x degrees)
  set_rotation_angle(90);
  init_pixbuf();
  read_bitmap("GoodScan2.bmp",XSIZE,YSIZE);

  //to test transformation, I need to create my own and get the current one.
  pixbuf_main = new unsigned int**[XSIZE];
	for(int i=0;i<XSIZE;i++){
		pixbuf_main[i] = new unsigned int*[YSIZE];
		for(int j=0;j<YSIZE;j++){
			pixbuf_main[i][j] = new unsigned int[3];
			for(int k=0;k<3;k++){
				pixbuf_main[i][j][k]=0;
			}
		}
	}
  pixbuf_main = get_pixbuf();

  //now for every point that has a value 255, I will project it.
  for(int i=0;i<XSIZE;i++){
    for(int j=0;j<YSIZE;j++){
      if(pixbuf_main[i][j][0]==255){
        int* transformation = applyTransformation(i,j);
        draw_point(transformation[0], transformation[1], 0,0,255);
      }
    }
  }

  //then save this as 90 degree transformation img
  write_bitmap("ninetyDegreeTransform.bmp",XSIZE,YSIZE);

  //delete_pixbuf_main();
  delete_pixbuf();

  return 0;
}
