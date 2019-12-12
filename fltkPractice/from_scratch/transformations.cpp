#include <math.h>
#include <vector>

#define PI 3.14159265
#define RAD 57.29578

//max distance/ angle difference to not be considered as irregularity
int threshold_distance=2;
int threshold_angle=1;
//to begin calculating orientation threshold (otherwise would have too much variance)
int threshold_data_points=5; //require 5 data points.
//to continue building the line or create a new separate one.
int threshold_orientation=10;

int lineOrientation = -1; //orientation of line
int currentLineLength = 0; //line length in progress (nb data points)

//previous distance registered
double distance_previous=-1;
//this is important in case some angle does not get registered.
//this happens when the device is too close to an object (~5cm).
double angle_previous=-1;
//stores angle and distance of each scenario.
std::vector<int> missing_info_locations_distance;
std::vector<int> missing_info_locations_angle;

//stores the length of the lines and their starting point and ending point.
//this will be useful when comparing two scans and computing the transformation.
std::vector<double> euclid_distance_lines;
std::vector<int> start_x;
std::vector<int> start_y;
std::vector<int> end_x;
std::vector<int> end_y;
//std::vector<int> orientation_lines;

//threshold for comparing line length (to know if we are talking about same line)
int threshold_length_line = 2;

//important transformation variables
double rotation_angle=-1;
int translation_x=0;
int translation_y=0;
//rotation point to rotate all our points. This will make drawing points possible.
//I prefer drawing points because it is less complex!
//TO CHANGE! ONLY FOR TESTING! Normally at 0
int rotation_point_x=500;
int rotation_point_y=500;

//outputted line with transformation
int* point_transformation = new int[2];

//max length of line from compound scan.
int max_length_compound_line = 0;

/*
Computes euclid distance.
Why is it not sent over to test_transformations???
*/
double getEuclidDistance(int previous_point_x, int previous_point_y, int new_point_x, int new_point_y){
  return double(sqrt(pow((previous_point_x-new_point_x),2)+pow((previous_point_y-new_point_y),2)));
}

int* applyTransformation(int point_x, int point_y){
  if((int)rotation_angle==-1){
    //you need to compute the transformation first!
    point_transformation[0]=-1;
    point_transformation[1]=-1;
    return point_transformation;
  }

  if(point_x==rotation_point_x&&point_y==rotation_point_y){
    point_transformation[0]=point_x;
    point_transformation[1]=point_y;
    return point_transformation;
  }


  //https://www.gamefromscratch.com/post/2012/11/24/GameDev-math-recipes-Rotating-one-point-around-another-point.aspx
  double rotatedX = cos(rotation_angle*PI/180)*(point_x-rotation_point_x)-
  sin(rotation_angle*PI/180)*(point_y-rotation_point_y)+rotation_point_x;
  double rotatedY = sin(rotation_angle*PI/180)*(point_x-rotation_point_x)+
  cos(rotation_angle*PI/180)*(point_y-rotation_point_y)+rotation_point_y;

  point_transformation[0]=(int)round(rotatedX);
  point_transformation[1]=(int)round(rotatedY);

  return point_transformation;

}

void reset_transformation(){
  rotation_angle=-1;
}

//for test purposes
void set_rotation_angle(double rotation){
  rotation_angle=rotation;
}

/*
startX_line and startY_line, endX_line and endY_line are the computed pixel positions of the
line if it were to be drawn.
Favor the line with maximum length here!
*/
void computeTransformationLine(int startX_line, int startY_line, int endX_line, int endY_line){
  //I am first going to identify to which line it corresponds
  //calculate euclid distance
  double euclid_distance = getEuclidDistance(startX_line, startY_line, endX_line, endY_line);

  int index_similar_line = 0;
  bool isFound = false;
  //now find a similar line in our 'DB'
  for (std::vector<double>::iterator it = euclid_distance_lines.begin() ; it != euclid_distance_lines.end(); ++it){
    if((*it+threshold_length_line>=euclid_distance)&& (*it-threshold_length_line<=euclid_distance)){
      //this may be our line! register index.
      //however, multiple lines may correspond... so this is maybe not the best technique
      isFound=true;
      break;
    }
    index_similar_line+=1;
  }

  if(isFound){
    //now we have index. compute transformation.
    //all values for the line we are comparing to is in a vector at index index_similar_line
    //rotation resolved by doing cosine function of vectors.
    //assuming same starting point (because of translation transformation)
    rotation_angle = acos(((startX_line-endX_line)*(end_x.at(index_similar_line)-start_x.at(index_similar_line))
    +(start_y.at(index_similar_line)-end_y.at(index_similar_line))*(startY_line-endY_line))/(euclid_distance*euclid_distance_lines.at(index_similar_line)))*RAD;

    //to resolve translation, I subtract x_point by x_point_2 and y_point by y_point_2. Translation to be accomplished
    //from line to be added POV.
    translation_x = startX_line-start_x.at(index_similar_line);
    translation_y = startY_line-start_y.at(index_similar_line);

    //our rotation point is the start point of this line. This is important to consider
    //when applying the transformation.
    //rotation_point_x = 500;
    //rotation_point_y = 500;

    //our rotation point is the start point of this line. This is important to consider
    //when applying the transformation.
    rotation_point_x = startX_line;
    rotation_point_y = startY_line;
  }


}

