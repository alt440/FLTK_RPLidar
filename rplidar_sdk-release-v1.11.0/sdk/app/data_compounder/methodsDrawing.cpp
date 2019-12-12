#include <fstream>
#include <iostream>
#include <string>

#define PI 3.14159265
#include <math.h>

#define XSIZE 1000
#define YSIZE 1000

//my own image.
unsigned int*** pixbuf;

void draw_line(int x1, int y1, int x2, int y2, int red, int green, int blue){
	//find difference between point x1 and x2
	int diffX = x2-x1;
	//same for y
	int diffY = y2-y1;
	//now those results will tell me whether the line
	//is going down/up from starting point.
	//the ratio is simply diffX/diffY. we will move X
	//before we move Y.
	//std::cout<<"START LINE"<<std::endl;
	//vertical line.
	if(diffX == 0){
		if(diffY > 0){
			for(int i=0;i<=diffY;i++){
				pixbuf[x1+i][y1][0] = red;
				pixbuf[x1+i][y1][1] = green;
				pixbuf[x1+i][y1][2] = blue;
				//std::cout<<x1+i<<" "<<y1<<std::endl;
			}
		}

		else if(diffY<0){
			for(int i=0;i>=diffY;i--){
				pixbuf[x1+i][y1][0] = red;
				pixbuf[x1+i][y1][1] = green;
				pixbuf[x1+i][y1][2] = blue;
				//std::cout<<x1+i<<" "<<y1<<std::endl;
			}
		}

		//std::cout<<"END VERTICAL LINE"<<std::endl;
		return;
	}

	//horizontal line
	if(diffY == 0){
		if(diffX>0){
			for(int i=0;i<=diffX;i++){
				pixbuf[x1][y1+i][0] = red;
				pixbuf[x1][y1+i][1] = green;
				pixbuf[x1][y1+i][2] = blue;
				//std::cout<<x1<<" "<<y1+i<<std::endl;
			}
		}

		else if(diffX<0){
			for(int i=0;i>=diffX;i--){
				pixbuf[x1][y1+i][0] = red;
				pixbuf[x1][y1+i][1] = green;
				pixbuf[x1][y1+i][2] = blue;
				//std::cout<<x1<<" "<<y1+i<<std::endl;
			}
		}

		//std::cout<<"END HORIZONTAL LINE"<<std::endl;
		return;
	}

	//now any other circumstance
	double ratioX = diffX/(diffY*1.0);
	double ratioY = diffY/(diffX*1.0);

	//now use the smallest value to determine if
	//you need to move with that axis.
	double valX = ratioX;
	double valY = ratioY;
	if(diffX>0){
		for(int i=0;i<=diffX;i++){
			if(valY>1){
				for(int j=0;j<(int)valY;j++){
					pixbuf[x1+i][y1+j][0]=red;
					pixbuf[x1+i][y1+j][1]=green;
					pixbuf[x1+i][y1+j][2]=blue;
					//std::cout<<x1+i<<" "<<y1+j<<std::endl;
				}
				y1+=(int)valY;
				valY-=(int)valY;
			}
			else if(valY<-1){
				for(int j=0;j>(int)valY;j--){
					pixbuf[x1+i][y1+j][0]=red;
					pixbuf[x1+i][y1+j][1]=green;
					pixbuf[x1+i][y1+j][2]=blue;
					//std::cout<<x1+i<<" "<<y1+j<<std::endl;
				}
				y1+=(int)valY;
				valY-=(int)valY;
			}

			valY+=ratioY;

			pixbuf[x1+i][y1][0]=red;
			pixbuf[x1+i][y1][1]=green;
			pixbuf[x1+i][y1][2]=blue;
			//std::cout<<x1+i<<" "<<y1<<std::endl;
		}
	}

	else if(diffX<0){
		for(int i=0;i>=diffX;i--){
			if(valY>1 && diffY>0){
				for(int j=0;j<(int)valY;j++){
					pixbuf[x1+i][y1+j][0]=red;
					pixbuf[x1+i][y1+j][1]=green;
					pixbuf[x1+i][y1+j][2]=blue;
					//std::cout<<x1+i<<" "<<y1+j<<std::endl;
				}
				y1+=(int)valY;
				valY-=(int)valY;
			}
			else if(valY>1 && diffY<0){
				for(int j=0;j>(int)valY*-1;j--){
					pixbuf[x1+i][y1+j][0]=red;
					pixbuf[x1+i][y1+j][1]=green;
					pixbuf[x1+i][y1+j][2]=blue;
					//std::cout<<x1+i<<" "<<y1+j<<std::endl;
				}
				y1-=(int)valY;
				valY-=(int)valY;
			}
			else if(valY<-1 && diffY>0){
				for(int j=0;j<(int)valY*-1;j++){
					pixbuf[x1+i][y1+j][0]=red;
					pixbuf[x1+i][y1+j][1]=green;
					pixbuf[x1+i][y1+j][2]=blue;
					//std::cout<<x1+i<<" "<<y1+j<<std::endl;
				}
				y1+=(int)valY*-1;
				valY-=(int)valY;
			}
			else if(valY<-1 && diffY<0){
				for(int j=0;j>(int)valY;j--){
					pixbuf[x1+i][y1+j][0]=red;
					pixbuf[x1+i][y1+j][1]=green;
					pixbuf[x1+i][y1+j][2]=blue;
					//std::cout<<x1+i<<" "<<y1+j<<std::endl;
				}
				y1-=(int)valY;
				valY-=(int)valY;
			}

			valY+=ratioY;

			pixbuf[x1+i][y1][0]=red;
			pixbuf[x1+i][y1][1]=green;
			pixbuf[x1+i][y1][2]=blue;
			//std::cout<<x1+i<<" "<<y1<<std::endl;
		}
	}

	//std::cout<<"END LINE"<<std::endl;
}

