#include"ray.h"


class PointLight {
public:
	//Light Origin
	Point o;

	//diffuse 
	float dif[3];

	//reflect
	float ref[3];
	int refp;

	//ambient
	float amb[3];

	//luminance
	float lum[3];

	PointLight() { dif[0] = dif[1] = dif[2] = 1.f; ref[0] = ref[1] = ref[2] = 1.f; refp = 1; }

	PointLight(float xx, float yy, float zz) {
		o.x = xx; o.y = yy; o.z = zz;
	}

	PointLight(float xx, float yy, float zz,float a,float d,float r,float p,float l) {
		o.x = xx; o.y = yy; o.z = zz;
		dif[0] = dif[1] = dif[2] = d; ref[0] = ref[1] = ref[2] = r; refp = p;
		amb[0] = amb[1] = amb[2] = a; lum[0] = lum[1] = lum[2] = l;
	}
};

void LightCompute( Vertex *v, const PointLight &l,const Point &eye) {

	float cosA = Dot(Normalize(v->n), Normalize(v->p - l.o));
	Vector h = Normalize((v->p - eye) + (v->p - l.o));	
	float cosB = Dot(Normalize(v->n), h);

	v->r = v->r* (l.amb[0] +l.dif[0] *l.lum[0] * max(0.f, cosA) )+ 255*l.lum[0] * l.ref[0] * pow(max(0.f, cosB), l.refp);
	v->g = v->g* (l.amb[1] +l.dif[1] * l.lum[1] * max(0.f, cosA) )+ 255*l.lum[1] * l.ref[1] * pow(max(0.f, cosB), l.refp);
	v->b = v->b* (l.amb[2] + l.dif[2] * l.lum[2] * max(0.f, cosA) )+ 255*l.lum[2] * l.ref[2] * pow(max(0.f, cosB), l.refp);

}
