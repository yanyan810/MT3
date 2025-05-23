#include <Novice.h>
#include <cmath>
#include <assert.h>
#include <windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <imgui.h>

const char kWindowTitle[] = "LE2B_25_ミヤザワハルヒ_MT3";

static const int kRowHeight = 20;
static const int kClownWidth = 60;

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

typedef struct Vector3 {
	float x;
	float y;
	float z;
}Vector3;

//4次元マトリックス
typedef struct Matrix4x4 {
	float m[4][4];
}Matrix4x4;

struct Sphere {
	Vector3 center;//!中心点
	float radius; //!<半径
};

struct Line {
	Vector3 origin;//!<始点
	Vector3 diff;  //<!終点への差分ベクトル
};

struct Ray {

	Vector3 origin;
	Vector3 diff;
};

struct Segment {

	Vector3 origin;
	Vector3 diff;

};

struct Plane {
	Vector3 normal;//!<法線
	float distance;//<!距離

};

struct Triangle {
	Vector3 vertices[3];//!<頂点
};

//1.透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspcectRatio, float nearClip, float farClip) {
	Matrix4x4 result;
	result.m[0][0] = 1 / aspcectRatio * (1 / std::tan(fovY / 2));
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1 / std::tan(fovY / 2);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = -(farClip * nearClip) / (farClip - nearClip);
	result.m[3][3] = 0.0f;

	return result;
}

//正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {

	Matrix4x4 result;
	result.m[0][0] = 2.0f / (right - left);
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f / (farClip - nearClip);
	result.m[2][3] = 0.0f;

	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1.0f;

	return result;

}

//ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {

	Matrix4x4 result;
	result.m[0][0] = width / 2;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = -height / 2;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[2][3] = 0.0f;

	result.m[3][0] = left + width / 2;
	result.m[3][1] = top + height / 2;
	result.m[3][2] = minDepth;
	result.m[3][3] = 1.0f;

	return result;

}

//ベクトルの加法
Vector3 VectorAdd(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

//2.行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

//ベクトルの減法
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

//3.行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			result.m[i][j] = m1.m[i][0] * m2.m[0][j] +
				m1.m[i][1] * m2.m[1][j] +
				m1.m[i][2] * m2.m[2][j] +
				m1.m[i][3] * m2.m[3][j];

		}
	}
	return result;
}

Vector3 Multiply(const Matrix4x4& m, const Vector3& v) {
	Vector3 result;
	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3];
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3];
	result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3];
	return result;
}

Vector3 Multiply(float distance, const Vector3 v2) {
	Vector3 result;
	result.x = distance * v2.x;
	result.y = distance * v2.y;
	result.z = distance * v2.z;
	return result;
}

//4.逆行列
Matrix4x4 inverse(const Matrix4x4& m) {

	float det =
		m.m[0][0] * (m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) -
			m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) +
			m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1])) -
		m.m[0][1] * (m.m[1][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) -
			m.m[1][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) +
			m.m[1][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0])) +
		m.m[0][2] * (m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) -
			m.m[1][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) +
			m.m[1][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0])) -
		m.m[0][3] * (m.m[1][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) -
			m.m[1][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) +
			m.m[1][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));

	float invDet = 1.0f / det;

	Matrix4x4 result;

	result.m[0][0] = invDet * (m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) -
		m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) +
		m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]));

	result.m[0][1] = -invDet * (m.m[0][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) -
		m.m[0][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) +
		m.m[0][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]));

	result.m[0][2] = invDet * (m.m[0][1] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) -
		m.m[0][2] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) +
		m.m[0][3] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1]));

	result.m[0][3] = -invDet * (m.m[0][1] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) -
		m.m[0][2] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) +
		m.m[0][3] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]));

	result.m[1][0] = -invDet * (m.m[1][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) -
		m.m[1][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) +
		m.m[1][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]));

	result.m[1][1] = invDet * (m.m[0][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) -
		m.m[0][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) +
		m.m[0][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]));

	result.m[1][2] = -invDet * (m.m[0][0] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) -
		m.m[0][2] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) +
		m.m[0][3] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0]));

	result.m[1][3] = -invDet * (m.m[0][0] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) -
		m.m[0][2] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) +
		m.m[0][3] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]));

	result.m[2][0] = invDet * (m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) -
		m.m[1][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) +
		m.m[1][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));

	result.m[2][1] = -invDet * (m.m[0][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) -
		m.m[0][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) +
		m.m[0][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));

	result.m[2][2] = invDet * (m.m[0][0] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) -
		m.m[0][1] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) +
		m.m[0][3] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0]));

	result.m[2][3] = -invDet * (m.m[0][0] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) -
		m.m[0][1] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) +
		m.m[0][3] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]));


	result.m[3][0] = -invDet * (m.m[1][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) -
		m.m[1][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) +
		m.m[1][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));

	result.m[3][1] = invDet * (m.m[0][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) -
		m.m[0][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) +
		m.m[0][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));

	result.m[3][2] = -invDet * (m.m[0][0] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1]) -
		m.m[0][1] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0]) +
		m.m[0][2] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0]));

	result.m[3][3] = invDet * (m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) -
		m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) +
		m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]));



	return result;
}

