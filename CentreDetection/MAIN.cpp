#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <tuple>
#include <vector>
#include <fstream>
#include <ctime>
#include <uEye.h>
#include <uEye_tools.h>
#include <ueye_deprecated.h>
#include <wchar.h>
#include <locale.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "stdafx.h"
#include <windows.h>



using namespace cv;  
using namespace std;
//using boost::tuple;
char key;
// Chotko
/// Global variables für Canny
int edgeThresh = 1;//ok
int lowThreshold = 15 ;
int const max_lowThreshold = 100;
int ratio = 2;//war 3 ist ok oder vllt 2
int kernel_size = 3;//gut so
char* window_name_rgb = "Laser Beam in RGB";
char* window_name_bw = "Laser Beam in BW";


typedef vector <tuple<int,int>> coordinates;
 
/*//extreme Coordinates
extLeft = tuple(c[c[:, :, 0].argmin()][0]);
extRight = tuple(c[c[:, :, 0].argmax()][0]);
extTop = tuple(c[c[:, :, 1].argmin()][0]);
extBot = tuple(c[c[:, :, 1].argmax()][0]);
*/

	string im_name="Bild3";
	string im_extension = ".jpg";
	string bw = "_bw";
	Mat im_rgb  = imread (im_name + im_extension);
	Mat im_gray;
	Mat dst, detected_edges;
	Mat im_gray1;
	
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);

	
void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur( im_gray, detected_edges, Size(3,3) );
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
/// Find contours
  findContours( detected_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  /// Using Canny's output as a mask, we display our resultt
  dst = Scalar::all(0);

Mat drawing = Mat::zeros( detected_edges.size(), CV_8UC3 );
  for( int i = 0; i < contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point(5,5) );
     }
  
  im_gray.copyTo( dst, detected_edges);
  imshow( window_name_bw, dst ); 
  imshow( "Contours", drawing );
 }


/*void Readyforextremepoints(int, void*)
{
	thresh = threshold(im_gray, 45, 255, cv2.THRESH_BINARY)
	thresh = erode(im_gray, None, iterations=2)
	thresh = dilate(im_gray, None, iterations=2)
}*/



//TRESHOLD
int threshold_value = 0;
int threshold_type = 3;;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;

char* window_name_tresholddemo = "Threshold Demo";
char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
char* trackbar_value = "Value";


void Threshold_Demo( int, void* )
{
  /* 0: Binary
     1: Binary Inverted
     2: Threshold Truncated
     3: Threshold to Zero
     4: Threshold to Zero Inverted
   */
  threshold( im_gray, dst, threshold_value, max_BINARY_value,threshold_type );
  imshow( window_name_tresholddemo, dst );
}



/*int[] extreme_Points(int ax, int ay, int bx, int by)
{
int erg[] = new int[2];
erg[0] = ax + bx;
erg[1] = ay + by;
 
return erg;
}*/





