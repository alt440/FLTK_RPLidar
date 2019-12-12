#include <string>

void draw_line(int x1, int y1, int x2, int y2, int red, int green, int blue);

void draw_circle(int x1, int y1, int radius, int red, int green, int blue);

void draw_point(int x1, int y1, int red, int green, int blue);

void write_bitmap(const std::string path, const int width, const int height);

void read_bitmap(const std::string path, const int width, const int height);

void init_pixbuf();

unsigned int*** get_pixbuf();

void delete_pixbuf();
