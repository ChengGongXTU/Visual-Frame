#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<vector>
#include<algorithm>
#include<string.h>
#include<string>
#include<Windows.h>

static int cxClient, cyClient;
static HDC hdc;
PAINTSTRUCT ps;
char* backbuffer = NULL;
unsigned int *buffer;
static int w = 800, h = 800;
HWND hwnd;
MSG msg;
auto ClassName = L"Visual Frame";

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




