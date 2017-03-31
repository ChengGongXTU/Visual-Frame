#include"pathtrace.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd) {

	//initialize buffer
	for (int i = 0; i < w*h; i++) {
		zbuffer[i] = -1.f;
		pbuffer[4 * i] = (BYTE)(int)(backcolor[0]*255);
		pbuffer[4 * i + 1] = (BYTE)(int)(backcolor[1] * 255);
		pbuffer[4 * i + 2] = (BYTE)(int)(backcolor[2] * 255);
		pbuffer[4 * i + 3] = (BYTE)0;
	}

	//initialize DIBsection
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = w;
	bmih.biHeight = h;
	bmih.biPlanes = 1;
	bmih.biBitCount = 32;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;
	
	hBitmap = CreateDIBSection(NULL, (BITMAPINFO*)&bmih, 0, (void**)&pBits, NULL, 0);

	//initialize windows
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = ClassName;

	if (!RegisterClass(&wc)) {
		MessageBox(NULL, L"window can't be register", ClassName, MB_ICONERROR);
		return 0;
	}
		
	hwnd = CreateWindow(ClassName, L"Visual window", 0, 0, 0, w,h,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;

}



LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {


	float phong = 0.8f;
	float red = 0.75,f;
	float green = 0.55f;
	float white = 0.9f;
	float blue = 0.75f;

	TriangleMesh tMesh1, tMesh2, tMesh3,tMesh4;

	LoadObj(tMesh1, "diban2.obj");
	setObj(tMesh1, Dif, Ls(0.5, 0.5, 0.5),Vector(-120.f, 0, 140.f));

	LoadObj(tMesh2, "HCYuanzhu.obj");
	setObj(tMesh2, Dif,Ls(0.8,0.8,0.0),Vector(-120.f, 0,140.f ));
	Texture text2;
	LoadTexture(text2, "hongci.bmp");
	BindTexture(&text2, tMesh2);

	LoadObj(tMesh3, "FSHBall.obj");
	setObj(tMesh3, Dif, Ls(white,0,0),Vector(-120.f, 0 , 140.f));
	Texture text3;
	float shadeTable[] = { 0.0f,0.2f,0.26,1.f };
	Set1DTexture(text3, shadeTable,2);
	BindTexture(&text3, tMesh3);
	SetCartoonStyle(1, tMesh3);
	//LoadTexture(text3, "fushihui.bmp");
	//BindTexture(&text3, tMesh3);

	LoadObj(tMesh4, "chahu.obj");
	setObj(tMesh4, Dif, Ls(0,white,  0), Vector(-120.f, 0, 140.f));
	Texture text4;
	LoadTexture(text4, "qinghuaci.bmp");
	BindTexture(&text4, tMesh4);

	TriangleMesh mesh[4] = { tMesh1,tMesh2,tMesh3,tMesh4 };
	Point eye = Point(0, 800, -800);
	Point gaze = Point(0.f,0.f, 0.f);
	Vector upView = Vector(0.f, 1.f, 0.f);
	PointLight pl = PointLight(-2500.f, 5000.f,-2500.f,64,1,0.2);
	float wn = -80;
	Camera camera = Camera(eye, gaze, upView, wn, -wn, wn*h /w, -wn*h/w,-200, -30000.f, w, h);
	//viewport
	//Point eye = Point(278.f, 273.f, -800.f + 0.035);
	//Point gaze = Point(278.f, 273.f, 0.f);
	//Vector upView = Vector(0.f, 1.f, 0.f);
	//PointLight pl = PointLight(400.f, 400.f, 0.f,256,1.0, 0.2);
	////PointLight pl2 = PointLight(100,100,0.f, 32,1.f,0.1);
	//Camera camera = Camera(eye, gaze, upView, -0.0125f, 0.0125f, -0.0125f, 0.0125f, -0.035f, -30000.f, w, h);

	switch (message) {

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		//draw square
		RenderPipeline(mesh, 4, camera, pl);
		screenupdate();
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_SPACE)	bool test = PathTrace(w,h,2, camera, mesh,4,5,8);

	case WM_DESTROY:
		delete zbuffer;
		PostQuitMessage(0);
		return 0;
		
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}