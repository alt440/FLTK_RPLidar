Inside the compressed file, there is two folders that caught my attention:
-Examples
-Test

These folders contain many examples of how to use the library.

I have included some of my experiments in this folder.

I used this to build the library:
./configure --disable-xft
And the command did not release any errors (before it told me that I did not have xft...)
After, you can go in the FL folder and run
make
sudo make install

And it will put the same files in your /usr/local/include folder. Which is NOT what I wanted.
I need to generate a .a file from it.

The binary files are put into /usr/lib/x86_64-linux-gnu.

And the argument -l includes only the part of the name of the library without the extension and not containing lib: liba.a becomes -la. Also, you have to tell g++ where your library is located using -L.

I had to include the library m (libm.a) because "DSO missing from command line" indicated that it required this library. Once that was done, I received SOOOOOOO much errors from my fltk library that I thought was good to go. Gotta resolve that now...

So I realized this. If you have something like:
/usr/lib/x86_64-linux-gnu/libxcb.so.1: error adding symbols: DSO missing from command line

Then it means you need to search for a file called either libxcb.so or libxcb.a, and it is most likely going to be in /usr/lib/x86_64-linux-gnu if you have it in your computer. You need to add the library for that file for your program to compile. By the way, I have always linked to .a files for now.

So I followed this URL since I saw that the errors for my fltk libraries were going to Xrender and Xft:
https://randombio.com/linuxsetup70.html

And I found out I needed to create a file called render.pc in my pkgconfig folder which is in /usr/lib/pkgconfig.

Now for some reason I followed all of the instructions of the website and my system could still not find any Xrender.h. This is why I installed apt-file, to find out where I could find such a file. Essentially apt-file allows you to know which package you need to install to get a certain file.

I found out that the package I needed to install was libxrender-dev by typing out the command :
apt-file search "X11/extensions/Xrender.h"

I also realized that I needed the freetype package because it is required for Xft library.
I installed it, but Xft did not recognize my download (bitch), so I searched for 'freetype-config' (as my error suggested) with apt-file and I found that the package libfreetype6-dev had what I wanted.

Then I needed to install 'fontconfig' because it was not found in pkg-config... Which for some reason I already had. So, I needed to install 'libfontconfig1-dev' instead.

Then I could make install Xft successfully.

Once all of this was done, I had to find my library libXft.a (which was in /usr/lib instead of /usr/lib/x86_64-linux-gnu...) and add to the makefile -lXft -lXrender (as we need both)

However, I also have to link fontconfig!
So I had to find the location of my fontconfig installation... which really was the 'libfontconfig1-dev' library instead, using:
dpkg -L libfontconfig1-dev
And so I found it in the /usr/lib/x86_64-linux-gnu directory again. Cool.

Then I had to add freetype (was in /usr/lib/x86... as libfreetype.a) with -lfreetype

Then I had to add libXfixes (already on my computer, package libxfixes-dev if you have to download) -> so -lXfixes

Then I had to add libXcursor (package libxcursor-dev, which was not installed) -> so -lXcursor
Then I had to add libXinerama (package libxinerama-dev, which was not installed) -> so -lXinerama

Then there was an issue with _dl_x86_cpu_features... which was resolved by adding my own file with that name... solution came from here:
https://stackoverflow.com/questions/56415996/linking-error-selective-static-linking-of-libm-a-in-gcc
