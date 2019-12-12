/*
finding unexplored locations: to do so, I must look at the locations of my scan
where there seems to be a big drop of distance suddenly (set your threshold...).

This is assuming that the RPLIDAR is used horizontally, not bent sideways-vertically.
This program will tell the robot how to get to the unexplored location by looking
at what is in its path up to the big drop of distance. This will involve a U-turn
for sure.
*/
