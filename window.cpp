#include"render.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd) {
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
	Point p1 = Point(12.f, 3.f, 4.f);
	Point p2 = Point(7.f, 16.f, 9.f);
	Point p3 = Point(13.f, 15.f, 12.f);
	Vertex v1 = Vertex(p1);
	Vertex v2 = Vertex(p2);
	Vertex v3 = Vertex(p3);
	Triangle tri = Triangle(v1, v2, v3);


	Point eye = Point(0.f, 0.f, 0.f);
	Point gaze = Point(0.f, 0.f, 1.f);
	Vector upView = Vector(0.f, 1.f, 0.f);
	Camera camera = Camera(eye, gaze, upView, -20.f, 20.f, -20.f, 20.f, 2.f, 50.f, w, h);

	Triangle tri1 = TriangleTransform(tri, camera);

	switch (message) {
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		BresenhamAlgorithm(hwnd, tri1);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}