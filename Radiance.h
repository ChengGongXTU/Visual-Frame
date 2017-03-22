#include"render.h"

struct Ls{
	float r, g, b;

	Ls() { r = g = b = 0.f; }

	Ls(float x, float y, float z) { r = x, g = y, b = z; }

	Ls operator+(const Ls ls) const {
		return Ls(r + ls.r, g + ls.g, b + ls.b);
	}

	Ls operator*(const Ls ls)const {
		return Ls(r * ls.r, g * ls.g, b * ls.b);
	}

	Ls operator*(const float t) const {
		return Ls(r*t, g*t, b*t);
	}
};


//search nearest ray-intersected triangle, update Ray's t and triId;
bool IntersectionSearch(Ray &r,Triangle* tri, int triNum) {

	float mint = INFINITY;
	int minId = 0;
	int sect = 0;

	for (int i = 0; i < triNum; i++) {
		if (intersection(r, tri[i])) {
			sect = 1;
			if (r.t < mint) {
				mint = r.t;
				minId = i;
			}
		}			
	}

	r.t = mint;
	r.objId = minId;

	if (sect == 0) return FALSE;
	else return TRUE;
}

Ls Radiance(Ray &r, int depth, Triangle* tri, int triNum, int sampleNum) {

	//find ray-intersetcion triangle's id and ray's t;
	if (!IntersectionSearch(r, tri, triNum))
	{
		//if not intersect, return background
		return Ls(backcolor[0], backcolor[1], backcolor[2]);
	}
		//if intersected, ray trace time increases. 
	else r.depth++;

	//build this triangle's reference
	Triangle &tri1 = tri[r.objId];

	//intersection 's position
	Vertex v = Vertex(r.o + r.t*r.d);

	//intersection 's normal
	if(Dot(tri1.n,-r.d)>0)
		v.n = tri1.n;
	else
		v.n = -tri1.n;

	//intersection's barycemtric coordinate
	Point bary = Tri3DBarycentric(v.p, tri1);
	float a = bary.x;
	float b = bary.y;
	float c = bary.z;

	//intersection's radiance parameter
	//diffuse reflection

	v.p = a*tri1.va.p + b*tri1.vb.p + c*tri1.vc.p;

	v.r = a*tri1.va.r + b*tri1.vb.r + c*tri1.vc.r;
	v.g = a*tri1.va.g + b*tri1.vb.g + c*tri1.vc.g;
	v.b = a*tri1.va.b + b*tri1.vb.b + c*tri1.vc.b;

	//phong reflection
	v.pr = a*tri1.va.pr + b*tri1.vb.pr + c*tri1.vc.pr;
	v.pg = a*tri1.va.pg + b*tri1.vb.pg + c*tri1.vc.pg;
	v.pb = a*tri1.va.pb + b*tri1.vb.pb + c*tri1.vc.pb;

	//emission
	v.er = a*tri1.va.er + b*tri1.vb.er + c*tri1.vc.er;
	v.eg = a*tri1.va.eg + b*tri1.vb.eg + c*tri1.vc.eg;
	v.eb = a*tri1.va.eb + b*tri1.vb.eb + c*tri1.vc.eb;
	
	//material's type
	v.refType = tri1.va.refType;
	float p = max(v.r, max(v.g, v.b));

	//if ray trace time beyound depth, use Russian roulette to instead of "only returning zero". 
	if (r.depth > depth) {
		
		if (dis(generator) < p) {
			v.r = v.r*(1.f / p);
			v.g = v.g*(1.f / p);
			v.r = v.b*(1.f / p);
		}
		else return Ls(v.r, v.g, v.b);
	}


	//if hit a luminaire, stop reflecting

	if (v.refType == Dif) {

		//compute random numbers
		double r1 = 2 * M_PI*dis(generator);
		double r2 = dis(generator);
		float r2a = sqrtf(r2);
		float r2b = sqrtf(1 - r2);

		//compute coodinate at intersection
		Vector w = Normalize(Vector(v.n.x, v.n.y, v.n.z));
		Vector u = Normalize(tri1.vb.p - tri1.va.p);
		Vector v1 = Cross(w, u);

		//compute random direction
		Vector d = cos(r1)*r2a*u + sin(r1)*r2a*v1 + r2b*w;

		//if (Dot(d, r.d) > 0) d = -d;

		Ray rt = Ray(v.p, d, 0, INFINITY, INFINITY);
		rt.t0 = 0.001f;
		rt.t = 0.f;
		rt.depth = r.depth;
		rt.objId = r.objId;

		return Ls(v.er, v.eg, v.eb) + Ls(v.r, v.g, v.b)*Radiance(rt, depth, tri, triNum, sampleNum);

	}
}