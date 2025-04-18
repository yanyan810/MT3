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

	Vector3 scale = { 1.2f, 0.79f, -2.1f };
	Vector3 rotate = { 0.4f, 1.43f, -0.8f };
	Vector3 translate = { 2.7f, -4.15f, 1.57f };

	Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);

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

		MatrixScreenPrintf(0, 0, worldMatrix, "worldMatrix");

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