//3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 result;

	result.m[0][0] = scale.x * cosf(rotate.y) * cosf(rotate.z);
	result.m[0][1] = scale.x * cosf(rotate.y) * sinf(rotate.z);
	result.m[0][2] = -scale.x * sinf(rotate.y);
	result.m[0][3] = 0.0f;

	result.m[1][0] = scale.y * (sinf(rotate.x) * sinf(rotate.y) * cosf(rotate.z) - cosf(rotate.x) * sinf(rotate.z));
	result.m[1][1] = scale.y * (sinf(rotate.x) * sinf(rotate.y) * sinf(rotate.z) + cosf(rotate.x) * cosf(rotate.z));
	result.m[1][2] = scale.y * (sinf(rotate.x) * cosf(rotate.y));
	result.m[1][3] = 0.0f;

	result.m[2][0] = scale.z * (cosf(rotate.x) * sinf(rotate.y) * cosf(rotate.z) + sinf(rotate.x) * sinf(rotate.z));
	result.m[2][1] = scale.z * (cosf(rotate.x) * sinf(rotate.y) * sinf(rotate.z) - sinf(rotate.x) * cosf(rotate.z));
	result.m[2][2] = scale.z * (cosf(rotate.x) * cosf(rotate.y));
	result.m[2][3] = 0.0f;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
}

//3.座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];

	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

//クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}


//正規化
Vector3 Normalize(const Vector3& vector) {
	Vector3 result;
	float length = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	if (length == 0.0f) {
		return { 0.0f, 0.0f, 0.0f };
	}
	result.x = vector.x / length;
	result.y = vector.y / length;
	result.z = vector.z / length;
	return result;
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& m, const char* label) {
	Novice::ScreenPrintf(x, y + 20, "%s", label);
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			Novice::ScreenPrintf(x + column * kClownWidth, y + 40 + row * kRowHeight, "%6.02f", m.m[row][column]);
		}
	}
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {

	Novice::ScreenPrintf(x, y, "%0.2f", vector.x);
	Novice::ScreenPrintf(x + kClownWidth, y, "%0.2f", vector.y);
	Novice::ScreenPrintf(x + kClownWidth * 2, y, "%0.2f", vector.z);
	Novice::ScreenPrintf(x + kClownWidth * 3, y, "%s", label);

}

