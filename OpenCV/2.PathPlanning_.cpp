#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

int main()
{
	int c=0;
	int n=0,k=0;
	double d[180];	
	int arr[180][3];

    Mat Xvideo, Xthrushold, Xnoise;
	vector<vector<Point> > contoursX;
	vector<Vec4i> hierarchyX;
		
	Xvideo = imread("C:\\Drishti.jpg",CV_LOAD_IMAGE_COLOR);
    inRange(Xvideo,Scalar(125,0,0),Scalar(255,17,27),Xthrushold);
	findContours( Xthrushold.clone(), contoursX, hierarchyX, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	drawContours( Xvideo, contoursX, -1, Scalar(0,255,255), 2, 8, hierarchyX, 3, Point());
    //if(contoursX.size()>0)
	for(int i=0; i<contoursX.size() ; i++)
	{
		int area = contourArea(contoursX[i]);
		if(1260<area && area<1295)
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
		//while(n>0)
		{
			for(int z=0; z<n; z++)
			{
			    d[z]=pow((72-arr[z][0]),2)+pow((445-arr[z][1]),2);    
			}
			for(int i=0; i<n; i++)
			{
				for(int j=0;j<n-i-1;j++)
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
			for(int z=0; z<n; z++)
			{
				cout<<" Distance "<< d[z] <<endl;
		        putText(Xvideo,"x3",Point(arr[z][0],arr[z][1]),2,1,Scalar(255,0,255),1,2);
	            line(Xvideo,Point(arr[z][0],arr[z][1]),Point(arr[z+1][0],arr[z+1][1]),Scalar(255,0,255),2,7,0);
			}
		    //n--;

		}
		imshow("XVideo", Xvideo);
		imshow("XThrushold", Xthrushold);
	    waitKey(1);	
	}
	return 0;
}