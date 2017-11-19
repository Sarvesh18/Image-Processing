#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

int main()
{
	cv::Mat image, thrushold;
	vector<vector<Point> > contourstill;
	vector<Vec4i> hierarchystill;
        namedWindow("control", CV_WINDOW_NORMAL);
        int huelow = 0;
        int huehigh =179;
        int satlow = 0;
        int Sathigh = 255;
        int vallow = 0;
        int valhigh = 255;
        cvCreateTrackbar("LowH", "control", &huelow, 179);
        cvCreateTrackbar("HighH", "control", &huehigh, 179);
        cvCreateTrackbar("LowS", "control", &satlow, 255);
        cvCreateTrackbar("HighS", "control", &Sathigh, 255);
        cvCreateTrackbar("LowV", "control", &vallow, 255);
        cvCreateTrackbar("HighV", "control", &valhigh, 255);
        Mat element = getStructuringElement( MORPH_RECT,Size( 10, 10 ),Point( 0, 0 ) );
	while(1)
	{
		image = imread("C:\\Still.jpg",CV_LOAD_IMAGE_COLOR);
	        inRange(image,Scalar(huelow,satlow,vallow),Scalar(huehigh,Sathigh,valhigh),thrushold);
	        findContours( thrushold.clone(), contourstill, hierarchystill, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
                drawContours( image, contourstill, -1 , Scalar(0,255,255), 2, 8, hierarchystill, 3, Point());
                Moments mymoments = moments(thrushold);
                double m01 = mymoments.m01;
                double m10 = mymoments.m10;
                double m00 = mymoments.m00;
	        int x = m10/m00;
	        int y = m01/m00;
                cout<<"x = "<<x<<", y = "<<y<<endl;    
                putText(image,"Sarvesh",Point(x,y),2,1,Scalar(110,101,145),1,2);   
                imshow("Image",image);
                imshow("Thrushold",thrushold);
	        waitKey(1);
	}	
	return 0;
}
