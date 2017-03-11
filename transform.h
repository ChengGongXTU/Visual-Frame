#include"geometry.h"

struct Matrix4x4 {

	Matrix4x4() {
		m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
		m[0][1] = m[0][2] = m[0][3] = 0.f;
		m[1][0] = m[1][2] = m[1][3] = 0.f;
		m[2][0] = m[2][1] = m[2][3] = 0.f;
		m[3][0] = m[3][1] = m[3][2] = 0.f;
	}

	Matrix4x4(float mat[4][4]) {
		memcpy(m, mat, 16 * sizeof(float));
	}

	Matrix4x4(float t00, float t01, float t02, float t03,
		float t10, float t11, float t12, float t13,
		float t20, float t21, float t22, float t23,
		float t30, float t31, float t32, float t33) {
		m[0][0] = t00; m[0][1] = t01; m[0][2] = t02; m[0][3] = t03;
		m[1][0] = t10; m[1][1] = t11; m[1][2] = t12; m[1][3] = t13;
		m[2][0] = t20; m[2][1] = t21; m[2][2] = t22; m[2][3] = t23;
		m[3][0] = t30; m[3][1] = t31; m[3][2] = t32; m[3][3] = t33;
	}

	//矩阵m1右乘m2
	static Matrix4x4 Mul(const Matrix4x4 &m1, const Matrix4x4 &m2) {    
		Matrix4x4 r;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				r.m[i][j] = m1.m[i][0] * m2.m[0][j] +
				m1.m[i][1] * m2.m[1][j] +
				m1.m[i][2] * m2.m[2][j] +
				m1.m[i][3] * m2.m[3][j];
		return r;
	}

	//矩阵转置
	friend Matrix4x4 Transpose(const Matrix4x4 &m);
	
	//矩阵求逆
	friend Matrix4x4 Inverse(const Matrix4x4 &);
	

	//两个矩阵是否相同
	bool operator==(const Matrix4x4 &m2) const {					
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				if (m[i][j] != m2.m[i][j]) return false;
		return true;
	}

	//两个矩阵是否不同
	bool operator!=(const Matrix4x4 &m2) const {
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				if (m[i][j] != m2.m[i][j]) return true;
		return false;
	}

	//matrix's float element
	float m[4][4];
};





class Transform {
public:
	//---------初始化：Matrix4x4结构中已赋默认值-----------
	Transform() {}

	//赋值
	Transform(const float mat[4][4]) {
		m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
			mat[1][0], mat[1][1], mat[1][2], mat[1][3],
			mat[2][0], mat[2][1], mat[2][2], mat[2][3],
			mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
		mInv = Inverse(m);
	}
	
	//赋值
	Transform(const Matrix4x4 &mat)
		:m(mat), mInv(Inverse(mat)) {
	}

	//给矩阵和逆矩阵赋值
	Transform(const Matrix4x4 &mat, const Matrix4x4 &minv)
		:m(mat), mInv(minv) {
	}

	//矩阵和逆矩阵相互交换
	friend Transform Inverse(const Transform &t) {
		return Transform(t.mInv, t.m);
	}

	//平移变换T和它的逆变换（平移向量delta）
	friend Transform Translate(const Vector &delta);

	//比例变换S
	friend Transform Scale(float x, float y, float z);

	//旋转变换X,Y,Z轴
	friend Transform RotateX(float angle);
	friend Transform RotateY(float angle);
	friend Transform RotateZ(float angle);

	//任意旋转轴的旋转变换R
	friend Transform Rotate(float angle, const Vector &axis);   

	//look-at变换(camera变换项)
	friend Transform LookAt(const Point &pos, const Point &look, const Vector &up);


	//点坐标变换，得到新点
	inline Point operator()(const Point &pt) const;

	//点坐标变换，结果返回给ptrans指针
	inline void operator()(const Point &pt, Point *ptrans)const;

	//向量变换（向量的齐次权重w=0，可不考虑wp）
	inline Vector operator()(const Vector &v) const;

	//向量变换，结果返回给vtrans指针
	inline void operator()(const Vector &v, Vector *vtrans)const;

	//法线转换矩阵，需要列向量左乘转置的逆转换矩阵
	inline Normal operator()(const Normal &n)const;

	 //矩阵右乘t2，用于多重转换的矩阵推导
	inline Transform operator*(const Transform &t2)const;       

public:

	//transform private data
	Matrix4x4 m, mInv;

	//四元数
	friend struct Quaternion;
};



inline Point Transform::operator()(const Point &pt)const {             
	float x = pt.x, y = pt.y, z = pt.z;
	float xp = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
	float yp = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
	float zp = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
	float wp = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];
	if (wp == 1.) return Point(xp, yp, zp);
	else return Point(xp, yp, zp) / wp;
}


inline void Transform::operator()(const Point &pt, Point *ptrans)const {  
	float x = pt.x, y = pt.y, z = pt.z;
	ptrans->x = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
	ptrans->y = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3];
	ptrans->z = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3];
	float w = m.m[3][0] * x + m.m[3][1] * y + m.m[3][2] * z + m.m[3][3];
	if (w != 1.) *ptrans /= w;
}


