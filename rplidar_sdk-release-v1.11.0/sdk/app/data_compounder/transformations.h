
void computeTransformationLine(int startX_line, int startY_line, int endX_line, int endY_line);

bool analyzeMissingRoomInformation(double angle, double distance, int center_point_x, int center_point_y);

bool analyzeMissingRoomInformationCompound(double angle, double distance, int center_point_x, int center_point_y);

void lineControl(int center_point_x, int center_point_y, double new_distance, double new_angle);

int getEuclidDistance(int previous_point_x, int previous_point_y, int new_point_x, int new_point_y);

int* applyTransformation(int point_x, int point_y);

void reset_transformation();

void lineInspection(int center_point_x, int center_point_y, double new_distance, double new_angle);

int getTranslationX();

int getTranslationY();

double getRotationAngle();
