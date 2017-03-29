#include"pathtrace.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd) {

	//initialize buffer
	for (int i = 0; i < w*h; i++) {
		zbuffer[i] = -1.f;
		pbuffer[4 * i] = (BYTE)backcolor[2];
		pbuffer[4 * i + 1] = (BYTE)backcolor[1];
		pbuffer[4 * i + 2] = (BYTE)backcolor[0];
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

	TriangleMesh tMesh3, tMesh, tMesh2;
	TriangleMesh mesh[3];

	LoadObj(tMesh2, "chahu2.obj");

	for (int i = 0; i < tMesh2.verNum; i++)
	{
		tMesh2.p[i].z = -tMesh2.p[i].z;
		tMesh2.p[i] = tMesh2.p[i] + Vector(200.f, 50.f, 200.f);
	}

	for (int i = 0; i < tMesh2.nNum; i++) {
		tMesh2.n[i].z = -tMesh2.n[i].z;
	}

	tMesh2.uv = new float[tMesh2.triNum * 3];
	for (int i = 0; i < tMesh2.triNum; i++)
	{
		tMesh2.uv[3 * i] = 0.5;
		tMesh2.uv[3 * i + 1] = 0.75;
		tMesh2.uv[3 * i + 2] = 0.3;
	}

	LoadObj(tMesh, "cornellbox.obj");

	tMesh.uv = new float[30];

	for (int i = 0; i < tMesh.verNum; i++)
	{
		tMesh.p[i].z = -tMesh.p[i].z;
		tMesh.p[i] = tMesh.p[i] + Vector(279.6000, 0.0000, 276.4000);
	}

	for (int i = 0; i < tMesh.nNum; i++) {
		tMesh.n[i].z = -tMesh.n[i].z;
		tMesh.n[i] = -tMesh.n[i];
	}

	//color
	for (int i = 0; i < 2; i++) {
		tMesh.uv[3 * i] = white;
		tMesh.uv[3 * i + 1] = white;
		tMesh.uv[3 * i + 2] = white;
	}

	for (int i = 2; i < 4; i++) {
		tMesh.uv[3 * i] = white;
		tMesh.uv[3 * i + 1] = white;
		tMesh.uv[3 * i + 2] = white;
	}

	for (int i = 4; i < 6; i++) {
		tMesh.uv[3 * i] = 0.25;
		tMesh.uv[3 * i + 1] = 0.25;
		tMesh.uv[3 * i + 2] = blue;
	}

	for (int i = 6; i < 8; i++) {
		tMesh.uv[3 * i] = white;
		tMesh.uv[3 * i + 1] = white;
		tMesh.uv[3 * i + 2] = white;
	}

	for (int i = 8; i < 10; i++) {
		tMesh.uv[3 * i] = red;
		tMesh.uv[3 * i + 1] = 0.25;
		tMesh.uv[3 * i + 2] = 0.25;
	}

	LoadObj(tMesh3, "yuanzu.obj");
	tMesh.reftype == Dif;

	tMesh3.reftype = Dif;
	tMesh3.uv = new float[tMesh3.triNum * 3];
	for (int i = 0; i < tMesh3.triNum; i++) {
		tMesh3.uv[3 * i] = white;
		tMesh3.uv[3 * i + 1] = white;
		tMesh3.uv[3 * i + 2] = 0.1;
	}

	for (int i = 0; i < tMesh3.verNum; i++)
	{
		tMesh3.p[i].z = -tMesh3.p[i].z;
		tMesh3.p[i] = tMesh3.p[i] + Vector(350.f, 10.f, 200.f);
	}

	for (int i = 0; i < tMesh3.nNum; i++) {
		tMesh3.n[i].z = -tMesh3.n[i].z;
	}

	mesh[0] = tMesh;
	mesh[1] = tMesh2;
	mesh[2] = tMesh3;


	//viewport
	Point eye = Point(278.f, 273.f, -800.f + 0.035);
	Point gaze = Point(278.f, 273.f, 0.f);
	Vector upView = Vector(0.f, 1.f, 0.f);
	PointLight pl = PointLight(500.f, 400.f, -100.f,256,1.0, 0.2);
	//PointLight pl2 = PointLight(100,100,0.f, 32,1.f,0.1);
	Camera camera = Camera(eye, gaze, upView, -0.0125f, 0.0125f, -0.0125f, 0.0125f, -0.035f, -30000.f, w, h);

	switch (message) {

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		//draw square
		RenderPipeline(mesh, 3, camera, pl);
		screenupdate();
		return 0;

	case WM_KEYDOWN:


		if (wParam == VK_SPACE)	bool test = PathTrace(w,h,2, camera, mesh,3,5,8);

	case WM_DESTROY:
		delete zbuffer;
		PostQuitMessage(0);
		return 0;
		
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}