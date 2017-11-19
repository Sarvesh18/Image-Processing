#include <cv.h>
#include <highgui.h>
#include "windows.h"

using namespace std;
using namespace cv;


int robotmove(char);
HANDLE chhonkar_serial;

int main()
{
	//=================for bluetooth communication================
	chhonkar_serial = CreateFile("COM4", GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);	
	DCB dcbSerial = {0};
	dcbSerial.DCBlength=sizeof(dcbSerial);
	if (!GetCommState(chhonkar_serial, &dcbSerial)) {
		cout<<"dcb error"<<endl;
	}
	dcbSerial.DCBlength=sizeof(dcbSerial);
	dcbSerial.BaudRate=CBR_9600;
	dcbSerial.ByteSize=8;
	dcbSerial.StopBits=ONESTOPBIT;
	dcbSerial.Parity=NOPARITY;
	if(!SetCommState(chhonkar_serial, &dcbSerial)){
		cout<<"error setting serial port state"<<endl;
	}
	//================================write your code from here==============================


	robotmove('A'); // for left side move
	robotmove('W'); // for forward move
	robotmove('S'); // for backward move
	robotmove('D'); // for right side move
	return 0;
}

int robotmove(char sz)
{
	char szBuff[16] ={sz};
	DWORD dwBytesRead;
	WriteFile(chhonkar_serial, szBuff,16, &dwBytesRead, NULL);
	return 0;
}
