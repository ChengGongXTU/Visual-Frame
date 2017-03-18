#include"device.h"

class Ray {
public:
	//origin
	Point o;
	
	//direction
	Vector d;

	//max, min and variable t
	float t0, t1;
	float t;

	//the intersection's depth
	float depth;

	//initialization
	Ray() {
		t0 = 0.f; t1 = INFINITY; t = 0.f; depth = 1.f;
	}

	Ray(const Point &p, const Vector &v, float tmin, float tmax, float tx) {
		o = p; d = Normalize(v); t0 = tmin; t1 = tmax; t = tx;
	}

	//get a point from t
	Point operator()(float t) {
		return (o + d*t);
	}
};


float intersection(const Ray &r, const Triangle &t) {
	
	Vector e1 = t.vb.p - t.va.p;
	Vector e2 = t.vc.p - t.va.p;
	Vector s1 = Cross(r.d, e2);
	float inv =  Dot(s1, e1);

	if (inv == 0) return 0;

	//barycentric coordinate
	Vector s = r.o - t.va.p;
	float b1 = Dot(s, s1) / inv;

	Vector s2 = Cross(s, e1);
	float b2 = Dot(s2, r.d) / inv;

	if (b1 < 0.f || b2<0.f || (b1 + b2)>1.f) return 0;

	float tx = Dot(s2, e2) / inv;

	if (tx < r.t0 || tx>r.t1)
		return 0;
	else
		return tx;
}


