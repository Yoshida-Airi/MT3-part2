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

	Sphere sphere
	{
		{0,0,0,},
		0.5f
	};

	Segment segment
	{
		{-2.0f,-1.0f,0.0f},
		{3.0f,2.0f,2.0f}
	};

	Vector3 point
	{
		-1.5f,0.6f,0.6f
	};

	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	Vector3 closestPoint = ClosestPoint(point, segment);

	//1cmの球を描画
	Sphere pointSphere
	{
		point,0.01f
	};

	Sphere ClosestPointSphere
	{
		closestPoint,0.01f
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


		//線分
		Vector3 start = Transform(Transform(segment.origin, ViewProjectionMatirx), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), ViewProjectionMatirx), viewportMatrix);
	
	

		ImGui::Begin("Window");
		/*ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);*/
		ImGui::DragFloat3("point", &point.x, 0.01f);

		ImGui::InputFloat3("Project", &project.x, "% .3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::End();



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		//グリッド線
		DrawGrid(ViewProjectionMatirx, viewportMatrix);


		//線分
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);



		//球
		DrawSphere(pointSphere, ViewProjectionMatirx, viewportMatrix, RED);
		DrawSphere(ClosestPointSphere, ViewProjectionMatirx, viewportMatrix, BLACK);


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