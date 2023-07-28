#include <Novice.h>
#include"MathUtils.h"
#include<imgui.h>

const char kWindowTitle[] = "LE2B_28_ヨシダアイリ_タイトル";



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//カメラの位置
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	//カメラ角度
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	Vector3 rotate{};
	Vector3 translate{};

	Segment segment
	{
		{-2.0f,-1.0f,0.0f},
		{3.0f,2.0f,2.0f}
	};

	
	Sphere Sphere1
	{
		{0,0,0,},
		0.6f,
		WHITE
		
	};

	Sphere Sphere2
	{
		{0,1.0f,1.0f},
		0.4f,
		WHITE
	};



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

		// カメラ設定
		Matrix4x4 worldMatrix = MakeAffinMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffinMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 ViewProjectionMatirx = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		//当たり判定
		if (IsCollision(Sphere1, Sphere2) == true)
		{
			Sphere1.color = RED;
		}
		else
		{
			Sphere1.color = WHITE;
		}
		
		
		

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
	
		ImGui::DragFloat3("sphere1Center", &Sphere1.center.x, 0.01f);
		ImGui::DragFloat("sphere1Radius", &Sphere1.radius, 0.01f);
		ImGui::DragFloat3("sphere2Center", &Sphere2.center.x, 0.01f);
		ImGui::DragFloat("sphere2Radius", &Sphere2.radius, 0.01f);
		
		ImGui::End();



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		//グリッド線
		DrawGrid(ViewProjectionMatirx, viewportMatrix);


		//球
		DrawSphere(Sphere1, ViewProjectionMatirx, viewportMatrix, Sphere1.color);
		DrawSphere(Sphere2, ViewProjectionMatirx, viewportMatrix, Sphere2.color);


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