//for testing purposes only...
void addLineToList(int startX_line, int startY_line, int endX_line, int endY_line){
  double euclid_distance = getEuclidDistance(startX_line, startY_line, endX_line, endY_line);

  euclid_distance_lines.push_back(euclid_distance);
  start_x.push_back(startX_line);
  start_y.push_back(startY_line);
  end_x.push_back(endX_line);
  end_y.push_back(endY_line);
}

void lineControl(int center_point_x, int center_point_y, double new_distance, double new_angle){
  //this method will determine what was our previous value for our line, and
  //determine if the line has gone through corners so we separate the content
  //into multiple lines.

  //for now, this is only going to add the lines to our vectors.
  //compute line information and add to vectors.
  int adjacentVal_previous = (int)(cos(angle_previous*PI/180.0)*distance_previous);
  int oppositeVal_previous = (int)(sin(angle_previous*PI/180.0)*distance_previous);
  int adjacentVal_new = (int)(cos(new_angle*PI/180.0)*new_distance);
  int oppositeVal_new = (int)(sin(new_angle*PI/180.0)*new_distance);
  //add this to center point.
  int previous_point_x = center_point_x+adjacentVal_previous;
  int previous_point_y = center_point_y+oppositeVal_previous;
  int new_point_x = center_point_x+adjacentVal_new;
  int new_point_y = center_point_y+oppositeVal_new;

  //calculate euclid distance
  double euclid_distance = getEuclidDistance(previous_point_x, previous_point_y, new_point_x, new_point_y);

  //compute orientation. I use a (1,0) vector to find out the angle, because I
  //want the angle relative to the 0degree vector.
  /*double orientation_angle = acos((abs(previous_point_x-new_point_x)*(1)
  +abs(previous_point_y-new_point_y)*(0))/(euclid_distance*1));*/

  //save all data to tables
  start_x.push_back(previous_point_x);
  start_y.push_back(previous_point_y);
  end_x.push_back(new_point_x);
  end_y.push_back(new_point_y);
  euclid_distance_lines.push_back(euclid_distance);
  //orientation_lines.push_back((int)orientation_angle);

}

//this method inspects the line to compute its transformation.
void lineInspection(int center_point_x, int center_point_y, double new_distance, double new_angle){
  int adjacentVal_previous = (int)(cos(angle_previous*PI/180.0)*distance_previous);
  int oppositeVal_previous = (int)(sin(angle_previous*PI/180.0)*distance_previous);
  int adjacentVal_new = (int)(cos(new_angle*PI/180.0)*new_distance);
  int oppositeVal_new = (int)(sin(new_angle*PI/180.0)*new_distance);
  //add this to center point.
  int previous_point_x = center_point_x+adjacentVal_previous;
  int previous_point_y = center_point_y+oppositeVal_previous;
  int new_point_x = center_point_x+adjacentVal_new;
  int new_point_y = center_point_y+oppositeVal_new;

  computeTransformationLine(previous_point_x, previous_point_y, new_point_x, new_point_y);
}

