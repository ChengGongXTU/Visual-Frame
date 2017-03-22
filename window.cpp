#include"pathtrace.h"

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

	//Cornell Box

	float phong = 5.f;
	float red = 0.75,f;
	float green = 0.75f;

	//white floor
	Vertex vf1 = Vertex(552.8f, 0.f, 0.f, 0.75f,0.75f, 0.75f, phong,Dif);
	Vertex vf2 = Vertex(0.f, 0.f, 0.f, 0.75f, 0.75f, 0.75f, phong,Dif);
	Vertex vf3 = Vertex(0.f, 0.f, 559.2f, 0.75f, 0.75f, 0.75f, phong,Dif);
	Vertex vf4 = Vertex(549.6f, 0.0f, 559.2f, 0.75f, 0.75f, 0.75f, phong,Dif);


	//Holes
	//Vertex vh1 = Vertex(130.f, 0.f, 65.f, 0.8, 0.1, 0.1, phong, Dif);
	//Vertex vh2 = Vertex(82.f, 0.f, 225.f, 0.8, 0.1, 0.1, phong, Dif);
	//Vertex vh3 = Vertex(240.f, 0.f, 272.f, 0.8, 0.1, 0.1, phong, Dif);
	//Vertex vh4 = Vertex(290.f, 0.f, 114.f, 0.8, 0.1, 0.1, phong, Dif);

	//Vertex vh5 = Vertex(423.f, 0.f, 247.f, 0.8, 0.1, 0.1, phong, Dif);
	//Vertex vh6 = Vertex(265.f, 0.f, 295.f, 0.8, 0.1, 0.1, phong, Dif);
	//Vertex vh7 = Vertex(314.f, 0.f, 456.f, 0.8, 0.1, 0.1, phong, Dif);
	//Vertex vh8 = Vertex(472.f, 0.f, 406.f, 0.8, 0.1, 0.1, phong, Dif);

	////Light source
	Vertex vli1 = Vertex(343.f, 545.8f, 227.f, 0.75, 0.75, 0.75, phong, 12.f, Dif);
	Vertex vli2 = Vertex(343.f, 545.8f, 332.f, 0.75, 0.75, 0.75, phong, 12.f, Dif);
	Vertex vli3 = Vertex(213.f, 545.8f, 332.f, 0.75, 0.75, 0.75, phong, 12.f, Dif);
	Vertex vli4 = Vertex(213.f, 545.8f, 227.f, 0.75, 0.75, 0.75, phong, 12.f, Dif);

	

	//white ceiling
	Vertex vc1 = Vertex(556.f, 548.8f, 0.f, 0.75f, 0.75f, 0.75f, phong, Dif);
	Vertex vc2 = Vertex(556.f, 548.8f, 559.2f, 0.75f, 0.75f, 0.75f, phong, Dif);
	Vertex vc3 = Vertex(0.f, 548.8f, 559.2f, 0.75f, 0.75f, 0.75f, phong, Dif);
	Vertex vc4 = Vertex(0.f, 548.8f, 0.f, 0.75f, 0.75f, 0.75f, phong, Dif);

	//Holes
	//Vertex vch1 = Vertex(343.f, 548.8f, 227.f, 0.8, 0.1, 0.1, phong, Dif);
	//Vertex vch2 = Vertex(343.f, 548.8f, 332.f, 0.8, 0.1, 0.1, phong, Dif);
	//Vertex vch3 = Vertex(213.f, 548.8f, 332.f, 0.8, 0.1, 0.1, phong, Dif);
	//Vertex vch4 = Vertex(213.f, 548.8f, 227.f, 0.8, 0.1, 0.1, phong, Dif);

	//white back wall
	Vertex vb1 = Vertex(549.6f, 0.f, 559.2f, 0.75f, 0.75f, 0.75f, phong,Dif);
	Vertex vb2 = Vertex(0.f, 0.f, 559.2f, 0.75f, 0.75f, 0.75f, phong ,Dif);
	Vertex vb3 = Vertex(0.f, 548.8f, 559.2f, 0.75f, 0.75f, 0.75f, phong, Dif);
	Vertex vb4 = Vertex(556.f, 548.8f, 559.2f, 0.75f, 0.75f, 0.75f,phong, Dif);

	//red left wall
	Vertex vl1 = Vertex(0.f, 0.f, 559.2f, red, 0.2f, 0.2f, phong, Dif);
	Vertex vl2 = Vertex(0.f, 0.f, 0.f, red, 0.2f, 0.2f, phong, Dif);
	Vertex vl3 = Vertex(0.f, 548.8f, 0.f, red, 0.2f, 0.2f, phong, Dif);
	Vertex vl4 = Vertex(0.f, 548.8f, 559.2f, red, 0.2f, 0.2f, phong, Dif);
	

	//green right wall
	Vertex vr1 = Vertex(552.8f, 0.f, 0.f, 0.2f, green, 0.2f, phong,Dif);
	Vertex vr2 = Vertex(549.6f, 0.f, 559.2f, 0.2f, green, 0.2f, phong, Dif);
	Vertex vr3 = Vertex(556.f, 548.8f, 559.2f, 0.2f, green, 0.2f, phong, Dif);
	Vertex vr4 = Vertex(556.f, 548.8f, 0.f, 0.2f, green, 0.2f, phong, Dif);

	
	//build some triangle for cornell box

	//floor
	Triangle tf1 = Triangle(vf1, vf2, vf3);
	Triangle tf2 = Triangle(vf1, vf3, vf4);

	//light
	Triangle tli1 = Triangle(vli1, vli2, vli3);
	Triangle tli2 = Triangle(vli1, vli3, vli4);

	//ceiling
	Triangle tc1 = Triangle(vc1, vc2, vc3);
	Triangle tc2 = Triangle(vc1, vc3, vc4);

	//back wall
	Triangle tb1 = Triangle(vb1, vb2, vb3);
	Triangle tb2 = Triangle(vb1, vb3, vb4);

	//right wall
	Triangle tr1 = Triangle(vr1, vr2, vr3);
	Triangle tr2 = Triangle(vr1, vr3, vr4);

	//left wall
	Triangle tl1 = Triangle(vl1, vl2, vl3);
	Triangle tl2 = Triangle(vl1, vl3, vl4);


	Triangle tri[12] = { tf1,tf2,tc1,tc2,tb1,tb2,tr1,tr2,tl1,tl2,tli1,tli2};


	//viewport
	Point eye = Point(278.f, 273.f,-800.f+0.035);
	Point gaze = Point(278.f, 273.f, 0.f);
	Vector upView = Vector(0.f, 1.f, 0.f);
	PointLight pl = PointLight(278.f, 273.f, -800.f,4,8.f,0.1);
	//PointLight pl2 = PointLight(100,100,0.f, 32,1.f,0.1);
	Camera camera = Camera(eye, gaze, upView, -0.0125f, 0.0125f,-0.0125f, 0.0125f, -0.035f, -30000.f, w, h);



	switch (message) {
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_PAINT:
		//draw square
		hdc = BeginPaint(hwnd, &ps);
		for (int i = 0; i < 12; i++)
			RenderPipeline(tri[i], camera, pl);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_SPACE)	bool test = PathTrace(w, h,2, camera, tri,12,5,40);

	case WM_DESTROY:
		delete zbuffer;
		PostQuitMessage(0);
		return 0;
		
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}