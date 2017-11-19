#include <math.h>
#include <cv.h>
#include <highgui.h>
#include "windows.h"

using namespace std;
using namespace cv;

int robotmove(char);
HANDLE chhonkar_serial;

int main()
{
	VideoCapture cap(1);
	
	//Bluetooth Communication
	chhonkar_serial = CreateFile("COM8", GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);	
	DCB dcbSerial = {0};
	dcbSerial.DCBlength=sizeof(dcbSerial);
	if (!GetCommState(chhonkar_serial, &dcbSerial)) 
    {
		cout<<"dcb error"<<endl;
	}
	dcbSerial.DCBlength=sizeof(dcbSerial);
	dcbSerial.BaudRate=CBR_9600;
	dcbSerial.ByteSize=8;
	dcbSerial.StopBits=ONESTOPBIT;
	dcbSerial.Parity=NOPARITY;
	if(!SetCommState(chhonkar_serial, &dcbSerial))
    {
		cout<<"error setting serial port state"<<endl;
	}

	int n=0,k=0;
	double min=1;
	double d[180];	
	int arr[180][3];
	int x1=0,y1=0,x2=0,y2=0,x3=0,y3=0;
	int tempX1 = 0;
	double maxX1 = 180;
	int tempX2 = 0;
	double maxX2 = 180;
	int tempX3 = 0;
	double maxX3 = 180;

	cv::Mat image, threshold;
    Mat video, videoHSV, Xvideo, XvideoHSV, thrushold, Xthrushold, X1thrushold, X2thrushold, X3thrushold, noise, Xnoise, X1noise, X2noise, X3noise;
	vector<vector<Point> > contours, contoursX, contoursX1, contoursX2, contoursX3, contourstill;
	vector<Vec4i> hierarchy, hierarchyX, hierarchyX1, hierarchyX2, hierarchyX3, hierarchystill;
    
	namedWindow("control", CV_WINDOW_NORMAL);
    int huelow = 0;
    int huehigh =255;
    int satlow = 0;
    int Sathigh = 255;
    int vallow = 0;
    int valhigh = 255;
	//int amin = 0;
	//int amax = 1800;
    cvCreateTrackbar("LowH", "control", &huelow, 255);
    cvCreateTrackbar("HighH", "control", &huehigh, 255);
    cvCreateTrackbar("LowS", "control", &satlow, 255);
    cvCreateTrackbar("HighS", "control", &Sathigh, 255);
    cvCreateTrackbar("LowV", "control", &vallow, 255);
    cvCreateTrackbar("HighV", "control", &valhigh, 255);
	//cvCreateTrackbar("LowA", "control", &amin, 1500);
	//cvCreateTrackbar("HighA", "control", &amax, 1500);

    Mat element = getStructuringElement( MORPH_RECT,Size( 10, 10 ),Point( 0, 0 ) );

	image = imread("C:\\Shape.jpg",CV_LOAD_IMAGE_COLOR);
	inRange(image,Scalar(0,0,0),Scalar(255,255,90),threshold);
	findContours( threshold.clone(), contourstill, hierarchystill, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    drawContours( image, contourstill, -1 , Scalar(0,255,255), 2, 8, hierarchystill, 3, Point());

	for(int a=0;a<180;a++)
	{
		cap.read(Xvideo);
        cvtColor(Xvideo,XvideoHSV,CV_BGR2HSV);
	    //Arena-Blue Detection
	    inRange(XvideoHSV,Scalar(101,45,33),Scalar(141,168,108),Xthrushold);
		//inRange(XvideoHSV,Scalar(81,53,53),Scalar(131,176,118),Xthrushold);
	}
	erode(Xthrushold,Xnoise,element);
	dilate(Xnoise,Xnoise,element);
	dilate(Xnoise,Xnoise,element);
	erode(Xnoise,Xnoise,element);
	findContours( Xthrushold.clone(), contoursX, hierarchyX, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	//findContours( Xnoise.clone(), contoursX, hierarchyX, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	drawContours( Xvideo, contoursX, -1, Scalar(0,255,255), 2, 8, hierarchyX, 3, Point());
    //if(contoursX.size()>0)
	for(int i=0; i<contoursX.size() ; i++)
	{
		int area = contourArea(contoursX[i]);
		if(950<area && area<1050)
		{  
			//double index=matchShapes( contourstill[0] , contoursX[i] , CV_CONTOURS_MATCH_I1,0);
			{ 
				//if(index<=min)
				{
					//index=min;
					Moments mymoments = moments(contoursX[i]);
					double m01 = mymoments.m01;
					double m10 = mymoments.m10;
					double m00 = mymoments.m00;
					int x = m10/m00;
					int y = m01/m00;
					arr[k][0]=x;
	                arr[k][1]=y;
					cout<<"Area = "<< m00 <<endl;
					cout<<" x = "<< arr[k][0] <<" y = "<< arr[k][1] <<endl;
			        putText(Xvideo,"Square",Point(x,y),2,1,Scalar(110,101,145),1,2);	
					k++;
					n++;
				}
			}
		}
	}

	while(1)
	{
		cap.read(video);
		cvtColor(video,videoHSV,CV_BGR2HSV);
		
		//Arena Detection
		inRange(videoHSV,Scalar(huelow,satlow,vallow),Scalar(huehigh,Sathigh,valhigh),thrushold);
		erode(thrushold,noise,element);
		dilate(noise,noise,element);
		dilate(noise,noise,element);
		erode(noise,noise,element);
		findContours( noise.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		drawContours( video, contours, -1, Scalar(0,255,255), 2, 8, hierarchy, 3, Point());

		//Back-Pink Detection   
	    inRange(videoHSV,Scalar(161,155,128),Scalar(204,218,185),X1thrushold);
		//inRange(videoHSV,Scalar(169,147,131),Scalar(192,187,171),X1thrushold);
		erode(X1thrushold,X1noise,element);
		dilate(X1noise,X1noise,element);
		dilate(X1noise,X1noise,element);
		erode(X1noise,X1noise,element);
		findContours( X1thrushold.clone(), contoursX1, hierarchyX1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	    //findContours( X1noise.clone(), contoursX1, hierarchyX1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		drawContours( video , contoursX1, -1 , Scalar(0,255,255), 2, 8, hierarchyX1, 3, Point());
		if(contoursX1.size()>0)
		{
		for(int b=0;b<contoursX1.size();b++)
		{
			double areaX1 = contourArea(contoursX1[b]);
		    if( areaX1>maxX1)
			{   
				tempX1=b;
				maxX1=areaX1;
            }
		}
		Moments mymoments1 = moments(contoursX1[tempX1]);
        double m101 = mymoments1.m01;
        double m110 = mymoments1.m10;
        double m100 = mymoments1.m00;
	    x1 = m110/m100;
        y1 = m101/m100;
     	cout<<" x1 = "<< x1 <<" y1 = "<< y1 <<endl;

		//Front-Yellow Detection
		inRange(videoHSV,Scalar(10,154,128),Scalar(35,231,197),X2thrushold);
		//inRange(videoHSV,Scalar(7,161,103),Scalar(33,215,136),X2thrushold);
		erode(X2thrushold,X2noise,element);
		dilate(X2noise,X2noise,element);
		dilate(X2noise,X2noise,element);
		erode(X2noise,X2noise,element);
		findContours( X2thrushold.clone(), contoursX2, hierarchyX2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	    //findContours( X2noise.clone(), contoursX2, hierarchyX2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		drawContours( video , contoursX2, -1 , Scalar(0,255,255), 2, 8, hierarchyX2, 3, Point());
		if(contoursX2.size()>0)
		{
		for(int f=0;f<contoursX2.size();f++)
		{
			double areaX2 = contourArea(contoursX2[f]);
		    if( areaX2>maxX2)
			{   
				tempX2=f;
				maxX2=areaX2;
            }
		}
		Moments mymoments2 = moments(contoursX2[tempX2]);
        double m201 = mymoments2.m01;
        double m210 = mymoments2.m10;
        double m200 = mymoments2.m00;
	    x2 = m210/m200;
        y2 = m201/m200;
     	cout<<" x2 = "<< x2 <<" y2 = "<< y2 <<endl;
		
		//Last-Red Detection
	    //inRange(XvideoHSV,Scalar(10,154,128),Scalar(35,231,197),X3thrushold);
        inRange(XvideoHSV,Scalar(0,178,23),Scalar(201,217,103),X3thrushold);
	    erode(X3thrushold,X3noise,element);
	    dilate(X3noise,X3noise,element);
    	dilate(X3noise,X3noise,element);
	    erode(X3noise,X3noise,element);
	    findContours( X3thrushold.clone(), contoursX3, hierarchyX3, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        //findContours( X3noise.clone(), contoursX3, hierarchyX3, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	    drawContours( video , contoursX3, -1 , Scalar(0,255,255), 2, 8, hierarchyX3, 3, Point());
	    if(contoursX3.size()>0)
	    {
	    for(int l=0;l<contoursX3.size();l++)
		{
			double areaX3 = contourArea(contoursX3[l]);
		    if( areaX3>maxX3)
		    {   
				tempX3=l;
				maxX3=areaX3;
			}
		}
		Moments mymoments3 = moments(contoursX3[tempX3]);
		double m301 = mymoments3.m01;
		double m310 = mymoments3.m10;
		double m300 = mymoments3.m00;
		x3 = m310/m300;
		y3 = m301/m300;
		cout<<" x3 = "<< x3 <<" y3 = "<< y3 <<endl;
		putText(Xvideo,"x3",Point(x3,y3),2,1,Scalar(255,0,255),1,2);
		}
		}
		}

	    line(video,Point(x1,y1),Point(x2,y2),Scalar(255,0,255),2,7,0);	
		
		//imshow("Image",image);
        //imshow("Threshold",threshold);
		imshow("Video", video);
		imshow("Thrushold", thrushold);
		imshow("XVideo", Xvideo);
		imshow("XThrushold", Xthrushold);
		//imshow("X1Thrushold",X1thrushold);
		//imshow("X2Thrushold",X2thrushold);
		//imshow("X3Thrushold",X3thrushold);

		while(n>0)
		{
			for(int z=0; z<n; z++)
			{
			    d[z]=pow((x1-arr[z][0]),2)+pow((x2-arr[z][1]),2);    
			}

			for(int c=0; c<n; c++)
			{
				for(int j=0; j<n-c-1; j++)
				{
					if(d[j]<d[j+1])
					{
						double temp=d[j];
						double tempx=arr[j][0];
						double tempy=arr[j][1];

						d[j]=d[j+1];
						arr[j][0]=arr[j+1][0];
						arr[j][1]=arr[j+1][1];
						
						d[j+1]=temp;
						arr[j+1][0]=tempx;
						arr[j+1][1]=tempy;
					}
				}
			}

		    putText(Xvideo,"x",Point(arr[n-1][0],arr[n-1][1]),2,1,Scalar(255,0,255),1,2);
	        line(Xvideo,Point(x1,y1),Point(arr[n-1][0],arr[n-1][1]),Scalar(255,0,255),2,7,0);
			
			//Angle in Degree
		    //if(x1>0 && x2>0 && arr[n-1][0]>0 && 0<arr[n-1][1] && 0<y1 && 0<y2)
			{
				float distance=atan(((arr[n-1][1]-y1)*(x2-x1)-(y2-y1)*(arr[n-1][0]-x1))/((x2-x1)*(arr[n-1][0]-x1)+(arr[n-1][1]-y1)*(y2-y1)))*180/3.14159;
	            double deviation=(arr[n-1][1]-y1)*x2-(arr[n-1][0]-x1)*y2+arr[n-1][0]*y1-arr[n-1][1]*x1;
			    cout<<" Distance = "<< distance <<endl;
				cout<<" Deviation = "<< deviation <<endl;
		        if(distance<12.0 || distance>-12.0)
				{
					cout<<" move = F "<<endl;
			        robotmove('W'); 
			    }
				else if(distance<-10.0)
				{
					cout<<" move = L "<<endl;
			        robotmove('A');
				}
				else if(distance>10.0)
				{
					cout<<" move = R "<<endl;
			        robotmove('D');
				}
				//else if(distance>85|| distance<90)
				//robotmove('R');
				else
				robotmove('W');
			}
        
    	    maxX1=0;
	   	    maxX2=0;
	   	    tempX1=0;
	   	    tempX2=0;

		    //Back-Pink Detection   
	        inRange(videoHSV,Scalar(161,155,128),Scalar(204,218,185),X1thrushold);
		    //inRange(videoHSV,Scalar(169,147,131),Scalar(192,187,171),X1thrushold);
		    erode(X1thrushold,X1noise,element);
		    dilate(X1noise,X1noise,element);
		    dilate(X1noise,X1noise,element);
		    erode(X1noise,X1noise,element);
		    findContours( X1thrushold.clone(), contoursX1, hierarchyX1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	        //findContours( X1noise.clone(), contoursX1, hierarchyX1, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		    drawContours( video , contoursX1, -1 , Scalar(0,255,255), 2, 8, hierarchyX1, 3, Point());
	    if(contoursX1.size()>0)
	    {
		for(int b=0;b<contoursX1.size();b++)
		{
			double areaX1 = contourArea(contoursX1[b]);
		    if( areaX1>maxX1)
			{   
				tempX1=b;
				maxX1=areaX1;
            }
		}
		Moments mymoments1 = moments(contoursX1[tempX1]);
        double m101 = mymoments1.m01;
        double m110 = mymoments1.m10;
        double m100 = mymoments1.m00;
	    x1 = m110/m100;
        y1 = m101/m100;
     	cout<<" x1 = "<< x1 <<" y1 = "<< y1 <<endl;

		    //Front-Yellow Detection
	    	inRange(videoHSV,Scalar(10,154,128),Scalar(35,231,197),X2thrushold);
		    //inRange(videoHSV,Scalar(7,161,103),Scalar(33,215,136),X2thrushold);
		    erode(X2thrushold,X2noise,element);
		    dilate(X2noise,X2noise,element);
		    dilate(X2noise,X2noise,element);
		    erode(X2noise,X2noise,element);
		    findContours( X2thrushold.clone(), contoursX2, hierarchyX2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	        //findContours( X2noise.clone(), contoursX2, hierarchyX2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		    drawContours( video , contoursX2, -1 , Scalar(0,255,255), 2, 8, hierarchyX2, 3, Point());
		if(contoursX2.size()>0)
		{
		for(int f=0;f<contoursX2.size();f++)
		{
			double areaX2 = contourArea(contoursX2[f]);
		    if( areaX2>maxX2)
			{   
				tempX2=f;
				maxX2=areaX2;
            }
		}
		Moments mymoments2 = moments(contoursX2[tempX2]);
        double m201 = mymoments2.m01;
        double m210 = mymoments2.m10;
        double m200 = mymoments2.m00;
	    x2 = m210/m200;
        y2 = m201/m200;
     	cout<<" x2 = "<< x2 <<" y2 = "<< y2 <<endl;
		}
		}

        if(d[n-1]<2)
		{
			n--;
		}
		}

		if(n=0)
		{
			//Angle in Degree
			{
				float distance=atan(((y3-y1)*(x2-x1)-(y2-y1)*(x3-x1))/((x2-x1)*(x3-x1)+(y3-y1)*(y2-y1)))*180/3.14159;
	            double deviation=(y3-y1)*x2-(x3-x1)*y2+x3*y1-y3*x1;
				cout<<" Distance = "<< distance <<endl;
			    cout<<" Deviation = "<< deviation <<endl;
		        if(distance<12.0 || distance>-12.0)
				{
					cout<<" move = F "<<endl;
			    	robotmove('W'); 
			    }
				else if(distance<-10.0)
				{
					cout<<" move = L "<<endl;
			        robotmove('A'); 
				}
				else if(distance>10.0)
				{
					cout<<" move = R "<<endl;
			        robotmove('D'); 
				}
				//else if(distance>85|| distance<90)
				//robotmove('R'); 
				else
				robotmove('W');
			}       
		}

	    waitKey(1);	
	    maxX1=0;
	    maxX2=0;
	    tempX1=0;
 	    tempX2=0;
	}
	return 0;
}

int robotmove(char sz)
{
	char szBuff[16] ={sz};
	DWORD dwBytesRead;
	WriteFile(chhonkar_serial, szBuff,16, &dwBytesRead, NULL);
	return 0;
}
