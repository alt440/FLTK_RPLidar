#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_Widget.H>

class Drawing : public Fl_Widget {
	void draw(){
    fl_line_style(FL_SOLID);
		fl_color(FL_BLACK);
		/*fl_begin_points();
		fl_point(x()+10,y()+10);
		fl_color(FL_DARK3);
		fl_point(x()+11,x()+11);
		fl_color(FL_DARK3);
		fl_end_points();*/
		fl_begin_line();
		//start with this point.
		fl_vertex(25, 25);
		//then connect to this one.
		fl_vertex(40, 40);
		//you could add more here before ending line...
		fl_end_line();
	}
	public:
	 Drawing(int x, int y, int w, int h, const char *l = 0) : Fl_Widget(x, y, w, h, l){}

};

int main(int argc, char **argv){
  Fl_Window window(340,180);
  window.begin();

  Drawing dw(0,0,340,180);
  window.resizable(&dw);
	window.end();
	window.show(argc, argv);
	return Fl::run();
}
