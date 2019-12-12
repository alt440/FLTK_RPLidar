/*
In this file I create the setup for my data from the RPLidar sensor to be shown.
*/
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#define PI 3.14159265
#include <math.h>

#include <iostream>
using namespace std;


//creating a data array.
int* x_array;
int* y_array;
int size_val=0;

class Drawing : public Fl_Window {

  //int* x_array;
  //int* y_array;
  //int size = 0;

	void draw(){
		//here, I will draw multiple lines to make this an empty circle.
    int start_x = 375;
    int start_y = 375;
    int radius = 20;
    int angle = 0;

    fl_line_style(FL_SOLID);
    fl_color(FL_DARK_BLUE);

    for(int x=0;x<20;x++){
      //calculate from center point start_x, start_y distance points from
      //that center and link them together.
      fl_begin_line();
      //hypothenus is always radius
      for(int i=0;i<120;i++){
        //find points at distance radius from center point
        //find the adjacent length value
        int adjacentVal = (int)(cos(angle*PI/180.0)*radius);
        int oppositeVal = (int)(sin(angle*PI/180.0)*radius);

        fl_vertex(start_x+adjacentVal, start_y+oppositeVal);
        angle+=3;
      }
      //to complete the circle
      fl_vertex(start_x+radius, start_y);
      fl_end_line();

      //increase radius
      radius+=20;
    }

    fl_color(FL_RED);
    //now draw the points. (set from the arrays given)
    fl_begin_points();

    for(int x=0;x<size_val;x++){
      fl_vertex(*(x_array+x), *(y_array+x));
      cout << *(x_array+x) << "," << *(y_array+x) << endl;
    }

    fl_end_points();

	}
	public:
	 Drawing(int w, int h, const char *l = 0) : Fl_Window(w, h, l){}
};

int main(int argc, char **argv){
  int array[4] = {7,8,9,10};
  x_array = array;
  y_array = array;
  size_val = 4;

	Fl_Window *window = new Drawing(750,750);
  //window->setDataArray({7,8,9,10},{7,8,9,10});
	window->end();
	window->show(argc, argv);
	return Fl::run();
}