//Gridを表示
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	// 奥から手前への線（X方向固定）
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		float x = -kGridHalfWidth + xIndex * kGridEvery;

		Vector3 start = { x, 0.0f, -kGridHalfWidth };
		Vector3 end = { x, 0.0f, kGridHalfWidth };

		Vector3 ndcStart = Transform(start, viewProjectionMatrix);
		Vector3 ndcEnd = Transform(end, viewProjectionMatrix);

		Vector3 screenStart = Transform(ndcStart, viewportMatrix);
		Vector3 screenEnd = Transform(ndcEnd, viewportMatrix);

		uint32_t color = (x == 0.0f) ? 0xFF0000FF : 0xAAAAAAFF;  // 中心線は赤
		Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), color);
	}

	// 左から右（Z方向固定）
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float z = -kGridHalfWidth + zIndex * kGridEvery;

		Vector3 start = { -kGridHalfWidth, 0.0f, z };
		Vector3 end = { kGridHalfWidth, 0.0f, z };

		Vector3 ndcStart = Transform(start, viewProjectionMatrix);
		Vector3 ndcEnd = Transform(end, viewProjectionMatrix);

		Vector3 screenStart = Transform(ndcStart, viewportMatrix);
		Vector3 screenEnd = Transform(ndcEnd, viewportMatrix);

		uint32_t color = (z == 0.0f) ? 0x0000FFFF : 0xAAAAAAFF;  // 中心線は青
		Novice::DrawLine(int(screenStart.x), int(screenStart.y), int(screenEnd.x), int(screenEnd.y), color);
	}
}


void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 8;
	const float kLatEvery = float(M_PI) / float(kSubdivision);  // π / 32
	const float kLonEvery = float(M_PI * 2) / float(kSubdivision);  // 2π / 32

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex;
		float latNext = lat + kLatEvery;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			float lonNext = lon + kLonEvery;

			// 頂点計算
			Vector3 a{
				sphere.center.x + sphere.radius * cosf(lat) * cosf(lon),
				sphere.center.y + sphere.radius * sinf(lat),
				sphere.center.z + sphere.radius * cosf(lat) * sinf(lon)
			};
			Vector3 b{
				sphere.center.x + sphere.radius * cosf(latNext) * cosf(lon),
				sphere.center.y + sphere.radius * sinf(latNext),
				sphere.center.z + sphere.radius * cosf(latNext) * sinf(lon)
			};
			Vector3 c{
				sphere.center.x + sphere.radius * cosf(lat) * cosf(lonNext),
				sphere.center.y + sphere.radius * sinf(lat),
				sphere.center.z + sphere.radius * cosf(lat) * sinf(lonNext)
			};

			// 座標変換
			a = Transform(Transform(a, viewProjectionMatrix), viewportMatrix);
			b = Transform(Transform(b, viewProjectionMatrix), viewportMatrix);
			c = Transform(Transform(c, viewProjectionMatrix), viewportMatrix);

			// 描画
			Novice::DrawLine(int(a.x), int(a.y), int(b.x), int(b.y), color);
			Novice::DrawLine(int(a.x), int(a.y), int(c.x), int(c.y), color);
		}
	}
}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float dotVV = v2.x * v2.x + v2.y * v2.y + v2.z * v2.z;
	float dotPV = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	float scale = dotPV / dotVV;

	Vector3 result;
	result.x = v2.x * scale;
	result.y = v2.y * scale;
	result.z = v2.z * scale;
	return result;
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 segStart = segment.origin;
	Vector3 segEnd = {
		segment.origin.x + segment.diff.x,
		segment.origin.y + segment.diff.y,
		segment.origin.z + segment.diff.z
	};

	Vector3 segDir = Subtract(segEnd, segStart);
	Vector3 ptToStart = Subtract(point, segStart);

	float segLengthSq =
		segDir.x * segDir.x +
		segDir.y * segDir.y +
		segDir.z * segDir.z;

	float dot = ptToStart.x * segDir.x + ptToStart.y * segDir.y + ptToStart.z * segDir.z;
	float t = dot / segLengthSq;

	if (t < 0.0f) t = 0.0f;
	if (t > 1.0f) t = 1.0f;

	Vector3 result = {
		segStart.x + segDir.x * t,
		segStart.y + segDir.y * t,
		segStart.z + segDir.z * t
	};

	return result;
}

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x, 0.0f };
	}
	return { 0.0f, -vector.z, vector.y };
}

