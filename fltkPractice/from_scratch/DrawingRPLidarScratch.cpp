#include "methodsDrawing.h"

#ifndef XSIZE
#define XSIZE 1000
#define YSIZE 1000
#endif

unsigned int*** pixbuf_main;

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
  init_pixbuf();

  //draw the circles to indicate the length to a certain destination of the room
  for(int i=0;i<20;i++){
    draw_circle(500,500,i*20+20, 0,255,0);
  }

  draw_line(500,500,520,530, 255,255,255);
	draw_line(520,530,500,500, 255,255,255);

	draw_line(500,500,520,460, 255,255,255);
	draw_line(520,460,500,500, 255,255,255);

	draw_line(500,500,480,450, 255,255,255);
	draw_line(480,450,500,500, 255,255,255);

	draw_line(500,500,480,540, 255,255,255);
	draw_line(480,540,500,500, 255,255,255);

  write_bitmap("RPLidar.bmp",XSIZE,YSIZE);

  //to test getPixBuf
  /*pixbuf_main = new unsigned int**[XSIZE];
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

  //now deallocate the array.
  delete_pixbuf_main();*/
  //to test read_bitmap
  /*read_bitmap("RPLidar.bmp",XSIZE,YSIZE);

  write_bitmap("RPLidarCopy.bmp",XSIZE,YSIZE);*/

  return 0;
}