//Milenas Prog
int main(int argc, char *argv[])
{
	string file_path = "D:/Documents/Visual Studio 2012/Projects/CentreDetection/CentreDetection.txt";
	ofstream myfile;
	myfile.open (file_path, ios::trunc); // Inhalt der vorhandenen Datei löschen
	myfile	<< "Datum und Uhrzeit\t" << "Leistung" << endl << endl;
	myfile.close();

//	time_t t;
	//struct tm now;

	HIDS hCam = 0;
	char* imgMem;
	int memId;
	
	if(is_InitCamera (&hCam, NULL)!= IS_SUCCESS)
	{
		cout << " Initialisierungsproblem: Kamera ausstecken-einstecken?" << endl;
//		return 0;
	}

	const int img_width=3840, img_height=2748, img_bpp=8;
	is_AllocImageMem (hCam, img_width, img_height, img_bpp, &imgMem, &memId);
	is_SetImageMem (hCam, imgMem, memId);
	is_SetDisplayMode (hCam, IS_SET_DM_DIB); //Bitmap-Modus
	is_SetColorMode (hCam, IS_CM_MONO8);
	is_SetImageSize (hCam, img_width, img_height);

	double disable = 0;
	is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_GAIN, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_FRAMERATE, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SHUTTER, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SENSOR_GAIN, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE,&disable,0);
	is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SENSOR_SHUTTER, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SENSOR_GAIN_SHUTTER,&disable,0);
	is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SENSOR_FRAMERATE, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_REFERENCE, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_ANTI_FLICKER_MODE, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_SENS_AUTO_BACKLIGHT_COMP, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_SENS_AUTO_CONTRAST_CORRECTION, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_SENS_AUTO_SHUTTER_PHOTOM, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_SKIPFRAMES, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_WB_SKIPFRAMES, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_SENS_AUTO_GAIN_PHOTOM, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_WB_HYSTERESIS, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_REFERENCE, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_GAIN_MAX, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_SHUTTER_MAX, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_SPEED, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_WB_OFFSET, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_WB_GAIN_RANGE, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_WB_SPEED, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_WB_ONCE, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_BRIGHTNESS_ONCE, &disable, 0);
	is_SetAutoParameter (hCam, IS_SET_AUTO_HYSTERESIS, &disable, 0);

	double FPS,NEWFPS;
	FPS = 3; // 2.13 ist max
	is_SetFrameRate(hCam,FPS,&NEWFPS);

	double Exposure = 300; // Belichtungszeit (in ms)
	is_Exposure(hCam, IS_EXPOSURE_CMD_SET_EXPOSURE, (void*) &Exposure, sizeof(Exposure));

	is_SetGamma(hCam,100); // Default = 100, corresponds to a gamma value of 1.0
	is_Focus (hCam, FOC_CMD_SET_DISABLE_AUTOFOCUS, NULL, 0);
	is_SetHardwareGain (hCam, 1, 0, 0, 0);

	int ii = 0;
	short stop=0;

	while (stop==0)
	{		
		if(is_FreezeVideo(hCam, IS_WAIT) == IS_SUCCESS){
			void *pMemVoid; //pointer to where the image is stored
			is_GetImageMem (hCam, &pMemVoid);
			IplImage * img;
			img=cvCreateImage(cvSize(img_width, img_height), IPL_DEPTH_8U, 1); 
			img->nSize=sizeof(IplImage);
			img->ID=0;
			img->nChannels=1;
			img->alphaChannel=0;
			img->depth=8;
			img->dataOrder=0;
			img->origin=0;
			img->align=4;	// egal
			img->width=img_width;
			img->height=img_height;
			img->roi=NULL;
			img->maskROI=NULL;
			img->imageId=NULL;
			img->tileInfo=NULL;
			img->imageSize=img_width*img_height;
			img->imageData=(char*)pMemVoid;  //the pointer to imagaData
			img->widthStep=img_width;
			img->imageDataOrigin=(char*)pMemVoid; //and again
/*
			// Mittelwert aller Pixel
			double sum = 0;
			double result = 0.0;
			for (int i = 0; i < img->imageSize; i++)
			{
				sum += img->imageData[i];					
			}
			result = (double)sum/img->imageSize;

			// aktuelle Zeit
			t = time(0);
			localtime_s(&now, &t);

			
			// in die Datei schreiben
			if (ii!=0) // der erste Wert ist falsch, nicht ausgeben!
			{
				myfile.open (file_path, ios::app);
				myfile	<< now.tm_mday << "." << now.tm_mon+1
					<< "." << now.tm_year+1900 << " "
					<< now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec
					<< "\t" << result << endl;
				myfile.close();
			} ii=2; 

			// Resize img (für cvShowImage)
			IplImage *img_resized;
			double resize_factor = 0.1;
			img_resized = cvCreateImage(cvSize((int)(resize_factor*img->width), (int)(resize_factor*img->height)), img->depth, img->nChannels);
			cvResize(img, img_resized);

			

			cvNamedWindow( "A", 1 );
			cvShowImage("A",img_resized);
			cv::waitKey(1);*/
		}
	stop = GetAsyncKeyState(VK_LSHIFT); 
	}

	is_ExitCamera(hCam);
	return 0;
}













/*
int main()
{
	 /*better version to explain
   if( argc != 2)
    {
     cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }
	
	namedWindow( window_name_rgb, CV_WINDOW_AUTOSIZE );
    imshow( window_name_rgb, im_rgb );
	cvtColor(im_rgb,im_gray,CV_RGB2GRAY); //RGB to grayscale
	Mat im_bw = im_gray > 128; //convert to binary
	imwrite((im_name+bw+im_extension), im_bw); //save to disk
	//namedWindow( window_name_bw, CV_WINDOW_AUTOSIZE );
   // imshow( window_name_bw, im_bw );
	/// Create a matrix of the same type and size as im_gray (for dst)
	dst.create( im_gray.size(), im_gray.type() );

int rows = im_gray.rows;
int cols = im_gray.cols;
cv::Size s = im_gray.size();
rows = s.height;
cols = s.width;
cout << s << "groesse im gray" <<endl;

   	
	//blur( im_gray, im_gray, Size(3,3) ); //blur it
	     
		
  /// Create a Trackbar for user to enter threshold
							//createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
createTrackbar( trackbar_type,
                  window_name_tresholddemo, &threshold_type,
                  max_type, Threshold_Demo );

createTrackbar( trackbar_value,
                  window_name_tresholddemo, &threshold_value,
                  max_value, Threshold_Demo );

CannyThreshold(0, 0);


//cout << contours<<std::vector<<cv:: << endl;
	for (int i = 0; i < 5thcontour.size(); i++) 
		{
    Point coordinate_i_ofcontour = 5thcontour.size();
    cout << endl << "contour with coordinates: x = " << coordinate_i_ofcontour.x << " y = " << coordinate_i_ofcontour.y;
}
*/
  
  /*	GaussianBlur( im_gray, im_gray, Size(10,10), 0, 0, BORDER_DEFAULT );
	
	convertScaleAbs( im_gray1, im_gray1);
	
	vector<Vec3f> circles;
    HoughCircles(im_gray1, circles, CV_HOUGH_GRADIENT,1, im_gray.rows/2, 100, 20, 0,im_gray.cols/2 );	
	
	 for( size_t i = 0; i < circles.size(); i++ )
    {
         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         // draw the circle center
         circle( im_rgb, center, 3, Scalar(0,255,0), -1, 8, 0 );
         // draw the circle outline
         circle( im_rgb, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }
    namedWindow( "circles", 1 );
    imshow( "circles", im_rgb );
	imshow( "circles2", im_gray );
	cout << circles.size() << " Groesse der Kreise" <<endl;
  		// Show our image inside it.
		

    waitKey(0);			// Wait for a keystroke in the window
    return 0;
}
*/

