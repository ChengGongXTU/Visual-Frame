#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<vector>
#include<algorithm>
#include<string.h>
#include<string>
#include<Windows.h>
#include<time.h>
#include"rand.h"
#include<random>
#include<fstream>
#include<iostream>
#include<istream>
#include<sstream>
#include<vector>

using namespace std;

static int cxClient, cyClient;
std::default_random_engine generator(time(NULL));
std::uniform_real_distribution<double> dis(0.0, 1.0);

//screen size 
int w=400, h=400;
float backcolor[3] = { 0.f,0.f,0.f };

//windows device
static HDC hdc = 0;
HWND hwnd = 0;
MSG msg;
auto ClassName = L"Visual Frame";
PAINTSTRUCT ps;
HBITMAP hBitmap;
BYTE* pBits;
BITMAPINFOHEADER bmih;
BITMAP bitmap;
HDC hdcmem;

//z-depth buffer: 
float* zbuffer = new float[w*h];

//pixel buffer: 4 byte per pixel(a-r-g-b)
BYTE* pbuffer = new BYTE[4*w*h];

#define M_PI 3.1415926

using std::swap;
using std::min;
using std::max;

inline float Radians(float deg) {
	return ((float)M_PI / 180.f) * deg;
}

inline float LineInter(float a, float b, float t) {
	float f;
	f = (1 - t)*a + t*b;
	return f;
}

struct Matrix4x4;

bool FindFace(string &str, int &j,int &k,int &l) {

	int charNum = str.length();
	int begin = 0;
	int end = 0;
	string a;
	string b;
	string c;

	for (int i = 0; i < charNum; i++) {
		if ((int)str[i] == 47) {
			begin = i;
			break;
		}
	}

	for (int i = begin + 1; i < charNum; i++) {
		if ((int)str[i] == 47) {
			end = i;
			break;
		}
	}

	if (begin == 0 && end == 0) {
		return false;
	}

	a = str.substr(0, begin - 0);

	if ((end - begin) != 1) {
		b = str.substr(begin + 1, end - begin - 1);
		k = atoi(b.c_str()) - 1;
	}
	else
		k = 0;

	c = str.substr(end + 1, charNum - end - 1);
	j = atoi(a.c_str())-1;
	l = atoi(c.c_str())-1;

	return true;
}


