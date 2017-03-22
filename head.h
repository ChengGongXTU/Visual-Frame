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




