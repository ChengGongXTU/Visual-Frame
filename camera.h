#include"transform.h"

class Camera {
public:

	// eye position
	Point eye;
	//gaze position
	Point gaze;
	//up vector
	Vector upView;

	// transform from world to eye ccordinate
	Transform worldToEye;

	//Orthographic view volume
	float oLeft, oRight, oTop, oBottom, oNear, oFar;

	//Transform form eye to orthographic projection coordinate
	Transform eyeToOrth;

	//transform from eye to perspective projection coordinate
	Transform eyeToPer;

	//pixel's number of viewport
	float nx, ny;

	//tranform from projection to viewport coordinate
	Transform proToView;

	//transform form world to viewport coordinate
	Transform orthView;
	Transform perView;

	//compute the world to eye transform
	friend Transform WorldToEye(const Point &eye,const Point &gaze,const Vector &upVeiew);

	//compute eye to orthographic projection transform
	friend Transform EyeToOrth(const float &l,const float &r, const float &b,const float &t,
		const int &n,const int &f);

	//compute eye to perspective transform;
	friend Transform EyeToPer(const float &l,const float &r,const float &b,const float &t,
		const int &n,const int &f);

	//compute  projection canonical to viewport 
	friend Transform ProToView(const int &nx,const int &ny);

	//orthographic transform
	Transform OrthView() const{
		Transform t;
		t = proToView*eyeToOrth*worldToEye;
		return t;
	}

	//perspective transform
	Transform PerView() const {
		Transform t;
		t = proToView*eyeToPer*worldToEye;
		return t;
	}

	//initialization
	Camera() { oLeft = oRight = oBottom = oTop = oNear = oFar = nx = ny = 0.f; };

	Camera(const Point eye, const Point gaze, const Vector upView, const float l, const float r, const float b, const float t,
		const float n, const float f, const int &nx, const  int &ny) {
		worldToEye = WorldToEye(eye, gaze, upView);
		eyeToOrth = EyeToOrth(l, r, b, t, n, f);
		eyeToPer = EyeToPer(l, r, b, t, n, f);
		proToView = ProToView(nx, ny);
		orthView = OrthView();
		perView = PerView();
	};

};

Transform WorldToEye(const Point &eye, const Point &gaze, const Vector &upVeiew) {
	Transform t;
	t = LookAt(eye, gaze, upVeiew);
	return t;
}

Transform EyeToOrth(const float &l, const float &r, const float &b, const float &t,
	const int &n, const int &f) {
	Matrix4x4 m;
	m.m[0][0] = 2.f / (r - l);
	m.m[1][1] = 2.f / (t - b);
	m.m[2][2] = 2.f / (f - n);

	m.m[0][3] = -(r + l) / (float)(r - l);
	m.m[1][3] = -(t + b) / (float)(t - b);
	m.m[2][3] = -(f + n) / (float)(f - n);

	m.m[0][1] = m.m[0][2] = 0.f;
	m.m[1][0] = m.m[1][2] = 0.f;
	m.m[2][0] = m.m[2][1] = 0.f;
	m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.f;
	m.m[3][3] = 1.0f;

	return Transform(m);
}

Transform EyeToPer(const float &l, const float &r, const float &b, const float &t,
	const int &n, const int &f) {
	Matrix4x4 m;
	
	m.m[0][0] = 2*n / (r - l);
	m.m[1][1] = 2*n / (t - b);
	m.m[2][2] = (n + f) / (float)(n - f);

	m.m[0][3] = 0.f;
	m.m[1][3] = 0.f;
	m.m[2][3] = 2 * n*f / (n - f);

	m.m[0][1] = 0.f;
	m.m[0][2] = (l + r) / (float)(r - l);

	m.m[1][0] = 0.f;
	m.m[1][2] = (b + t) / (float)(t - b);
	m.m[2][0] = m.m[2][1] = 0.f;
	m.m[3][0] = m.m[3][1] = 0.f;
	m.m[3][2] = -1.f;
	m.m[3][3] = 0.f;

	return Transform(m);
}

Transform ProToView(const int &nx, const int &ny) {
	Matrix4x4 m;

	m.m[0][0] = nx / 2.f;
	m.m[1][1] = ny / 2.f;
	m.m[2][2] = 1.f;

	m.m[0][3] = (nx - 1) / 2.f;
	m.m[1][3] = (ny - 1) / 2.f;
	m.m[2][3] = 0.f;

	m.m[0][1] = 0.f;
	m.m[0][2] = 0.f;

	m.m[1][0] = 0.f;
	m.m[1][2] = 0.f;
	m.m[2][0] = m.m[2][1] = 0.f;
	m.m[3][0] = m.m[3][1] = 0.f;
	m.m[3][2] = 0.f;
	m.m[3][3] = 1.f;

	return Transform(m);
}