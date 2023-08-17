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
		{-0.45f,0.41f,0.0f},
		{1.0f,0.58f,0.0f},
		WHITE
	};

	
	Sphere Sphere1
	{
		{0,0,0,},
		0.6f,
		WHITE
		
	};


	Plane plane
	{
		{0.0f,1.0f,0.0f},
		1.0f,
		WHITE
	};

	AABB aabb1;
	AABB aabb2;

	aabb1.min = { -0.5f,-0.5f,-0.5f };
	aabb1.max = { 0.0f,0.0f,0.0f };
	aabb1.color = WHITE;

	aabb2.min = { 0.2f,0.2f,0.2f };
	aabb2.max = { 1.0f,1.0f,1.0f };
	aabb2.color = WHITE;

	Triangle triangle;
	triangle.vertices[0] = { -1.0f,0.0f,0.0f };
	triangle.vertices[1] = { 0.0f,1.0f,0.0f };
	triangle.vertices[2] = { 1.0f,0.0f,0.0f };

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


		//minとmaxが入れ替わらないように
		aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
		aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);



		//当たり判定
		if (IsCollision(aabb1,Sphere1) == true)
		{
			aabb1.color = RED;
		}
		else
		{
			aabb1.color = WHITE;
		}
		
		
		

		ImGui::Begin("Window");

		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
	
		ImGui::Text("sphere");
		ImGui::DragFloat3("sphere1Center", &Sphere1.center.x, 0.01f);
		ImGui::DragFloat("sphere1Radius", &Sphere1.radius, 0.01f);

		ImGui::Text("AABB");
		ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.01f);
		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.01f);
		ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.01f);
		ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.01f);

		/*ImGui::Text("triangle");
		ImGui::DragFloat3("triangle.v0", &triangle.vertices[0].x, 0.01f);
		ImGui::DragFloat3("triangle.v1", &triangle.vertices[1].x, 0.01f);
		ImGui::DragFloat3("triangle.v2", &triangle.vertices[2].x, 0.01f);

		ImGui::Text("line");
		ImGui::DragFloat3("Segment.Origine", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Segment.Diff", &segment.diff.x, 0.01f);
		*/
		/*ImGui::Text("plane");
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		plane.normal = Normalize(plane.normal);
		ImGui::DragFloat("Plane.Distance", &plane.distance, 0.01f);
		*/
		
		ImGui::End();



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


		//グリッド線
		DrawGrid(ViewProjectionMatirx, viewportMatrix);

		DrawAABB(aabb1, ViewProjectionMatirx, viewportMatrix, aabb1.color);

		

		////線分
		//Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), segment.color);



		////三角形
		//DrawTriangle(triangle, ViewProjectionMatirx, viewportMatrix, WHITE);



		////平面
		//DrawPlane(plane, ViewProjectionMatirx, viewportMatrix);

		//球
		DrawSphere(Sphere1, ViewProjectionMatirx, viewportMatrix, Sphere1.color);


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