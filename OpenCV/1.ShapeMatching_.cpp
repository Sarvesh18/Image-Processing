#include<cv.h>
#include<highgui.h>

using namespace std;
using namespace cv;

int main()
{    
	int i,x,y,tempstill=0;
	double min = 1;
	double A[180];
	VideoCapture cap(0);
	cv::Mat image, threshold;
	Mat video, thrushold, thrusholds, noise;
	vector<vector<Point> > contours,contourstill;
	vector<Vec4i> hierarchy,hierarchystill;
	namedWindow("control", CV_WINDOW_NORMAL);
	int huelow = 0;
	int huehigh = 255;
	int satlow = 0;
	int Sathigh = 255;
	int vallow = 0;
	int valhigh = 255;
	cvCreateTrackbar("LowH", "control", &huelow, 255);
	cvCreateTrackbar("HighH", "control", &huehigh, 255);
	cvCreateTrackbar("LowS", "control", &satlow, 255);
	cvCreateTrackbar("HighS", "control", &Sathigh, 255);
	cvCreateTrackbar("LowV", "control", &vallow, 255);
	cvCreateTrackbar("HighV", "control", &valhigh, 255);
	Mat element = getStructuringElement( MORPH_RECT,Size( 10, 10 ),Point( 0, 0 ) );
	image = imread("C:\\Shape.jpg",CV_LOAD_IMAGE_COLOR);
	inRange(image,Scalar(0,0,0),Scalar(255,255,90),threshold);
	findContours( threshold.clone(), contourstill, hierarchystill, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	drawContours( image, contourstill, -1 , Scalar(0,255,255), 2, 8, hierarchystill, 3, Point());
	while(1)
	{
		cap.read(video);
		//video = imread("C:\\Drishti.jpg",CV_LOAD_IMAGE_COLOR);
		inRange(video,Scalar(huelow,satlow,vallow),Scalar(huehigh,Sathigh,valhigh),thrushold);
		erode(thrushold,noise,element);
		dilate(noise,noise,element);
		dilate(noise,noise,element);
		erode(noise,noise,element);
		findContours( noise.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		drawContours( video, contours, -1, Scalar(0,255,255), 2, 8, hierarchy, 3, Point());
		if(contours.size()>0)
		{
			for(i=0; i<contours.size() ; i++)
			{
				A[i] = contourArea(contours[i]);
				if( A[i]>950 && A[i]<1050)
				{  
					{
						double index=matchShapes( contourstill[0] , contours[i] , CV_CONTOURS_MATCH_I1,0);
						{ 
							if(index<=min)
							{
								index=min;
								tempstill = 0;
								Moments mymoments = moments(contours[i]);
								double m01 = mymoments.m01;
								double m10 = mymoments.m10;
								double m00 = mymoments.m00;
								x = m10/m00;
								y = m01/m00;
								cout<<"Area = "<< m00 <<endl;
								cout<<"Centroid Coordinates (x,y) = "<< x <<","<< y <<endl;
				                if( tempstill==0 )
			                    putText(video,"Square",Point(x,y),2,1,Scalar(110,101,145),1,2);
	                        	else if( tempstill==1 )
			                    putText(video,"Circle",Point(x,y),2,1,Scalar(110,101,145),1,2);
		                        else if( tempstill==2 )
			                    putText(video,"Triangle",Point(x,y),2,1,Scalar(110,101,145),1,2);
							}
						} 
					}
				}
			}
		}
		//imshow("Image",image);
		//imshow("Threshold",threshold);
		imshow("Video", video);
		imshow("Thrushold", thrushold);	
		waitKey(1);
	}
	return 0;
}