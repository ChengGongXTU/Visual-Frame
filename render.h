#include"light.h"

class Box {
public:
	Point minPoint, maxPoint;

	Box() {}

	Box(const Point &p1, const Point &p2) {
		minPoint = Point(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z));
		maxPoint = Point(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z));
	}

	friend Box Union(const Box &b, const Point &p);
	friend Box Union(const Box &b, const Box &b2);

};

class DIBColor {
public:

	unsigned int  rgb[3];
	float alpha;

	DIBColor() { rgb[0] = rgb[1] = rgb[2] = 0,alpha = 1.f; }
};

Vertex VertexTransform(const Vertex &v,const Camera &camera) {
	Vertex newV;
	newV = v;
	newV.p = camera.worldToEye(v.p);
	newV.p = camera.eyeToPer(newV.p);
	newV.p = camera.proToView(newV.p);
	return newV;
}

Triangle TriangleTransform(const Triangle &tri,const Camera &camera) {
	Triangle newTri = tri;
	newTri.va = VertexTransform(tri.va,camera);
	newTri.vb = VertexTransform(tri.vb, camera);
	newTri.vc = VertexTransform(tri.vc, camera);
	newTri.n = Cross((newTri.va.p - newTri.vb.p), (newTri.va.p - newTri.vc.p));
	newTri.n = Normalize(newTri.n);
	return newTri;
}


Box Union(const Box &b, const Point &p) {
	Box ret = b;
	ret.minPoint.x = min(b.minPoint.x, p.x);
	ret.minPoint.y = min(b.minPoint.y, p.y);
	ret.minPoint.z = min(b.minPoint.z, p.z);
	ret.maxPoint.x = max(b.maxPoint.x, p.x);
	ret.maxPoint.y = max(b.maxPoint.y, p.y);
	ret.maxPoint.z = max(b.maxPoint.z, p.z);
	return ret;
}

Box Union(const Box &b, const Box &b2) {
	Box ret;
	ret.minPoint.x = min(b.minPoint.x, b2.minPoint.x);
	ret.minPoint.y = min(b.minPoint.y, b2.minPoint.y);
	ret.minPoint.z = min(b.minPoint.z, b2.minPoint.z);
	ret.maxPoint.x = max(b.maxPoint.x, b2.maxPoint.x);
	ret.maxPoint.y = max(b.maxPoint.y, b2.maxPoint.y);
	ret.maxPoint.z = max(b.maxPoint.z, b2.maxPoint.z);
	return ret;
}

void CutBox(const Camera &camera, Box *ret) {
	if (ret->minPoint.x < 0) ret->minPoint.x = 0;
	if (ret->minPoint.y < 0) ret->minPoint.y = 0;
	if (ret->minPoint.z < 0) ret->minPoint.y = 0;
}


//void PainterAlgorithm(Device *device,const Triangle &tri, const Camera &camera) {
//	DIBColor color;
//	Box ret(tri.va.p, tri.vb.p);
//	ret = Union(ret, tri.vc.p);
//
//	for (int j = (int)ret.minPoint.y; j < ret.maxPoint.y; j++)
//		for (int i = (int)ret.minPoint.x; i < ret.maxPoint.x; i++) {
//			float a = BarycentricFij(i, j, tri.vb.p, tri.vc.p) / BarycentricFij(tri.va.p.x, tri.va.p.y, tri.vb.p, tri.vc.p);
//			float b = BarycentricFij(i, j, tri.vc.p, tri.va.p) / BarycentricFij(tri.vb.p.x, tri.vb.p.y, tri.vc.p, tri.va.p);
//			float c = BarycentricFij(i, j, tri.va.p, tri.vb.p) / BarycentricFij(tri.vc.p.x, tri.vc.p.y, tri.va.p, tri.vb.p);
//
//			if (a > 0 && b > 0 && c > 0) {
//				color.rgb[0] = (unsigned int)(tri.va.r*a + tri.vb.r*b + tri.vc.r*c);
//				color.rgb[1] = (unsigned int)(tri.va.g*a + tri.vb.g*b + tri.vc.g*c);
//				color.rgb[2] = (unsigned int)(tri.va.b*a + tri.vb.b*b + tri.vc.b*c);
//				color.alpha = tri.va.alpha*a + tri.vb.alpha*b + tri.vc.alpha*c;
//
//				*(device->pBuffer + j + i * 4 +1) =color.rgb[2];
//				*(device->pBuffer+j+i*4+2) = color.rgb[1];
//				*(device->pBuffer+j + i * 4 + 3) = color.rgb[0];
//				//HDC hdc1 = BeginPaint(hwnd, &ps);
//				//for (int i = 0; i < cxClient; i++)
//				//for (int j = 0; j < cyClient; j++) {
//				//		SetPixel(hdc1, i, j, RGB(color.rgb[0], color.rgb[1], color.rgb[2]));
//				//	}
//			}
//			else
//				color.alpha = 0;
//
//			//device->pBuffer[j + i * 4 + 1] = (char)((unsigned int)(device->pBuffer[j + i * 4 + 1])*(1 - color.alpha) + color.rgb[2] * color.alpha);
//			//device->pBuffer[j + i * 4 + 2] = (char)((unsigned int)(device->pBuffer[j + i * 4 + 2])*(1 - color.alpha) + color.rgb[1] * color.alpha);
//			//device->pBuffer[j + i * 4 + 3] = (char)((unsigned int)(device->pBuffer[j + i * 4 + 3])*(1 - color.alpha) + color.rgb[0] * color.alpha);
//		}
//}


