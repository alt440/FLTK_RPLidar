#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

class Drawing : public Fl_Window {
	void draw(){
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
		fl_line_style(FL_SOLID);
		fl_color(FL_BLACK);
	}
	public:
	 Drawing(int w, int h, const char *l = 0) : Fl_Window(w, h, l){}

};

int main(int argc, char **argv){

	Fl_Window *window = new Drawing(340,180);
	window->end();
	window->show(argc, argv);
	return Fl::run();
}
