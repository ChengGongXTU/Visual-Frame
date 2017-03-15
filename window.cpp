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

	//build a model: sqaure.
	//8 Vertex
	Vertex v1 = Vertex(-5.f, 0.f, 20.f, 255, 0, 0);
	Vertex v2 = Vertex(5.f, 0.f, 20.f, 0, 255, 0);
	Vertex v3 = Vertex(-5.f, 10.f, 20.f, 0, 0, 255);
	Vertex v4 = Vertex(5.f, 10.f, 20.f, 255, 0, 0);
	Vertex v5 = Vertex(-5.f, 0.f, 30.f, 255, 0, 0);
	Vertex v6 = Vertex(5.f, 0.f, 30.f, 0, 255, 0);
	Vertex v7 = Vertex(-5.f, 10.f, 30.f, 0, 0, 255);
	Vertex v8 = Vertex(5.f, 10.f, 30.f, 255, 0, 0);

	//12 triangle
	Triangle t1 = Triangle(v1, v2, v3);
	Triangle t2 = Triangle(v2, v3, v4);
	Triangle t3 = Triangle(v2, v4, v6);
	Triangle t4 = Triangle(v4, v6, v8);
	Triangle t5 = Triangle(v1, v2, v5);
	Triangle t6 = Triangle(v2, v5, v6);
	Triangle t7 = Triangle(v1, v3, v5);
	Triangle t8 = Triangle(v3, v5, v7);
	Triangle t9 = Triangle(v3, v4, v7);
	Triangle t10 = Triangle(v4, v7, v8);
	Triangle t11 = Triangle(v5, v6, v7);
	Triangle t12 = Triangle(v6, v7, v8);

	//viewport
	Point eye = Point(-0.f, 30.f,0.f);
	Point gaze = Point(-0.f, 0.f, 30.f);
	Vector upView = Vector(0.f, 1.f, 0.f);
	Camera camera = Camera(eye, gaze, upView, -12.f, 12.f, -9.f, 9.f, 15.f, 2000.f, w, h);

	//transform vertex
	Triangle tri1 = TriangleTransform(t1, camera);
	Triangle tri2 = TriangleTransform(t2, camera);
	Triangle tri3 = TriangleTransform(t3, camera);
	Triangle tri4 = TriangleTransform(t4, camera);
	Triangle tri5 = TriangleTransform(t5, camera);
	Triangle tri6 = TriangleTransform(t6, camera);
	Triangle tri7 = TriangleTransform(t7, camera);
	Triangle tri8 = TriangleTransform(t8, camera);
	Triangle tri9 = TriangleTransform(t9, camera);
	Triangle tri10 = TriangleTransform(t10, camera);
	Triangle tri11 = TriangleTransform(t11, camera);
	Triangle tri12 = TriangleTransform(t12, camera);

	switch (message) {
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		//draw square
		hdc = BeginPaint(hwnd, &ps);
		BresenhamAlgorithm(hdc, tri10);
		BresenhamAlgorithm(hdc, tri9);
		BresenhamAlgorithm(hdc, tri2);
		BresenhamAlgorithm(hdc, tri1);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}