void draw_circle(int x1, int y1, int radius, int red, int green, int blue){
	//here, I will draw multiple lines to make this an empty circle.
	//many if conditions have been added for each beginning/end of quadrant
	//as there were some irregularities for smaller circles (vertical, horizontal
	//lines popping out of no where)
	int angle = 0;
	int previous_start_X=0;
	int previous_start_Y=0;
	int previous_start_X_177=0;
	int previous_start_Y_177=0;
	int previous_start_X_183=0;
	int previous_start_Y_183=0;
	//calculate from center point start_x, start_y distance points from
	//that center and link them together.
	//first point does not make a line.
	//hypothenus is always radius
	for(int i=0;i<=120;i++){
		//find points at distance radius from center point
		//find the adjacent length value
		int adjacentVal = (int)(cos(angle*PI/180.0)*radius);
		int oppositeVal = (int)(sin(angle*PI/180.0)*radius);

		if(i==0){
			previous_start_X=x1+adjacentVal;
			previous_start_Y=y1+oppositeVal;
		}

		if(angle<90){
			draw_line(x1+adjacentVal, y1+oppositeVal, previous_start_X, previous_start_Y, red,green,blue);

			previous_start_X=x1+adjacentVal;
			previous_start_Y=y1+oppositeVal;
		}

		else if(angle<180){
			draw_line(x1+adjacentVal, y1+oppositeVal, previous_start_X, previous_start_Y, red,green,blue);

			previous_start_X=x1+adjacentVal;
			previous_start_Y=y1+oppositeVal;
		}

		else if(angle<270){
			//to remove the part not covered in the circle.
			if(angle==183|| angle==186){
				draw_line(previous_start_X, previous_start_Y, x1+adjacentVal, y1-oppositeVal, red,green,blue);
			}
			draw_line(previous_start_X, previous_start_Y, x1+adjacentVal, y1+oppositeVal, red,green,blue);

			previous_start_X=x1+adjacentVal;
			previous_start_Y=y1+oppositeVal;
		}

		else if(angle<=360){
			draw_line(previous_start_X, previous_start_Y, x1+adjacentVal, y1+oppositeVal, red,green,blue);

			previous_start_X=x1+adjacentVal;
			previous_start_Y=y1+oppositeVal;
		}

		angle+=3;
	}
	//to complete the circle
	//this is what draws the small white line that added to the part that was not
	//completed of the circle.
	draw_line(previous_start_X, previous_start_Y, x1+radius, y1, red, green, blue);

}

void draw_point(int x1, int y1, int red, int green, int blue){
	pixbuf[x1][y1][0]=red;
	pixbuf[x1][y1][1]=green;
	pixbuf[x1][y1][2]=blue;
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

void read_bitmap(const std::string path, const int width, const int height) {
    const int pad=(4-(3*width)%4)%4, filesize=54+(3*width+pad)*height; // horizontal line must be a multiple of 4 bytes long, header is 54 bytes
    char header[54] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0, 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };
    char padding[3] = { 0,0,0 };
    for(int i=0; i<4; i++) {
        header[ 2+i] = (unsigned char)((filesize>>(8*i))&255);
        header[18+i] = (unsigned char)((width   >>(8*i))&255);
        header[22+i] = (unsigned char)((height  >>(8*i))&255);
    }
    unsigned char* img = new unsigned char[3*width*height];

    std::ifstream file(path, std::ios::out|std::ios::binary);
    file.read(header, 54);
    for(int i=height-1; i>=0; i--) {
        file.read((char*)(img+3*width*i), 3*width);
        file.read(padding, pad);
    }
    file.close();

		for(int i=0;i<height;i++){
			for(int j=0;j<width;j++){
				for(int k=0;k<3;k++){
					pixbuf[j][i][k] = (int)img[i*width*3+j*3+k];
				}
			}
		}

		delete[] img;
}

//initialize pixbuf before use
void init_pixbuf(){
	//init pixbuf
	pixbuf = new unsigned int**[XSIZE];
	for(int i=0;i<XSIZE;i++){
		pixbuf[i] = new unsigned int*[YSIZE];
		for(int j=0;j<YSIZE;j++){
			pixbuf[i][j] = new unsigned int[3];
			for(int k=0;k<3;k++){
				pixbuf[i][j][k]=0;
			}
		}
	}
}

unsigned int*** get_pixbuf(){
	return pixbuf;
}

void delete_pixbuf(){
	for(int i=0;i<XSIZE;i++){
		for(int j=0;j<YSIZE;j++){
			delete[] pixbuf[i][j];
		}
		delete[] pixbuf[i];
	}

	delete[] pixbuf;

}

//example of how to use the methods
/*int main(int argc, char **argv){

	init_pixbuf();

	draw_line(25,25,100,25, 255,255,255);
	draw_line(25,25,25,100, 255,255,255);
	draw_line(25,25,70,70, 255,255,255);
	draw_line(500,500,470,470, 255,255,255);
	draw_line(500,500,550,470, 255,255,255);
	draw_line(500,500,410,550, 255,255,255);
	draw_line(25,25,42,77, 255,255,255);
	draw_circle(150,150,10, 255,255,255);
	draw_circle(150,150,30, 255,255,255);
	draw_circle(150,150,100, 255,255,255);
	write_bitmap("test.bmp",XSIZE,YSIZE);
	return 0;
}*/
