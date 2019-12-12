#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

class Drawing : public Fl_Window {
	void draw(){
		fl_begin_points();
		//adding cushion to points to be able to see them.
		//center at 10,10
		fl_vertex(9,9);
		fl_vertex(9,10);
		fl_vertex(9,11);
		fl_vertex(10,9);
		fl_vertex(10,10);
		fl_vertex(10,11);
		fl_vertex(11,9);
		fl_vertex(11,10);
		fl_vertex(11,11);
		fl_end_points();

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