//平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = Multiply(plane.distance, plane.normal);

	// perpendiculars を作る（元の構成を保つ）
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	// points を順回りで構成（+u+v, -u+v, -u-v, +u-v）
	Vector3 points[4];
	points[0] = VectorAdd(center, VectorAdd(Multiply(2.0f, perpendiculars[0]), Multiply(2.0f, perpendiculars[2])));   // +u +v
	points[1] = VectorAdd(center, VectorAdd(Multiply(-2.0f, perpendiculars[0]), Multiply(2.0f, perpendiculars[2])));  // -u +v
	points[2] = VectorAdd(center, VectorAdd(Multiply(-2.0f, perpendiculars[0]), Multiply(-2.0f, perpendiculars[2]))); // -u -v
	points[3] = VectorAdd(center, VectorAdd(Multiply(2.0f, perpendiculars[0]), Multiply(-2.0f, perpendiculars[2])));  // +u -v

	//スクリーン座標へ変換
	for (int i = 0; i < 4; i++) {
		points[i] = Transform(Transform(points[i], viewProjectMatrix), viewportMatrix);
	}
	// DrawLine で順回りに矩形を描く（元の構造維持）
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);



}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 screenVertices[3];
	for (int i = 0; i < 3; i++) {
		Vector3 ndcVertex = Transform(triangle.vertices[i], viewProjectionMatrix);
		screenVertices[i] = Transform(ndcVertex, viewportMatrix);
	}

	// 塗りつぶしなし（線描画）
	Novice::DrawTriangle(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		color, kFillModeWireFrame
	);


}

bool IsCollision(const Sphere& s1, const Sphere& s2) {
	Vector3 diff = Subtract(s1.center, s2.center);
	float distanceSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
	float radiusSum = s1.radius + s2.radius;
	return distanceSq <= radiusSum * radiusSum;
}

bool IsCollision(const Sphere& sphere, const Plane& plane) {
	float dot = sphere.center.x * plane.normal.x +
		sphere.center.y * plane.normal.y +
		sphere.center.z * plane.normal.z;
	float distance = fabsf(dot - plane.distance);  // ← N・P - d
	return distance <= sphere.radius;
}

bool IsCollision(const Segment& segment, const Plane& plane) {
	Vector3 segStart = segment.origin;
	Vector3 segEnd = VectorAdd(segment.origin, segment.diff);
	Vector3 segDir = Subtract(segEnd, segStart);

	float dot = segDir.x * plane.normal.x + segDir.y * plane.normal.y + segDir.z * plane.normal.z;
	if (dot == 0.0f) return false; // 平行

	float t = (plane.distance - (segStart.x * plane.normal.x + segStart.y * plane.normal.y + segStart.z * plane.normal.z)) / dot;
	if (t < 0.0f || t > 1.0f) return false; // 線分の範囲外

	// 交点を求める
	Vector3 intersection = {
		segStart.x + segDir.x * t,
		segStart.y + segDir.y * t,
		segStart.z + segDir.z * t
	};

	// 平面の中心位置
	Vector3 center = Multiply(plane.distance, plane.normal);

	// 平面の座標軸（u, v）
	Vector3 u = Normalize(Perpendicular(plane.normal));
	Vector3 v = Normalize(Cross(plane.normal, u));

	// 中心→交点 のベクトルを作る
	Vector3 local = Subtract(intersection, center);

	// u, v 方向の成分を内積で求める
	float uDist = local.x * u.x + local.y * u.y + local.z * u.z;
	float vDist = local.x * v.x + local.y * v.y + local.z * v.z;

	// 範囲内かどうか（ここでは矩形サイズが±2）
	float halfSize = 2.0f;
	return fabsf(uDist) <= halfSize && fabsf(vDist) <= halfSize;
}

