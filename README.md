# FLTK_RPLidar
Project from RPLidar embedded device.
This project is composed of two different parts: FLTK practice, and RPLidar SDK.

# RPLidar

The RPLidar SDK part contains as well some of the scripts I made. The scripts that I designed are located in the directory ./sdk/app. A folder was made for every different application. To add another script, add another folder and put your scripts in it. Then, modify the Makefile in the folder ./sdk/app to add your folder to be 'make'd, and add a Makefile in your app's folder (which can be copied from another folder in the app directory). To figure out what they are doing in their 'make's, you can always do

Example:
-Create directory a in ./sdk/app.

-Add folder a to Makefile in ./sdk/app.

-Add makefile in folder a by copying it from another folder, say data_compounder.

-Change the name  in the Makefile for variable CXXSRC by your own main script (if b.cpp contains the main function, put b.cpp)

-Once these steps are done, 'make' the project from a higher level (you can make it from the app directory, so 'cd ./sdk/app' then 'make').

-The compiled files will appear in the folder ./output/Linux/Release

I also added the manual for RPLidar as a pdf file in this folder.

# FLTK
This project only required the fltk package, which can be downloaded using:

    sudo apt-get install fltk1.3-dev

This is the only thing required for being able to run the files in this folder.

# Apt-file
This component is very useful when searching for packages containing specific files. It became very useful in my project. You can download it using:

    sudo apt-get install apt-file

# Make
You can see the make commands by running:

    make -n

# Drawing 
I have included with my application data-compounder some very neat functions to draw lines, circles, and points on an image. It is found in the RPLidar source files, in ./sdk/app/data_compounder/, in the files drawingMethods.cpp and drawingMethods.h.

# '_dl_x86_cpu_features.c'
This file was made as a workaround to the issue where I could not import the math library because I was using the sin and cos functions. This file can be found in some folders under the ./sdk/app path.
