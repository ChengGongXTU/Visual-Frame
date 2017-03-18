#include"render.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd) {

	//initialize buffer
	for (int i = 0; i < w*h; i++) {
		zbuffer[i] = -1.f;
		pbuffer[4 * i] = (BYTE)255;
		pbuffer[4 * i + 1] = (BYTE)backcolor[0];
		pbuffer[4 * i + 2] = (BYTE)backcolor[1];
		pbuffer[4 * i + 3] = (BYTE)backcolor[2];
	}

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

	//build a model: sqaure.
	//8 Vertex
	float phong = 5.f;
	Vertex v1 = Vertex(-15.f, -15.f, 40.f,0.8,0.1,0.1, phong);
	Vertex v2 = Vertex(15.f, -15.f, 40.f, 0.8, 0.1, 0.1, phong);
	Vertex v3 = Vertex(-15.f, 15.f, 40.f, 0.8, 0.1, 0.1, phong);
	Vertex v4 = Vertex(15.f, 15.f, 40.f, 0.8, 0.1, 0.1, phong);
	Vertex v5 = Vertex(-15.f, -15.f, 70.f, 0.8, 0.1, 0.1, phong);
	Vertex v6 = Vertex(15.f, -15.f, 70.f, 0.8, 0.1, 0.1, phong);
	Vertex v7 = Vertex(-15.f, 15.f, 70.f, 0.8, 0.1, 0.1, phong);
	Vertex v8 = Vertex(15.f, 15.f, 70.f, 0.8, 0.1, 0.1, phong);
	Vertex v9 = Vertex(-40.f, -20.f, 20.f, 0.8, 0.1, 0.1, phong);
	Vertex v10 = Vertex(-40.f, -20.f, 90.f, 0.8, 0.1, 0.1, phong);
	Vertex v11 = Vertex(40.f, -20.f, 20.f, 0.5, 0.1, 0.5, 1.f);
	Vertex v12 = Vertex(40.f, -20.f, 90.f, 0.5, 0.1, 0.5, 1.f);

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
	Triangle t13 = Triangle(v9, v10, v11);
	Triangle t14 = Triangle(v10, v11, v12);

	//viewport
	Point eye = Point(-70, 70.f,0.f);
	Point gaze = Point(0.f, 0.f, 40.f);
	Vector upView = Vector(0.f, 1.f, 0.f);
	PointLight pl = PointLight(-100,100,0.f,4,1.f,0.1);
	PointLight pl2 = PointLight(100,100,0.f, 32,1.f,0.1);
	Camera camera = Camera(eye, gaze, upView, -32.f,32.f,-24.f,24.f, -30.f, -3000.f, w, h);

	switch (message) {
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		//draw square
		hdc = BeginPaint(hwnd, &ps);
		RenderPipeline(t1, camera, pl);
		RenderPipeline(t2, camera, pl);
		RenderPipeline(t3, camera, pl );
		RenderPipeline(t4, camera, pl );
		RenderPipeline(t5, camera, pl );
		RenderPipeline(t6, camera, pl );
		RenderPipeline(t7, camera, pl );
		RenderPipeline(t8, camera, pl );
		RenderPipeline(t9, camera, pl );
		RenderPipeline(t10, camera, pl);
		RenderPipeline(t11, camera, pl);
		RenderPipeline(t12, camera, pl);
		RenderPipeline(t13, camera, pl2);
		RenderPipeline(t14, camera, pl2);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		delete zbuffer;
		PostQuitMessage(0);
		return 0;
		
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}