Parameters
--------------------------
There is one file that is read by the program. It has very strict options for each 
line in the file. It includes the information for each ROI, parameters for the 
various functions for each ROI, and the images the appropriate functions should be run on. 
The first line is the number of ROIs (n) that the program will consider (maximum of 3). 
The following n lines will be information given for the parameters for each ROI for each function. 
Each of these n lines will contain the following information in the following order: x, y, sx, sy, 
T1, T2, cR, cG, cB, TC, DC, WS. They will not contain commans. After those n lines, the rest of the 
lines in the file should be images to run the specified functions on and how to name the output image. 
These lines will be in this form: input_image output_image function. Where the function can be 
double_thresh (for Double Thresholding), color_binar (for Color Binarization+), 2d_smooth (for 2D 
Uniform Smoothing), and 1d_smooth (for 1D Uniform Smoothing). A side-note is that the parameters file 
must end with an empty newline. Another note is that the parameters file should be in the following path: 
/Project1/project/bin/. The following image is an example of a properly constructed parameters file 
called parameters.txt:
2
0 0 100 300 100 150 100 150 100 80 20 5
320 80 150 150 50 150 210 200 60 100 20 5
park.pgm park_dt.pgm double_thresh
lena.pgm lena_dt.pgm double_thresh
park.pgm park_dt.pgm double_thresh
ball.ppm ball_cb.ppm color_binar
wine.ppm wine_cb.ppm color_binar
lena.ppm lena_cb.ppm color_binar
park.pgm park_2d.pgm 2d_smooth
lena.pgm lena_2d.pgm 2d_smooth
floor.pgm floor_2d.pgm 2d_smooth
lena.pgm lena_1d.pgm 1d_smooth
camera.pgm camera_1d.pgm 1d_smooth



Compilation and Execution
--------------------------
In order to compile this program, make needs to be run twice. Once inside the Project1 directory, 
run make inside of /Project1/iptools to compile both image.cpp and utility.cpp. Then travel to 
/Project1/project and run make to compile iptool.cpp. This will create the executable in 
/Project1/project/bin of the name iptool. To run the program, travel to /Project1/project/bin/ 
and execute ./iptool <file_name> where <file_name> is the name of your parameters file located 
inside /Project1/project/bin/.