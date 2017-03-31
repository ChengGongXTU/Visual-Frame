#include"texture.h"

enum RefType
{
	Dif, Spe, Mix,Ani,Lum   //Diffuse , Specular, Mix Diffuse and Specular,Luminaire;
};

class Vertex {
public:

	Vertex(){ p.x = p.y = p.z = 0.f; 
	r = g = b = 0.f; alpha = 1.f; 
	er = eg = eb = 0.f;
	pr = pg = pb = 0.f;
	textu = textv = textw = 0.f;
	refType = Dif;
	}

	Vertex(float xx, float yy, float zz, float rr, float gg, float bb,float phong,RefType rt) {
		p.x = xx, p.y = yy, p.z = zz;
		r = rr, g = gg, b = bb;
		er = eg = eb = 0.f;
		pr = pg = pb = phong;
		textu = textv = 0.f;
		refType = rt;
	}

	Vertex(float xx, float yy, float zz, float rr, float gg, float bb, float phong,float e, RefType rt) {
		p.x = xx, p.y = yy, p.z = zz;
		r = rr, g = gg, b = bb;
		er = eg = eb = e;
		pr = pg = pb = phong;
		textu = textv = 0.f;
		refType = rt;
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
	float textu, textv,textw;

	//normal
	Normal n;



	Vertex &operator=(const Vertex &v){
		p = v.p; 
		r = v.r; g = v.g; b = v.b; alpha = v.alpha;
		er = v.er; eg = v.eg; eb = v.eb;
		pr = v.pr; pg = v.pg; pb = v.pb;
		textu = v.textu; textv = v.textv;
		n = v.n;
		refType = v.refType;
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

	//initialization
	Triangle(){}

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

	//normal number
	int nNum;

	//texture vertex number
	int uvNum;

	// pointer to ith triangle's vertex indices
	int *vi;

	//vertex's texture indices
	int* uvi;

	//pointer to an array of vertex's position
	Point *p;

	//pointer to an array of triangles' normal
	Normal *n;

	//vertex normal's indices
	int* ni;

	//pointer to texture per vertex
	float *uv;

	//Diffuse
	float kd[3];

	//Ambient
	float ka[3];

	//texture buffer
	
	Texture* texture;

	//texture flag
	int flag;
	RefType reftype;

	//initialization
	TriangleMesh() {
		triNum = verNum = nNum = uvNum = 0.f;
		vi = NULL;
		p = NULL;
		n = NULL;
		uv = NULL;
		uvi = NULL;
		kd[0] = kd[1] = kd[2] = 0.f;
		ka[0] = ka[1] = ka[2] = 0.0f;
		texture = NULL;
		flag = 0;

	}

	//suface normal
	Vector Nor(int objId) {
		Normal n0 = n[ni[objId * 3]] + n[ni[objId * 3 + 1]] + n[ni[objId * 3 + 2]];
		Vector v0 = Normalize(Vector(n0.x, n0.y, n0.z));
		return v0;
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

float MidPointDistance(float x, float y, const Point &p0, const Point &p1) {
	double f = (p0.y - p1.y)*x + (p1.x - p0.x)*y + p0.x*p1.y - p1.x*p0.y;
	return f;
}


Point Tri3DBarycentric(const Point &p, const Triangle &tri) {
	Vector na = Cross((tri.vc.p - tri.vb.p), (p - tri.vb.p));
	Vector nb = Cross((tri.va.p - tri.vc.p), (p - tri.vc.p));
	Vector nc = Cross((tri.vb.p - tri.va.p), (p - tri.va.p));

	Vector n = Cross((tri.vb.p - tri.va.p), (tri.vc.p - tri.va.p));

	float a = Dot(n, na) / n.LengthSquared();
	float b = Dot(n, nb) / n.LengthSquared();
	float c = Dot(n, nc) / n.LengthSquared();

	return Point(a, b, c);


}



void LoadObj(TriangleMesh &mesh,string fileName) {

	ifstream fl(fileName);
	string line;
	stringstream ss;
	string word;
	vector<float> vp;
	vp.reserve(5000);
	vector<float> vn;
	vn.reserve(5000);
	vector<int> vni;
	vni.reserve(5000);
	vector<int> vtri;
	vtri.reserve(5000);
	vector<float> vuv;
	vuv.reserve(5000);
	vector<int> vuvi;
	vuvi.reserve(5000);

	int pNum = 0, triNum = 0, nNum = 0,vuNum = 0;
	int j, k, l;
	float a, b, c;
	string as, bs, cs;

	//if (!fl)	return false;

	while (getline(fl, line)) {
		ss.clear();
		ss.str(line);
		ss >> word;
		if ((int)word[0] == 118 && (int)word[1] == NULL) {
			pNum++;
			ss >> a >> b >> c;
			vp.push_back(a);
			vp.push_back(b);
			vp.push_back(c);
			continue;
		}

		if ((int)word[0] == 118 && (int)word[1] == 110) {
			nNum++;
			ss >> a >> b >> c;
			vn.push_back(a);
			vn.push_back(b);
			vn.push_back(c);
			continue;
		}

		if ((int)word[0] == 118 && (int)word[1] == 116) {
			vuNum++;
			a = b = c = 0.f;
			ss >> a >> b >> c;
			vuv.push_back(a);
			vuv.push_back(b);
			vuv.push_back(c);
			continue;
		}

		if ((int)word[0] == 102 && (int)word[1] == NULL) {
			triNum++;
			ss >> as >> bs >> cs;
			FindFace(as, j, k, l);
			vtri.push_back(j);
			vuvi.push_back(k);
			vni.push_back(l);

			FindFace(bs, j, k, l);
			vtri.push_back(j);
			vuvi.push_back(k);
			vni.push_back(l);

			FindFace(cs, j, k, l);
			vtri.push_back(j);
			vuvi.push_back(k);
			vni.push_back(l);
			continue;
		}
	}


	//if (pNum == 0 && triNum == 0)	return false;

	mesh.p = new Point[pNum];	
	mesh.n = new Normal[nNum];
	mesh.uv = new float[3*vuNum];

	mesh.vi = new int[3 * triNum];
	mesh.uvi = new int[3 * triNum];
	mesh.ni = new int[3 * triNum];
	
	mesh.verNum = pNum;
	mesh.triNum = triNum;
	mesh.nNum = nNum;
	mesh.uvNum = vuNum;

	//Point
	for (int i = 0; i < pNum; i++) {
		mesh.p[i].x = vp[i * 3];
		mesh.p[i].y = vp[i * 3 + 1];
		mesh.p[i].z = vp[i * 3 + 2];
	}
	vp.clear();
	vector<float>().swap(vp);

	//Normal
	for (int i = 0; i < nNum; i++) {
		mesh.n[i].x = vn[i * 3];
		mesh.n[i].y = vn[i * 3 + 1];
		mesh.n[i].z = vn[i * 3 + 2];
	}
	vn.clear();
	vector<float>().swap(vn);

	//normal's indices
	for (int i = 0; i < triNum; i++) {
		mesh.ni[i * 3] = vni[i * 3];
		mesh.ni[i * 3+1] = vni[i * 3+1];
		mesh.ni[i * 3+2] = vni[i * 3+2];
	}
	vni.clear();
	vector<int>().swap(vni);

	//triangle's veretx indices
	for (int i = 0; i < triNum; i++) {
		mesh.vi[i * 3] = vtri[i * 3];
		mesh.vi[i * 3 + 1] = vtri[i * 3 + 1];
		mesh.vi[i * 3 + 2] = vtri[i * 3 + 2];
	}
	vtri.clear();
	vector<int>().swap(vtri);

	//texture's position
	for (int i = 0; i < vuNum; i++) {
		mesh.uv[i * 3] = vuv[i * 3];
		mesh.uv[i * 3 + 1] = vuv[i * 3 + 1];
		mesh.uv[i * 3 + 2] = vuv[i * 3 + 2];
	}
	vuv.clear();
	vector<float>().swap(vuv);

	//triangle's vertex texture indices
	for (int i = 0; i < triNum; i++) {
		mesh.uvi[i * 3] = vuvi[i * 3];
		mesh.uvi[i * 3 + 1] = vuvi[i * 3 + 1];
		mesh.uvi[i * 3 + 2] = vuvi[i * 3 + 2];
	}
	vuvi.clear();
	vector<int>().swap(vuvi);


	fl.clear();
	fl.close();
	ss.clear();
	
}

bool BindTexture(Texture*tx, TriangleMesh &mesh) {
	mesh.texture = tx;
	mesh.flag = 1;
	if (mesh.texture == NULL) return false;
	else return true;
}


float Get2DTexture(TriangleMesh &mesh,float textu,float textv ,int k) {
	int i = int(textu*(mesh.texture->bmpW-1));
	int j = int(textv*(mesh.texture->bmpH-1));
	if (i < 0)	i = mesh.texture->bmpW + i;
	if (j < 0)	j = mesh.texture->bmpH + j;
	if (i > mesh.texture->bmpW)	i = i % mesh.texture->bmpW;
	if (j > mesh.texture->bmpH)	j = j % mesh.texture->bmpH;
	auto color = mesh.texture->tb[j*mesh.texture->lineByte + i*mesh.texture->pBitCount / 8 + k];
	return ((unsigned int)color) / 255.f;
}
void SetCartoonStyle(int i,TriangleMesh &mesh){
	if (i == 1) mesh.flag = 2;
}