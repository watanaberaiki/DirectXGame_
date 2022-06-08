#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"
#include"math.h"
GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	PI = 3.1415;

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280,720);
		
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());


	//ワールド変換行列//
	
	//X,Y,Z方向のスケーリングを設定
	worldTransform_.scale_ = {5.0f,5.0f,5.0f};
	//X,Y,Z軸周りの回転角を設定
	worldTransform_.rotation_ = {PI/4, PI/4,0.0f };
	//X,Y,Z軸周りの平行移動を設定
	worldTransform_.translation_ = {10.0f,10.0f,10.0f,};

	//合成用回転行列の宣言
	Matrix4 matRot;

	//各回転角を掛け算して合成用回転行列に代入する
	matRot = 
	RotateZ(worldTransform_.rotation_.z) *= 
	RotateX(worldTransform_.rotation_.x) *=
	RotateY(worldTransform_.rotation_.y);

	//ワールド行列を設定する
	MatWorld(Scale(worldTransform_.scale_), matRot, Translation(worldTransform_.translation_));

	//行列の転送
	worldTransform_.TransferMatrix();
}

void GameScene::Update() {
	//デバッグカメラの更新
	debugCamera_->Update();
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
	//3Dモデル描画
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
	

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
	Matrix4 matScale = {
		1.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f
	};

	//スケーリング倍率を行列に設定する
	matScale.m[0][0] = worldTransform_scale_.x;
	matScale.m[1][1] = worldTransform_scale_.y;
	matScale.m[2][2] = worldTransform_scale_.z;

	return matScale;
}


Matrix4 GameScene::RotateX(float worldTransform_rotation_x) {
	// X軸回転行列を宣言して単位行列を代入する
	Matrix4 matRotX = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f
	};
	// X軸回転行列に回転角を設定する
	matRotX.m[1][1] = cos(worldTransform_rotation_x);
	matRotX.m[1][2] = sin(worldTransform_rotation_x);
	matRotX.m[2][1] = -sin(worldTransform_rotation_x);
	matRotX.m[2][2] = cos(worldTransform_rotation_x);

	return matRotX;
}

Matrix4 GameScene::RotateY(float worldTransform_rotation_y) {
	// Y軸回転行列を宣言して単位行列を代入する
	Matrix4 matRotY = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f
	};	

	// Y軸回転行列に回転角を設定する
	matRotY.m[0][0] = cos(worldTransform_rotation_y);
	matRotY.m[0][2] = -sin(worldTransform_rotation_y);
	matRotY.m[2][0] = sin(worldTransform_rotation_y);
	matRotY.m[2][2] = cos(worldTransform_rotation_y);

	return matRotY;
}

Matrix4 GameScene::RotateZ(float worldTransform_rotate_z) {
	// Z軸回転行列を宣言して単位行列を代入する
	Matrix4 matRotZ = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// Z軸回転行列に回転角を設定する
	matRotZ.m[0][0] = cos(worldTransform_rotate_z);
	matRotZ.m[0][1] = sin(worldTransform_rotate_z);
	matRotZ.m[1][0] = -sin(worldTransform_rotate_z);
	matRotZ.m[1][1] = cos(worldTransform_rotate_z);

	return matRotZ;
}



Matrix4 GameScene::Translation(Vector3 worldTransform_translation_) {
	//平行移動行列を宣言して単位行列を代入する
	Matrix4 matTrans = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	//平行移動を行列に設定する
	matTrans.m[3][0] = worldTransform_translation_.x;
	matTrans.m[3][1] = worldTransform_translation_.y;
	matTrans.m[3][2] = worldTransform_translation_.z;

	return matTrans;
}


Matrix4 GameScene::MatWorld(Matrix4 matScale, Matrix4 matRot, Matrix4 matTrans) {
	worldTransform_.matWorld_ = {
		1.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f};


	//ワールドトランスフォームにスケーリング行列を代入する
	worldTransform_.matWorld_ *= matScale *= matRot *= matTrans;

	return worldTransform_.matWorld_;
}