inline Vector Transform::operator()(const Vector &v)const {                
	float x = v.x, y = v.y, z = v.z;
	return Vector(m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z,
		m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z,
		m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z);
}


inline void Transform::operator()(const Vector &v, Vector *vtrans)const {
	float x = v.x, y = v.y, z = v.z;
	vtrans->x = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z;
	vtrans->y = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z;
	vtrans->z = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z;
}


inline Normal Transform::operator()(const Normal &n)const {                
	float x = n.x, y = n.y, z = n.z;
	return Normal(mInv.m[0][0] * x + mInv.m[1][0] * y + mInv.m[2][0] * z,
		mInv.m[0][1] * x + mInv.m[1][1] * y + mInv.m[2][1] * z,
		mInv.m[0][2] * x + mInv.m[1][2] * y + mInv.m[2][2] * z);
}


inline Transform Transform::operator*(const Transform &t2)const {      
	Matrix4x4 m1 = Matrix4x4::Mul(m, t2.m);
	Matrix4x4 m2 = Matrix4x4::Mul(t2.mInv, mInv);
	return Transform(m1, m2);
}


Matrix4x4 Transpose(const Matrix4x4 &m) {
	return Matrix4x4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
		m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
		m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
		m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
}

Matrix4x4 Inverse(const Matrix4x4 &m) {
	return Matrix4x4(m);
}

Transform Translate(const Vector &delta) {
	Matrix4x4 m(1, 0, 0, delta.x,
		0, 1, 0, delta.y,
		0, 0, 1, delta.z,
		0, 0, 0, 1);
	Matrix4x4 minv(1, 0, 0, -delta.x,
		0, 1, 0, -delta.y,
		0, 0, 1, -delta.z,
		0, 0, 0, 1);
	return Transform(m, minv);
}


Transform Scale(float x, float y, float z) {
	Matrix4x4 m(x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);
	Matrix4x4 minv(1.f / x, 0, 0, 0,
		0, 1.f / y, 0, 0,
		0, 0, 1.f / z, 0,
		0, 0, 0, 1);
	return Transform(m, minv);
}

Transform RotateX(float angle) {
	float sin_t = sinf(Radians(angle));
	float cos_t = cosf(Radians(angle));
	Matrix4x4 m(1, 0, 0, 0,
		0, cos_t, -sin_t, 0,
		0, sin_t, cos_t, 0,
		0, 0, 0, 1);
	return Transform(m, Transpose(m));
}

Transform RotateY(float angle) {
	float sin_t = sinf(Radians(angle));
	float cos_t = cosf(Radians(angle));
	Matrix4x4 m(cos_t, 0, sin_t, 0,
		0, 1, 0, 0,
		-sin_t, 0, cos_t, 0,
		0, 0, 0, 1);
	return Transform(m, Transpose(m));
}

Transform RotateZ(float angle) {
	float sin_t = sinf(Radians(angle));
	float cos_t = cosf(Radians(angle));
	Matrix4x4 m(cos_t, -sin_t, 0, 0,
		sin_t, cos_t, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	return Transform(m, Transpose(m));
}

Transform Rotate(float angle, const Vector &axis) {
	Vector a = Normalize(axis);
	float s = sinf(Radians(angle));
	float c = cosf(Radians(angle));
	float m[4][4];

	m[0][0] = a.x * a.x + (1.f - a.x * a.x) * c;
	m[0][1] = a.x * a.y * (1.f - c) - a.z * s;
	m[0][2] = a.x * a.z * (1.f - c) + a.y * s;
	m[0][3] = 0;

	m[1][0] = a.x * a.y * (1.f - c) + a.z * s;
	m[1][1] = a.y * a.y + (1.f - a.y * a.y) * c;
	m[1][2] = a.y * a.z * (1.f - c) - a.x * s;
	m[1][3] = 0;

	m[2][0] = a.x * a.z * (1.f - c) - a.y * s;
	m[2][1] = a.y * a.z * (1.f - c) + a.x * s;
	m[2][2] = a.z * a.z + (1.f - a.z * a.z) * c;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;

	Matrix4x4 mat(m);
	return Transform(mat, Transpose(mat));
}

Transform LookAt(const Point &eye, const Point &gaze, const Vector &up) {
	Vector xaxis, yaxis, zaxis;
	Matrix4x4 m;

	zaxis = gaze - eye;
	zaxis = Normalize(zaxis);

	xaxis = Cross(zaxis, up);
	xaxis = Normalize(xaxis);

	yaxis = Cross(zaxis, xaxis);

	m.m[0][0] = xaxis.x;
	m.m[1][0] = xaxis.y;
	m.m[2][0] = xaxis.z;

	m.m[0][1] = yaxis.x;
	m.m[1][1] = yaxis.y;
	m.m[2][1] = yaxis.z;

	m.m[0][2] = zaxis.x;
	m.m[1][2] = zaxis.y;
	m.m[2][2] = zaxis.z;

	m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;

	m.m[0][3] = -eye.x;
	m.m[1][3] = -eye.y;
	m.m[2][3] = -eye.z;

	return Transform(m, Inverse(m));
}