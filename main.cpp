#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "LE2B_25_ミヤザワハルヒ_MT3";

static const int kRowHeight = 20;
static const int kClownWidth = 60;


//4次元マトリックス
typedef struct Matrix4x4 {
	float m[4][4];
}Matrix4x4;

struct Vector3 {
	float x;
	float y;
	float z;
};

//1.行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}

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

//5.転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m.m[j][i];
			result.m[i][j] = m.m[j][i];
			result.m[i][j] = m.m[j][i];
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}


//6.単位行列の作成
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				result.m[i][j] = 1.0f;
			} else {
				result.m[i][j] = 0.0f;
			}
		}
	}
	return result;
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& m, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			Novice::ScreenPrintf(x + column * kClownWidth, y + 20 + row * kRowHeight, "%6.02f", m.m[row][column]);
		}
	}
}

//==================================================
// 確認課題00-04
//==================================================

//1.X軸回転行列
Matrix4x4 MakeRotationXMatrix(float radian) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	return result;
}

//2.Y軸回転行列
Matrix4x4 MakeRotationYMatrix(float radian) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[0][0] = std::cos(radian);
	result.m[0][2] = -std::sin(radian);
	result.m[2][0] = std::sin(radian);
	result.m[2][2] = std::cos(radian);
	return result;
}
//3.Z軸回転行列
Matrix4x4 MakeRotationZMatrix(float radian) {
	Matrix4x4 result = MakeIdentity4x4();
	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	return result;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 rotate{ 0.4f,1.43f,-0.8f };
	Matrix4x4 rotateXMatrix = MakeRotationXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotationYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotationZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

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


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, rotateXMatrix, "rotateXMatrix");

		MatrixScreenPrintf(0, kRowHeight * 5, rotateYMatrix, "rotateYMatrix");

		MatrixScreenPrintf(0, kRowHeight * 5 * 2, rotateZMatrix, "rotateZMatrix");

		MatrixScreenPrintf(0, kRowHeight * 5 * 3, rotateXYZMatrix, "rotateXYZMatrix");


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