void BresenhamAlgorithm(HDC hdc, const Triangle &tri,const PointLight &pl,const Point &eye) {

	Box b = Box(tri.va.p, tri.vb.p);
	Box c = Union(b, tri.vc.p);
	Vertex vt;

	int x0, y0, x1, y1;
	x0 = floor(c.minPoint.x);
	y0 = floor(c.minPoint.y);
	x1 = ceil(c.maxPoint.x);
	y1 = ceil(c.maxPoint.y);

	for (int i = x0; i < x1; i++)
		for (int j = y0; j < y1; j++) {
			float a = MidPointDistance(i, j, tri.vb.p, tri.vc.p) / MidPointDistance(tri.va.p.x, tri.va.p.y, tri.vb.p, tri.vc.p);
			float b = MidPointDistance(i, j, tri.vc.p, tri.va.p) / MidPointDistance(tri.vb.p.x, tri.vb.p.y, tri.vc.p, tri.va.p);
			float c = MidPointDistance(i, j, tri.va.p, tri.vb.p) / MidPointDistance(tri.vc.p.x, tri.vc.p.y, tri.va.p, tri.vb.p);

			if (a > 0 && b > 0 && c > 0) {
				Point p = a*tri.va.p + b*tri.vb.p + c*tri.vc.p;
					
				if (p.z > zbuffer[i*w + j] && p.z<1.f&&p.z>-1.f) {
					zbuffer[i*w + j] = p.z;					
					int rp = a*tri.va.r + b*tri.vb.r + c*tri.vc.r;
					int gp = a*tri.va.g + b*tri.vb.g + c*tri.vc.g;
					int bp = a*tri.va.b + b*tri.vb.b + c*tri.vc.b;
					vt = Vertex(p.x, p.y, p.z, rp, gp, bp);
					vt.n = tri.n;
					if (Dot(vt.n, vt.p-eye) > 0) vt.n = -vt.n;

					LightCompute(&vt, pl, eye);

					SetPixel(hdc, i, h-j, RGB(vt.r,vt.g,vt.b));
				}					
			}				
		}

}

void RenderPipeline(Triangle &t, Camera &camera, PointLight &pl) {

	Triangle tri = TriangleTransform(t, camera);

	Box b = Box(tri.va.p, tri.vb.p);
	Box c = Union(b, tri.vc.p);
	Vertex vt;

	int x0, y0, x1, y1;
	x0 = floor(c.minPoint.x);
	x0 = max(0, x0);
	y0 = floor(c.minPoint.y);
	y0 = max(0, y0);
	x1 = ceil(c.maxPoint.x);
	x1 = min(w, x1);
	y1 = ceil(c.maxPoint.y);
	y1 = min(h, y1);

	for (int i = x0; i < x1; i++)
		for (int j = y0; j < y1; j++) {
			float a = MidPointDistance(i, j, tri.vb.p, tri.vc.p) / MidPointDistance(tri.va.p.x, tri.va.p.y, tri.vb.p, tri.vc.p);
			float b = MidPointDistance(i, j, tri.vc.p, tri.va.p) / MidPointDistance(tri.vb.p.x, tri.vb.p.y, tri.vc.p, tri.va.p);
			float c = MidPointDistance(i, j, tri.va.p, tri.vb.p) / MidPointDistance(tri.vc.p.x, tri.vc.p.y, tri.va.p, tri.vb.p);

			if (a > 0 && b > 0 && c > 0) {
				Point p = a*t.va.p + b*t.vb.p + c*t.vc.p;
				float z = a*tri.va.p.z + b*tri.vb.p.z + c*tri.vc.p.z;
				if (z > zbuffer[i*w + j] && z<1.f&&z>-1.f) {
					zbuffer[i*w + j] = z;
					float rp = a*tri.va.r + b*tri.vb.r + c*tri.vc.r;
					float gp = a*tri.va.g + b*tri.vb.g + c*tri.vc.g;
					float bp = a*tri.va.b + b*tri.vb.b + c*tri.vc.b;

					vt = Vertex(p.x, p.y, p.z, rp, gp, bp);
					vt.n = t.n;

					if (Dot(vt.n, p - camera.eye) < 0) vt.n = -vt.n;
					LightCompute(&vt, pl, camera.eye);
					vt.r = min(vt.r, 255);
					vt.g = min(vt.g, 255);
					vt.b = min(vt.b, 255);
					SetPixel(hdc, i, h - j, RGB(vt.r, vt.g, vt.b));
				}
			}
		}
}