/*
Distance and angle are from what is returned from the scan.
center_point_x and center_point_y are the information relating as to where is the center point to be able to draw
our data from that center point.
*/
bool analyzeMissingRoomInformation(double angle, double distance, int center_point_x, int center_point_y){
  //this is a method which will be used during processing of the information
  //received from the scan. It will register the locations which have not
  //been analyzed fully.
  currentLineLength+=1;
  if((int)distance_previous==-1 && (int)angle_previous==-1){
    distance_previous=distance;
    angle_previous=angle;
    //not connecting this to a line because this is (technically) our first point.
    currentLineLength=0;
    return false;
  }

  //now analyze angle. Making sure gap is small enough.
  //only going up.
  if(angle<=angle_previous+threshold_angle){
    //now analyze the distance. Also making sure the gap is small enough.
    //otherwise, would mean that there is some part that was left unexplored
    //in the room.
    //this means: if there is a distance greater than 10 cm btw previous and current
    if(!(distance<=distance_previous+threshold_distance && distance>=distance_previous-threshold_distance)){
      //if so, register values.
      missing_info_locations_angle.push_back((int)angle_previous);
      missing_info_locations_distance.push_back((int)distance_previous);

      //register line information, only if greater than our data points threshold
      if(currentLineLength>threshold_data_points)
        lineControl(center_point_x, center_point_y, distance, angle);

      //now update values.
      distance_previous=distance;
      angle_previous=angle;
      //not connecting the line, because too big of an irregularity
      currentLineLength=0;
      return false;
    }

    //if the points go into a corner, I do not want to continue connecting them!
    //to capture this scenario, I am going to compute the orientation of the line.
    /*if(currentLineLength>=threshold_data_points){

    }*/

    //now update values.
    distance_previous=distance;
    angle_previous=angle;

    //connect this point to the previous points, because it seems to follow
    //with the previous points.
    return true;
  }
  //this is when the angle is too big between the previous data point.

  //push the information into the vectors.
  missing_info_locations_angle.push_back((int)angle_previous);
  missing_info_locations_distance.push_back((int)distance_previous);

  //register line information, only if greater than our data points threshold.
  if(currentLineLength>threshold_data_points)
    lineControl(center_point_x, center_point_y, distance, angle);

  //now update values.
  distance_previous=distance;
  angle_previous=angle;

  //do not connect to the rest of the line! The gap is probably big
  currentLineLength=0;
  return false;

}


/*
Difference with the previous method: compares with the array of lines we have.
*/
bool analyzeMissingRoomInformationCompound(double angle, double distance, int center_point_x, int center_point_y){
  //this is a method which will be used during processing of the information
  //received from the scan. It will register the locations which have not
  //been analyzed fully.
  currentLineLength+=1;
  if((int)distance_previous==-1 && (int)angle_previous==-1){
    distance_previous=distance;
    angle_previous=angle;
    //not connecting this to a line because this is (technically) our first point.
    currentLineLength=0;
    return false;
  }

  //now analyze angle. Making sure gap is small enough.
  //only going up.
  if(angle<=angle_previous+threshold_angle){

    if(!(distance<=distance_previous+threshold_distance && distance>=distance_previous-threshold_distance)){
      //if so, register values.
      //missing_info_locations_angle.push_back((int)angle_previous);
      //missing_info_locations_distance.push_back((int)distance_previous);

      //register line information, only if greater than our data points threshold
      if(currentLineLength>threshold_data_points){
        //update max length.
        if(currentLineLength>max_length_compound_line){
          max_length_compound_line=currentLineLength;
          //compute transformation from this line.
          lineInspection(center_point_x, center_point_y, distance, angle);
        }

      }


      //now update values.
      distance_previous=distance;
      angle_previous=angle;
      //not connecting the line, because too big of an irregularity
      currentLineLength=0;
      return false;
    }

    //now update values.
    distance_previous=distance;
    angle_previous=angle;

    //connect this point to the previous points, because it seems to follow
    //with the previous points.
    return true;
  }

  //missing_info_locations_angle.push_back((int)angle_previous);
  //missing_info_locations_distance.push_back((int)distance_previous);

  //register line information, only if greater than our data points threshold.
  if(currentLineLength>threshold_data_points){
    if(currentLineLength>max_length_compound_line){
      max_length_compound_line=currentLineLength;
      //compute transformation from this line.
      lineInspection(center_point_x, center_point_y, distance, angle);
    }
  }

  //now update values.
  distance_previous=distance;
  angle_previous=angle;

  //do not connect to the rest of the line! The gap is probably big
  currentLineLength=0;
  return false;

}

int getTranslationY(){
  return translation_y;
}

int getTranslationX(){
  return translation_x;
}

double getRotationAngle(){
  return rotation_angle;
}
