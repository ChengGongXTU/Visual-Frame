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

Ls InterpolateRadiance(Ray &r, int depth, Triangle* tri, int triNum, int sampleNum) {

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

	if ((v.refType == Dif) || (v.refType == Lum)) {

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

		return Ls(v.er, v.eg, v.eb) + Ls(v.r, v.g, v.b)*InterpolateRadiance(rt, depth, tri, triNum, sampleNum);

	}
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
	Point p0 = Point(r.o + r.t*r.d);
	Vector n0 = Normalize(Cross(tri1.vb.p - tri1.va.p, tri1.vc.p - tri1.va.p));

	//intersection 's normal
	if (Dot(n0, -r.d) < 0)
		n0 = -n0;

	float p = max(tri1.va.r, max(tri1.va.g, tri1.va.b));

	Ls R = Ls(tri1.va.r, tri1.va.g, tri1.va.b);

	//if ray trace time beyound depth, use Russian roulette to instead of "only returning zero". 
	if (r.depth > depth) {

		if (dis(generator) < p) {
			R.r = tri1.va.r*(1.f / p);
			R.g = tri1.vb.g*(1.f / p);
			R.b = tri1.vc.b*(1.f / p);
		}
		else return Ls(tri1.va.er, tri1.va.eg, tri1.va.eb);
	}

	//if hit a luminaire, stop reflecting

	if ((tri1.va.refType == Dif) ||(tri1.va.refType == Lum)) {

		//compute random numbers
		double r1 = 2 * M_PI*dis(generator);
		double r2 = dis(generator);
		float r2a = sqrtf(r2);
		float r2b = sqrtf(1 - r2);

		//compute coodinate at intersection
		Vector w = Normalize(Vector(n0.x, n0.y, n0.z));
		Vector u = Normalize(tri1.vb.p - tri1.va.p);
		Vector v1 = Cross(w, u);

		//compute random direction
		Vector d = cos(r1)*r2a*u + sin(r1)*r2a*v1 + r2b*w;

		//if (Dot(d, r.d) > 0) d = -d;

		Ray rt = Ray(p0, d, 0, INFINITY, INFINITY);
		rt.t0 = 0.001f;
		rt.t = 0.f;
		rt.depth = r.depth;
		rt.objId = r.objId;

		return Ls(tri1.va.er, tri1.va.eg, tri1.va.eb) + R*Radiance(rt, depth, tri, triNum, sampleNum);

	}
}

Ls OneDirectLight(Ray &r, Triangle* tri, int triNum) {

	//find ray-intersetcion triangle's id and ray's t;
	if (!IntersectionSearch(r, tri, triNum))
	{
		//if not intersect, return background
		return Ls(backcolor[0], backcolor[1], backcolor[2]);
	}

	//intersected object
	Triangle &tri1 = tri[r.objId];

	if (tri1.va.refType == Lum)	return Ls(tri1.va.r, tri1.va.g, tri1.va.b);

	//intersection position
	Point p0 = Point(r.o + r.t*r.d);
	
	//triangle's normal
	if (Dot(tri1.n, r.d) > 0) tri1.n = -tri1.n;

	// find a light source
	for (int i = 0; i < triNum;i++)
		if (tri[i].va.refType == Lum) {

			//compute random numbers
			double r1 = dis(generator);
			double r2 = dis(generator);
			
			// find random point in luminaire.
			Point p1 = tri[i].va.p + (tri[i].vb.p - tri[i].va.p)*r1 + (tri[i].vc.p - tri[i].va.p)*r2;

			//return a ray to p0
			Vector d = p1 - p0;
			Vector ki = Normalize(d);
			Ray rt = Ray(p1, -ki, 0.01, INFINITY, INFINITY);

			//intersection check
			if (IntersectionSearch(rt, tri, triNum) && (tri[rt.objId].va.refType == Lum || rt.objId == r.objId)) {
				if (tri1.va.refType == Dif)
				{
					if (Dot(tri[i].n, ki) > 0) 
						tri[i].n = -tri[i].n;
					float inv = 1.f / tri[i].va.er;
					return Ls(tri1.va.r, tri1.va.g, tri1.va.b)*Ls(tri[i].va.er, tri[i].va.eg, tri[i].va.eb)*inv;
				}
			}

			else return Ls(0.f, 0.f, 0.f);
		}
}

Ls IndirectLight(Ray &r, int depth, Triangle* tri, int triNum, int sampleNum) {

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
	Point p0 = Point(r.o + r.t*r.d);
	Vector n0 = Normalize(Cross(tri1.vb.p - tri1.va.p, tri1.vc.p - tri1.va.p));

	//intersection 's normal
	if (Dot(n0, -r.d) < 0)
		n0 = -n0;

	float p = max(tri1.va.r, max(tri1.va.g, tri1.va.b));

	Ls R = Ls(tri1.va.r, tri1.va.g, tri1.va.b);

	//if ray trace time beyound depth, use Russian roulette to instead of "only returning zero". 
	if (r.depth > depth) {

		if (dis(generator) < p) {
			R.r = tri1.va.r*(1.f / p);
			R.g = tri1.vb.g*(1.f / p);
			R.b = tri1.vc.b*(1.f / p);
		}
		else return Ls(tri1.va.er, tri1.va.eg, tri1.va.eb);
	}

	//if hit a luminaire, stop reflecting

	if ((tri1.va.refType == Dif) || (tri1.va.refType == Lum)) {

		//compute random numbers
		double r1 = 2 * M_PI*dis(generator);
		double r2 = dis(generator);
		float r2a = sqrtf(r2);
		float r2b = sqrtf(1 - r2);

		//compute coodinate at intersection
		Vector w = Normalize(Vector(n0.x, n0.y, n0.z));
		Vector u = Normalize(tri1.vb.p - tri1.va.p);
		Vector v1 = Cross(w, u);

		//compute random direction
		Vector d = cos(r1)*r2a*u + sin(r1)*r2a*v1 + r2b*w;

		//if (Dot(d, r.d) > 0) d = -d;

		Ray rt = Ray(p0, d, 0, INFINITY, INFINITY);
		rt.t0 = 0.001f;
		rt.t = 0.f;
		rt.depth = r.depth;
		rt.objId = r.objId;

		return Ls(tri1.va.er, tri1.va.eg, tri1.va.eb) + R*Radiance(rt, depth, tri, triNum, sampleNum);

	}
}