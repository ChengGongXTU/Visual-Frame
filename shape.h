#include"camera.h"

enum RefType
{
	Dif, Spe, Mix   //Diffuse , Specular, Mix Diffuse and Specular;
};

class Vertex {
public:

	Vertex(){ p.x = p.y = p.z = 0.f; 
	r = g = b = 0.f; alpha = 1.f; 
	er = eg = eb = 0.f;
	pr = pg = pb = 0.f;
	textu = textv = 0.f;
	}

	Vertex(float xx, float yy, float zz, float rr, float gg, float bb,float phong) {
		p.x = xx, p.y = yy, p.z = zz;
		r = rr, g = gg, b = bb;
		er = eg = eb = 0.f;
		pr = pg = pb = phong;
		textu = textv = 0.f;
	}

	Vertex(float xx, float yy, float zz, float rr, float gg, float bb, float p1,float p2,float p3) {
		p.x = xx, p.y = yy, p.z = zz;
		r = rr, g = gg, b = bb;
		pr = p1; pg = p2; pb = p3;
		er = eg = eb = 0.f;
		textu = textv = 0.f;

	}

	Vertex(const Point &p1) {
		p.x = p1.x;
		p.y = p1.y;
		p.z = p1.z;
	}
	//position
	Point p;

	//reflection type
	RefType refType;

	//diffuse reflection (color)
	float r, g, b, alpha;

	//emission (light source)
	float er, eg, eb;

	//Phong reflection (mirror reflection)
	float pr, pg, pb;

	//texture coordinate
	float textu, textv;

	//normal
	Normal n;

	Vertex &operator=(const Vertex &v){
		p = v.p; 
		r = v.r; g = v.g; b = v.b; alpha = v.alpha;
		er = v.er; eg = v.eg; eb = v.eb;
		pr = v.pr; pg = v.pg; pb = v.pb;
		textu = v.textu; textv = v.textv;
		n = v.n;
		return *this;
	}
	
};

class Triangle {
public:
	//vertex of triangle
	Vertex va, vb, vc;

	//center of gravity of triangle shape
	Point center;

	//normal of triangle
	Normal n;

	//Triangle = another Triangle
	Triangle &operator=(const Triangle &tri) {
		va = tri.va; vb = tri.vb; vc = tri.vc;
		center = tri.center;
		n = tri.n;
		return *this;
	}
	
	Triangle(Vertex &v1, Vertex &v2, Vertex &v3) {
		va = v1;
		vb = v2;
		vc = v3;

		center = (v1.p + v2.p + v3.p)*0.3333;

		n.x = Cross(v2.p-v1.p, v3.p-v2.p).x;
		n.y = Cross(v2.p - v1.p, v3.p - v2.p).y;
		n.z = Cross(v2.p - v1.p, v3.p - v2.p).z;

		n = Normalize(n);
	}


	friend class TriangleMesh;

};


class TriangleMesh {
public:

	//number of triangles
	int triNum;

	//number of vertex
	int verNum;

	//pointer to an array of vertex
	Vertex *v;

	//pointer to an array of triangles' normal
	Normal *n;

	//pointer to an array of triangle vertex's index
	int *verIndex;

	//initialization
	TriangleMesh() {
		triNum = verNum = 0.f;
		v = NULL;
		n = NULL;
		verIndex = NULL;
	}

	friend class Triangle;

};


class Sphere {
public:
	//position in world coordinate
	Point p;

	//radius
	float radius;

	//color
	float r, g, b, alpha;

	//mesh
	TriangleMesh mesh;

	//initialization
	Sphere() { r = 0.f; r = g = b = 0.f; alpha = 1.f; }
};

// vertex position line-interpolation
void VertexLineInterp(Vertex *v1, Vertex *v2, float t, Vertex *vt) {
	vt->p.x = (1 - t)*v2->p.x + t*v2->p.x;
	vt->p.y = (1 - t)*v2->p.y + t*v2->p.y;
	vt->p.z = (1 - t)*v2->p.z + t*v2->p.z;
}


//barycentric coordinates in triangle;
float MidPointDistance(int x, int y,const Point &p0,const Point &p1) {
	float f = (p0.y - p1.y)*x + (p1.x - p0.x)*y + p0.x*p1.y - p1.x*p0.y;
	return f;
}





