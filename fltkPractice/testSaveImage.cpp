#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_RGB_Image.H>

#include <fstream>
#include <iostream>
#include <string>

#define XSIZE 1000
#define YSIZE 1000

//my own image.
unsigned int pixbuf[XSIZE][YSIZE][3];

void draw_line(int x1, int y1, int x2, int y2){
	//find difference between point x1 and x2
	int diffX = x2-x1;
	//same for y
	int diffY = y2-y1;
	//now those results will tell me whether the line
	//is going down/up from starting point.
	//the ratio is simply diffX/diffY. we will move X
	//before we move Y.
	std::cout<<"START LINE"<<std::endl;
	//vertical line.
	if(diffX == 0){
		if(diffY > 0){
			for(int i=0;i<diffY;i++){
				pixbuf[x1+i][y1][0] = 255;
				pixbuf[x1+i][y1][1] = 255;
				pixbuf[x1+i][y1][2] = 255;
				std::cout<<x1+i<<" "<<y1<<std::endl;
			}
		}

		if(diffY<0){
			for(int i=0;i>diffY;i--){
				pixbuf[x1+i][y1][0] = 255;
				pixbuf[x1+i][y1][1] = 255;
				pixbuf[x1+i][y1][2] = 255;
				//std::cout<<x1+i<<" "<<y1<<std::endl;
			}
		}

		std::cout<<"END VERTICAL LINE"<<std::endl;
		return;
	}

	//horizontal line
	if(diffY == 0){
		if(diffX>0){
			for(int i=0;i<diffX;i++){
				pixbuf[x1][y1+i][0] = 255;
				pixbuf[x1][y1+i][1] = 255;
				pixbuf[x1][y1+i][2] = 255;
				std::cout<<x1<<" "<<y1+i<<std::endl;
			}
		}

		if(diffX<0){
			for(int i=0;i>diffX;i--){
				pixbuf[x1][y1+i][0] = 255;
				pixbuf[x1][y1+i][1] = 255;
				pixbuf[x1][y1+i][2] = 255;
				std::cout<<x1<<" "<<y1+i<<std::endl;
			}
		}

		std::cout<<"END HORIZONTAL LINE"<<std::endl;
		return;
	}

	//now any other circumstance
	double ratioX = diffX/diffY;
	double ratioY = diffY/diffX;
	double ratio = ratioX;
	bool isX = true;
	if(ratioY > 1 || ratioY < -1){
		isX=false;
		ratio = ratioY;
	}

	//now use the smallest value to determine if
	//you need to move with that axis.
	double valX = ratioX;
	double valY = ratioY;

	for(int i=0;i<diffX;i++){
		if(valY>1 ||valY<-1){
			for(int j=0;j<(int)valY;j++){
				pixbuf[x1+i][y1+j][0]=255;
				pixbuf[x1+i][y1+j][1]=255;
				pixbuf[x1+i][y1+j][2]=255;
				std::cout<<x1+i<<" "<<y1+j<<std::endl;
			}
			y1+=(int)valY;
			valY-=(int)valY;
		}

		valY+=ratioY;
		pixbuf[x1+i][y1][0]=255;
		pixbuf[x1+i][y1][1]=255;
		pixbuf[x1+i][y1][2]=255;
		std::cout<<x1+i<<" "<<y1<<std::endl;
	}
	std::cout<<"END LINE"<<std::endl;
}

//https://stackoverflow.com/questions/36288421/c-create-png-bitmap-from-array-of-numbers
void write_bitmap(const std::string path, const int width, const int height) {
    const int pad=(4-(3*width)%4)%4, filesize=54+(3*width+pad)*height; // horizontal line must be a multiple of 4 bytes long, header is 54 bytes
    char header[54] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0, 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };
    char padding[3] = { 0,0,0 };
    for(int i=0; i<4; i++) {
        header[ 2+i] = (unsigned char)((filesize>>(8*i))&255);
        header[18+i] = (unsigned char)((width   >>(8*i))&255);
        header[22+i] = (unsigned char)((height  >>(8*i))&255);
    }
    unsigned char* img = new unsigned char[3*width*height];
    for(int i=0; i<height; i++) {
			for(int j=0;j<width;j++){
        //const unsigned int* color = pixbuf[i][j];
        img[i*width*3+j*3] = (unsigned char)(pixbuf[j][i][0]&255);
        img[i*width*3+j*3+1] = (unsigned char)(pixbuf[j][i][1]&255);
        img[i*width*3+j*3+2] = (unsigned char)(pixbuf[j][i][2]&255);
			}
		}
    std::ofstream file(path, std::ios::out|std::ios::binary);
    file.write(header, 54);
    for(int i=height-1; i>=0; i--) {
        file.write((char*)(img+3*width*i), 3*width);
        file.write(padding, pad);
    }
    file.close();
    delete[] img;
}


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

	//init pixbuf
	for(int i=0;i<XSIZE;i++){
		for(int j=0;j<YSIZE;j++){
			for(int k=0;k<3;k++){
				pixbuf[i][j][k]=0;
			}
		}
	}

	draw_line(25,25,100,25);
	draw_line(25,25,25,100);
	draw_line(25,25,70,70);
	draw_line(25,25,42,77);
	write_bitmap("test.bmp",XSIZE,YSIZE);
	return Fl::run();
}
