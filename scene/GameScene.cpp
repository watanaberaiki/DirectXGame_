#include "GameScene.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "math.h"
#include <cassert>
#include <random>
GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {
	//π
	PI = 3.1415;
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//平行移動の乱数範囲の指定
	std::uniform_real_distribution<float> move(-10.0f, 10.0f);
	//回転の乱数範囲の指定
	std::uniform_real_distribution<float> rotate(0.0f, PI);

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();


	//合成用回転行列の宣言
	Matrix4 matRot;
	for (WorldTransform& worldTransform : worldTransforms_) {
		//ワールドトランスフォームの初期化
		worldTransform.Initialize();

		// X,Y,Z方向のスケーリングを設定
		worldTransform.scale_ = {1.0f, 1.0f, 1.0f};
		// X,Y,Z軸周りの回転角を設定
		worldTransform.rotation_ = {rotate(engine), rotate(engine), rotate(engine)};
		// X,Y,Z軸周りの平行移動を設定
		worldTransform.translation_ = {move(engine), move(engine), move(engine)};

		//各回転角を掛け算して合成用回転行列に代入する
		matRot = RotateZ(worldTransform.rotation_.z) *= RotateX(worldTransform.rotation_.x) *=
		  RotateY(worldTransform.rotation_.y);

		//ワールド行列を設定する
		worldTransform.matWorld_ =
		  MatWorld(Scale(worldTransform.scale_), matRot, Translation(worldTransform.translation_));
		//行列の転送
		worldTransform.TransferMatrix();
	}
	

	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Radian(45.0f);
	//アスペクト比を設定
	viewProjection_.aspectRatio = 1.0f;
	//ニアクリップ距離を設定
	viewProjection_.nearZ = 52.0f;
	//ファークリップ距離を設定
	viewProjection_.farZ = 53.0f;
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() 
{

	////Fov変更処理
	////上キーで視野角が広がる
	//if (input_->PushKey(DIK_UP)) {
	//	viewProjection_.fovAngleY += 0.01f;
	//	viewProjection_.fovAngleY = clamp(viewProjection_.fovAngleY,0.01f,PI);
	//} else if (input_->PushKey(DIK_DOWN)) {
	//	viewProjection_.fovAngleY -= 0.01f;
	//	viewProjection_.fovAngleY = clamp(viewProjection_.fovAngleY, 0.01f, PI);
	//}

	////行列の再計算
	//viewProjection_.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50,110);
	debugText_->Printf("forAngleY(Degree):%f",Angle(viewProjection_.fovAngleY));


	//クリップ距離変更処理
	//上下キーでニアクリップを増減
	if (input_->PushKey(DIK_UP)) {
		viewProjection_.nearZ += 0.01;
	} else if (input_->PushKey(DIK_DOWN)) {
		viewProjection_.nearZ -= 0.01;
	}

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグ用表示
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f", viewProjection_.nearZ);


////視点移動処理
//
//	////デバッグカメラの更新
//	//debugCamera_->Update();
//	
//	//視点の移動ベクトル
//	Vector3 move = {0, 0, 0};
//
//	//視点の移動速さ
//	const float kEySpeed = 0.2f;
//
//	//押した方向で移動ベクトルを変更
//	if (input_->PushKey(DIK_W)) {
//		move.z += kEySpeed;
//	}
//	else if (input_->PushKey(DIK_S)) {
//		move.z -= kEySpeed;
//	}
//
//	//視点移動(ベクトルの加算)
//	viewProjection_.eye += move;
//
//	//行列の再計算
//	viewProjection_.UpdateMatrix();
//
	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf("eye:(%f,%f,%f)",viewProjection_.eye.x,viewProjection_.eye.y,viewProjection_.eye.z);
//
//	//注視点移動処理
//	//注視点の零ベクトル
//	Vector3 targetMove = {0, 0, 0};
//
//	//注視点の移動速さ
//	const float kTargetSpeed = 0.2f;
//
//	//押した方向で移動ベクトルを変更
//	if (input_->PushKey(DIK_LEFT)) {
//		targetMove.x -= kTargetSpeed;
//	}else if (input_->PushKey(DIK_RIGHT)) {
//		targetMove.x += kTargetSpeed;
//	}
//
//	//注視点移動(ベクトルの加算)
//	viewProjection_.target += targetMove;
//
//	//行列の再計算
//	viewProjection_.UpdateMatrix();
//
	//デバッグ用表示
	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,viewProjection_.target.z);
//
//	//上方向回転処理
//
//	//上方向の回転速さ[ラジアン/frame]
//	const float kUpRotSpeed = 0.05f;
//
//	//押した方向で移動ベクトルを変更
//	if (input_->PushKey(DIK_SPACE)) {
//		viewAngle += kUpRotSpeed;
//		//2πを超えたら0に戻す
//		viewAngle = fmodf(viewAngle, PI * 2.0f);
//	}
//
//	//上方向ベクトルを計算(半径1の円周上の座標)
//	viewProjection_.up = {cosf(viewAngle), sinf(viewAngle), 0.0f};
//
//	//行列の再計算
//	viewProjection_.UpdateMatrix();
//
	//デバッグ用表示
	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデル描画
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

Matrix4 GameScene::Scale(Vector3 worldTransform_scale_) {

	//単位行列を代入する
	Matrix4 matScale = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	                    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	//スケーリング倍率を行列に設定する
	matScale.m[0][0] = worldTransform_scale_.x;
	matScale.m[1][1] = worldTransform_scale_.y;
	matScale.m[2][2] = worldTransform_scale_.z;

	return matScale;
}

Matrix4 GameScene::RotateX(float worldTransform_rotation_x) {
	// X軸回転行列を宣言して単位行列を代入する
	Matrix4 matRotX = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	                   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	// X軸回転行列に回転角を設定する
	matRotX.m[1][1] = cos(worldTransform_rotation_x);
	matRotX.m[1][2] = sin(worldTransform_rotation_x);
	matRotX.m[2][1] = -sin(worldTransform_rotation_x);
	matRotX.m[2][2] = cos(worldTransform_rotation_x);

	return matRotX;
}

Matrix4 GameScene::RotateY(float worldTransform_rotation_y) {
	// Y軸回転行列を宣言して単位行列を代入する
	Matrix4 matRotY = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	                   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	// Y軸回転行列に回転角を設定する
	matRotY.m[0][0] = cos(worldTransform_rotation_y);
	matRotY.m[0][2] = -sin(worldTransform_rotation_y);
	matRotY.m[2][0] = sin(worldTransform_rotation_y);
	matRotY.m[2][2] = cos(worldTransform_rotation_y);

	return matRotY;
}

Matrix4 GameScene::RotateZ(float worldTransform_rotate_z) {
	// Z軸回転行列を宣言して単位行列を代入する
	Matrix4 matRotZ = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	                   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	// Z軸回転行列に回転角を設定する
	matRotZ.m[0][0] = cos(worldTransform_rotate_z);
	matRotZ.m[0][1] = sin(worldTransform_rotate_z);
	matRotZ.m[1][0] = -sin(worldTransform_rotate_z);
	matRotZ.m[1][1] = cos(worldTransform_rotate_z);

	return matRotZ;
}

Matrix4 GameScene::Translation(Vector3 worldTransform_translation_) {
	//平行移動行列を宣言して単位行列を代入する
	Matrix4 matTrans = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	                    0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	//平行移動を行列に設定する
	matTrans.m[3][0] = worldTransform_translation_.x;
	matTrans.m[3][1] = worldTransform_translation_.y;
	matTrans.m[3][2] = worldTransform_translation_.z;

	return matTrans;
}

Matrix4 GameScene::MatWorld(Matrix4 matScale, Matrix4 matRot, Matrix4 matTrans) {
	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.matWorld_ = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		                            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

		//ワールドトランスフォームにスケーリング行列を代入する
		worldTransform.matWorld_ *= matScale *= matRot *= matTrans;

		return worldTransform.matWorld_;
	}
}

//ラジアン変換
float GameScene::Radian(float angle) {
	float radian;
	radian = angle * (PI / 180);

	return radian;

}

//度数変換
float GameScene::Angle(float radian) {
	float angle;
	angle = radian * (180 / PI);

	return angle;
}


//clamp
float GameScene::clamp(float value, float min, float max) {
	if (value < min) {
		return min;
	} else if (value > max) {
		return max;
	}
	return value;
}