#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>

class Drawing : public Fl_Window {
	void draw(){
		fl_begin_complex_polygon();
		//x, y, radius, start angle, end angle,rotate
    		//fl_arc(60,60,3,0,360,0);
		//fl_gap();
    		fl_arc(60,60,40,0,-360);
		fl_end_complex_polygon();
    		//fl_vertex(w()-50+move, move+25);
    		//fl_vertex(move+25, h()/2-10+move);
    		// you must reset the line type when done:
    		fl_line_style(FL_SOLID);
    		fl_color(FL_BLACK);
				//fl_fill_color(FL_WHITE);
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
