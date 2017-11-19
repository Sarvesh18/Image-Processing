#include<cv.h>
#include<highgui.h>

using namespace std;
using namespace cv;
int main()
{
	VideoCapture cap(2);
	int min=0;
	double A[180];
    Mat image, threshold, video, thrushold, noise;
	vector<vector<Point> > contours,contourstill;
	vector<Vec4i> hierarchy,hierarchystill;
    namedWindow("control", CV_WINDOW_NORMAL);
        int huelow = 0;
        int huehigh = 255;
        int satlow = 0;
        int Sathigh = 255;
        int vallow = 0;
        int valhigh = 255;
		int amin = 950;
        int amax = 1050;
        cvCreateTrackbar("LowH", "control", &huelow, 255);
        cvCreateTrackbar("HighH", "control", &huehigh, 255);
        cvCreateTrackbar("LowS", "control", &satlow, 255);
        cvCreateTrackbar("HighS", "control", &Sathigh, 255);
        cvCreateTrackbar("LowV", "control", &vallow, 255);
        cvCreateTrackbar("HighV", "control", &valhigh, 255);
		cvCreateTrackbar("LowA", "control", &amin, 1800);
        cvCreateTrackbar("HighA", "control", &amax, 1800);
    Mat element = getStructuringElement( MORPH_RECT,Size( 10, 10 ),Point( 0, 0 ) );
	image = imread("C:\\Shape.jpg",CV_LOAD_IMAGE_COLOR);
	inRange(image,Scalar(0,0,0),Scalar(255,255,90),threshold);
	findContours( threshold.clone(), contourstill, hierarchystill, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    drawContours( image, contourstill, -1 , Scalar(0,255,255), 2, 8, hierarchystill, 3, Point());
	while(1)
	{
		cap.read(video);
		//cvtColor(video,video,CV_BGR2HSV);
		inRange(video,Scalar(huelow,satlow,vallow),Scalar(huehigh,Sathigh,valhigh),thrushold);
		erode(thrushold,noise,element);
		dilate(noise,noise,element);
		dilate(noise,noise,element);
		erode(noise,noise,element);
        findContours( noise.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        drawContours( video, contours, -1, Scalar(0,255,255), 2, 8, hierarchy, 3, Point());
		if(contours.size()>0)
		{
			for(int i=0; i<contours.size() ; i++)
			{
				A[i] = contourArea(contours[i]);
				if( A[i]>amin && A[i]<amax)
				{  
					double index=matchShapes( contourstill[0] , contours[i] , CV_CONTOURS_MATCH_I1,0);
					{ 
						if(index<=min)
						{
							index=min;
							Moments mymoments = moments(contours[i]);
							double m01 = mymoments.m01;
							double m10 = mymoments.m10;
							double m00 = mymoments.m00;
							int x = m10/m00;
							int y = m01/m00;
							cout<<"Area = "<< m00 <<endl;
							cout<<" x = "<< x <<" y = "<< y <<endl;
			                putText(video,"Square",Point(x,y),2,1,Scalar(110,101,145),1,2);	
						}
					}
				}
			}
		}	
		imshow("Video", video);
	    imshow("Thrushold", thrushold);	
        //imshow("Noise", noise);
		waitKey(1);
	}
	return 0;
}