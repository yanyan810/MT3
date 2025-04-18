#include <Novice.h>
#include <math.h>
#include <assert.h>
const char kWindowTitle[] = "LE2B_25_ミヤザワハルヒ_MT3";

static const int kRowHeight = 20;
static const int kClownWidth = 60;

typedef struct Vector3 {
	float x;
	float y;
	float z;
}Vector3;

//4次元マトリックス
typedef struct Matrix4x4 {
	float m[4][4];
}Matrix4x4;

//1.平行移動行列
Matrix4x4 MakeTranslationMatrix(const Vector3& translate) {

	Matrix4x4 result;

	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;

};

//2.拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;
	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
};

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

void VectorScreenPrintf(int x, int y, const Vector3& v, const char* label) {
	Novice::ScreenPrintf(x + kClownWidth * 4, y, "%s", label);
	Novice::ScreenPrintf(x + kClownWidth, y, "%6.02f", v.x);
	Novice::ScreenPrintf(x + kClownWidth * 2, y, "%6.02f", v.y);
	Novice::ScreenPrintf(x + kClownWidth * 3, y, "%6.02f", v.z);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& m, const char* label) {
	Novice::ScreenPrintf(x, y + 20, "%s", label);
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			Novice::ScreenPrintf(x + column * kClownWidth, y + 40 + row * kRowHeight, "%6.02f", m.m[row][column]);
		}
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };


	Vector3 translate{ 4.1f, 2.6f, 0.8f };

	Vector3 scale{ 1.5f,5.2f,7.3f };

	Matrix4x4 translationMatrix = MakeTranslationMatrix(translate);

	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Vector3 point{ 2.3f,3.8f,1.4f };

	Matrix4x4 transformMatrix = {
		1.0f,2.0f,3.0f,4.0f,
		3.0f,1.0f,1.0f,2.0f,
		1.0f,4.0f,2.0f,3.0f,
		2.0f,2.0f,1.0f,3.0f
	};

	Vector3 transformed = Transform(point, transformMatrix);

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

		VectorScreenPrintf(0, 0, transformed, "transformd");

		MatrixScreenPrintf(0, 20, translationMatrix, "translationMatrix");

		MatrixScreenPrintf(0, kRowHeight * 6, scaleMatrix, "scaleMatrix");

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
