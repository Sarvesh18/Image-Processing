#include <baseapi.h>
#include <allheaders.h>
#include <iostream>

using namespace std;

int main(void){
	tesseract::TessBaseAPI api;
	api.Init("", "eng", tesseract::OEM_DEFAULT);
	api.SetPageSegMode(static_cast<tesseract::PageSegMode>(7));
	api.SetOutputName("out");
	cout << "File name:";
	char image[256];
	cin >> image;

	cout << endl;

	PIX   *pixs = pixRead(image);
	STRING text_out;
	api.ProcessPages(image, NULL, 0, &text_out);
	cout << text_out.string();
}