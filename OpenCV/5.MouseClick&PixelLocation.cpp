#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;


void mouse_location(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          cout << "Right button - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_MBUTTONDOWN )
     {
          cout << "Middle button - position (" << x << ", " << y << ")" << endl;
     }
     else if ( event == EVENT_MOUSEMOVE )
     {
          cout << "Mouse move position  = (" << x << ", " << y << ")" << endl;

     }
}

int main()
{
	cv::Mat image;
	image = imread("C:\\c.jpg",CV_LOAD_IMAGE_COLOR);
	namedWindow("Window",CV_WINDOW_AUTOSIZE);
	setMouseCallback("Window", mouse_location, NULL);

	imshow("Window",image);
	waitKey(0);
	return 0;
}