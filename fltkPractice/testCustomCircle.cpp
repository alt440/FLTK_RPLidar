//I will create my own circle in here.
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#define PI 3.14159265
#include <math.h>

class Drawing : public Fl_Window {
	void draw(){
		//here, I will draw multiple lines to make this an empty circle.
    int start_x = 350;
    int start_y = 350;
    int radius = 40;
    int angle = 0;
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


    fl_line_style(FL_SOLID);
    fl_color(FL_BLACK);
	}
	public:
	 Drawing(int w, int h, const char *l = 0) : Fl_Window(w, h, l){}

};

int main(int argc, char **argv){

	Fl_Window *window = new Drawing(700,700);
	window->end();
	window->show(argc, argv);
	return Fl::run();
}