bool IsCollision(const Triangle& triangle, const Segment& segment) {
	const Vector3& orig = segment.origin;
	float segLength = sqrtf(segment.diff.x * segment.diff.x +
		segment.diff.y * segment.diff.y +
		segment.diff.z * segment.diff.z);
	if (segLength < 1e-6f) return false;

	Vector3 dir = {
		segment.diff.x / segLength,
		segment.diff.y / segLength,
		segment.diff.z / segLength
	};

	const Vector3& v0 = triangle.vertices[0];
	const Vector3& v1 = triangle.vertices[1];
	const Vector3& v2 = triangle.vertices[2];

	Vector3 edge1 = Subtract(v1, v0);
	Vector3 edge2 = Subtract(v2, v0);

	Vector3 pvec = Cross(dir, edge2);
	float det = edge1.x * pvec.x + edge1.y * pvec.y + edge1.z * pvec.z;

	if (fabs(det) < 1e-6f) return false;

	float invDet = 1.0f / det;

	Vector3 tvec = Subtract(orig, v0);
	float u = (tvec.x * pvec.x + tvec.y * pvec.y + tvec.z * pvec.z) * invDet;
	if (u < 0.0f || u > 1.0f) return false;

	Vector3 qvec = Cross(tvec, edge1);
	float v = (dir.x * qvec.x + dir.y * qvec.y + dir.z * qvec.z) * invDet;
	if (v < 0.0f || u + v > 1.0f) return false;

	float t = (edge2.x * qvec.x + edge2.y * qvec.y + edge2.z * qvec.z) * invDet;

	// 長さで比較（Normalize済みdirを使っているため）
	return (t >= 0.0f && t <= segLength);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	/*Vector3 v1{ 1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f };
	Vector3 cross = Cross(v1, v2);*/

	Vector3 cameraRotate{ 0.0f,0.0f,0.0f };
	Vector3 cameraTranslate{ 0.0f,-1.5f,1.0f };
	Vector3 cameraPosition{ 0.0f,0.0f,-10.000f };

	const Vector3 kLocalVertices[3] = {
  {0.0f, 1.0f, 0.0f},    // 頂点1
  {-1.0f, -1.0f, 0.0f},  // 頂点2
  {1.0f, -1.0f, 0.0f}    // 頂点3
	};

	Sphere sphere;
	sphere.center = { 0,0,0 };
	sphere.radius = 1.0f;

	Sphere sphere2;
	sphere2.center = { 0.1f,0,3 };
	sphere2.radius = 1.0f;

	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
	Vector3 point{ -1.0f,0.6f,0.6f };

	//int sphereColor = WHITE;

	int segmentColor = WHITE;

	int triangleColor = WHITE;

	Plane plane;
	plane.normal = { 0.0f,1.0f,0.0f };
	plane.distance = 1.0f;

	Triangle triangle = { {
		{-1.0f,0.0f,0.0f},
	    {0.0f,1.0f,0.0f},
	    {1.0f,0.0f,0.0f},
	}
	};

	//マウスの座標
	int mouseX = 0;
	int mouseY = 0;

	//マウスの前の位置
	int prevMouseX = 0;
	int prevMouseY = 0;

	//マウスの位置を中央に
	SetCursorPos(kWindowWidth / 2, kWindowHeight / 2);

	//カメラ移動の速度
	float speed = 0.1f;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		segmentColor = WHITE;

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 1.0f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatriix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		//============
		//マウスの処理
		//============

		Novice::GetMousePosition(&mouseX, &mouseY);

		//Rキーでマウスの位置をリセット
		if (keys[DIK_R]) {
			SetCursorPos(kWindowWidth / 2, kWindowHeight / 2);
		}

		int dx = mouseX - prevMouseX;
		int dy = mouseY - prevMouseY;

		//押している間移動
		if (keys[DIK_LSHIFT]) {
			if (Novice::IsPressMouse(1) || Novice::IsPressMouse(0)) {

				cameraRotate.y += dx * 0.01f;//左右回転
				cameraRotate.x += dy * 0.01f;//上下回転
			}
		}
		prevMouseX = mouseX;
		prevMouseY = mouseY;

		//===========
		//カメラ移動
		//===========

		if (keys[DIK_W]) {
			cameraTranslate.z -= speed;
		}

		if (keys[DIK_S]) {
			cameraTranslate.z += speed;
		}

		if (keys[DIK_A]) {
			cameraTranslate.x += speed;
		}

		if (keys[DIK_D]) {
			cameraTranslate.x -= speed;
		}

		if (keys[DIK_UP]) {
			cameraTranslate.y -= speed;
		}
		if (keys[DIK_DOWN]) {
			cameraTranslate.y += speed;
		}

		//当たり判定
		/*if (IsCollision(sphere, sphere2)) {
			sphereColor = RED;
		} else {
			sphereColor = WHITE;
		}*/

		/*	if (IsCollision(sphere, plane)) {
				sphereColor = RED;
			} else {
				sphereColor = WHITE;
			}*/

	/*	if (IsCollision(segment, plane)) {
			segmentColor = RED;
		} else {
			segmentColor = WHITE;
		}*/

	/*	if (IsCollision(segment, plane)) {
			triangleColor = RED;
		} else {
			triangleColor = WHITE;
		}*/

		if (IsCollision(triangle,segment)) {
			triangleColor = RED;
		} else {
			triangleColor = WHITE;
		}

		Vector3  project = Project(Subtract(point, segment.origin), segment.diff);
		Vector3 closestPoint = ClosestPoint(point, segment);


		Sphere pointSphere{ point,0.01f };
		Sphere closestPointSphere{ closestPoint,0.01f };

		Vector3 start = Transform(Transform(segment.origin, worldViewProjectionMatrix), viewportMatriix);
		Vector3 end = Transform(Transform(VectorAdd(segment.origin, segment.diff), worldViewProjectionMatrix), viewportMatriix);

		/*Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; i++) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatriix);
		}*/

		/*if (keys[DIK_W]) {
			cameraTranslate.z -= 0.1f;
		}

		if (keys[DIK_S]) {
			cameraTranslate.z += 0.1f;
		}

		if (keys[DIK_A]) {
			cameraTranslate.x = 0.1f;

		}

		if (keys[DIK_D]) {
			cameraTranslate.x -= 0.1f;

		}*/

		//rotate.y += 0.05f;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//VectorScreenPrintf(0, 0, cross, "Cross");



		DrawGrid(worldViewProjectionMatrix, viewportMatriix);
		/*DrawSphere(sphere, worldViewProjectionMatrix, viewportMatriix, sphereColor);
		DrawSphere(sphere2, worldViewProjectionMatrix, viewportMatriix, WHITE);
		DrawSphere(pointSphere, worldViewProjectionMatrix, viewportMatriix, RED);
		DrawSphere(closestPointSphere, worldViewProjectionMatrix, viewportMatriix, BLACK);*/
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), segmentColor);
		//DrawPlane(plane, worldViewProjectionMatrix, viewportMatriix, WHITE);
		DrawTriangle(triangle, worldViewProjectionMatrix, viewportMatriix, triangleColor);
		ImGui::Begin("Window");
		if (ImGui::CollapsingHeader("Camera")) {
			ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
			ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		}

		if (ImGui::CollapsingHeader("Sphere")) {
			ImGui::DragFloat3("SphereCenter", &sphere.center.x, 0.01f);
			ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);
		}

		if (ImGui::CollapsingHeader("Plane")) {
			if (ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f)) {
				plane.normal = Normalize(plane.normal);
			}
			ImGui::DragFloat("distance", &plane.distance, 0.01f);
		}

		if (ImGui::CollapsingHeader("Segment")) {
			ImGui::DragFloat3("Segment.Origin", &segment.origin.x, 0.01f);
			ImGui::DragFloat3("Segment.Diff", &segment.diff.x, 0.01f);
		}

		if (ImGui::CollapsingHeader("Triangle")) {
			ImGui::DragFloat3("Triangle.Vertex1", &triangle.vertices[0].x, 0.01f);
			ImGui::DragFloat3("Triangle.Vertex2", &triangle.vertices[1].x, 0.01f);
			ImGui::DragFloat3("Triangle.Vertex3", &triangle.vertices[2].x, 0.01f);
		}

		ImGui::End();


		/*Novice::DrawTriangle(
			int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y),
			int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid
		);*/
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
