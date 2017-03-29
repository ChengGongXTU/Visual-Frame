#include"ray.h"


class PointLight {
public:
	//Light Origin
	Point o;

	//reflection
	int refp;

	float amb[3];

	//luminance
	float lum[3];

	PointLight() { refp = 1; lum[1] = lum[0] = lum[2] = 1.f; amb[0] = amb[1] = amb[2] = 0.f; }

	PointLight(float xx, float yy, float zz) {
		o.x = xx; o.y = yy; o.z = zz;
		refp = 1; 
		lum[1] = lum[0] = lum[2] = 1.f; 
		amb[0] = amb[1] = amb[2] = 0.f;
	}

	PointLight(float xx, float yy, float zz,float p,float l,float a) {
		o.x = xx; o.y = yy; o.z = zz;
		refp = p;
		amb[0] = amb[1] = amb[2] = a;
		lum[0] = lum[1] = lum[2] = l;
	}
};

void LightCompute( Vertex *v, const PointLight &l,const Point &eye) {

	float cosA = Dot(Normalize(v->n), Normalize(l.o - v->p ));
	if (cosA < 0) cosA = 0;
	Vector h = Normalize(( eye - v->p) + (l.o -v->p ));	
	float cosB = Dot(Normalize(v->n), h);
	if (cosB < 0) cosB = 0;

	v->r = v->r*l.amb[0] + v->er + v->r*l.lum[0] * max(0.f, cosA) + v->pr*l.lum[0] * pow(max(0.f, cosB), l.refp);
	v->g = v->g*l.amb[1] + v->eg + v->g*l.lum[1] * max(0.f, cosA) + v->pg*l.lum[1] * pow(max(0.f, cosB), l.refp);
	v->b = v->b*l.amb[2] + v->eb + v->b*l.lum[2] * max(0.f, cosA) + v->pb*l.lum[2] * pow(max(0.f, cosB), l.refp);

}
