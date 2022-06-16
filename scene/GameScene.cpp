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

	//ワールドトランスフォームの初期化
	//キャラクターの大元[0]
	worldTransforms_[PartId::kRoot].Initialize();
	//脊椎
	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].translation_ = { 0, 4.5f, 0 };
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];

	//上半身//
	//胸
	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].translation_ = { 0,0,0 };
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	//頭
	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].translation_ = { 0,4.5f,0 };
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	//右腕
	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].translation_ = { 4.5f,0,0 };
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	//左腕
	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].translation_ = { -4.5f,0,0 };
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];

	//下半身
	//尻
	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].translation_ = { 0,-4.5f,0 };
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	//右足
	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].translation_ = { 4.5f,-4.5f,0 };
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	//左足
	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].translation_ = { -4.5f,-4.5f,0 };
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];

	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Radian(45.0f);
	////アスペクト比を設定
	// viewProjection_.aspectRatio = 1.0f;

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

void GameScene::Update() {

	//キャラクター移動処理
	//キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	if (input_->PushKey(DIK_RIGHT)) {
		move.x = 0.1f;
	}
	else if (input_->PushKey(DIK_LEFT)) {
		move.x = -0.1f;
	}
	else {
		move.x = 0;
	}
	worldTransforms_[PartId::kRoot].translation_ += move;


	//上半身回転処理
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_U)) {
		worldTransforms_[PartId::kChest].rotation_.y -= 0.1f;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransforms_[PartId::kChest].rotation_.y += 0.1f;
	}

	//下半身回転処理
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_J)) {
		worldTransforms_[PartId::kHip].rotation_.y -= 0.1f;
	}
	else if (input_->PushKey(DIK_K)) {
		worldTransforms_[PartId::kHip].rotation_.y += 0.1f;
	}


	//デバッグ用表示
	debugText_->SetPos(50, 50);
	debugText_->Printf(
		"translation:(%f,%f,%f)", worldTransforms_[0].translation_.x,
		worldTransforms_[0].translation_.y, worldTransforms_[0].translation_.z);

	//全パーツの更新処理を追加
	for (int i = 0; i < kNumPartId; i++) {
		WorldTransformUpdate(i);
	}


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
	for (int i = 0; i < kNumPartId; i++) {
		if (i == kSpine || i == kRoot) {
			continue;
		}
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
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
	Matrix4 matScale = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	//スケーリング倍率を行列に設定する
	matScale.m[0][0] = worldTransform_scale_.x;
	matScale.m[1][1] = worldTransform_scale_.y;
	matScale.m[2][2] = worldTransform_scale_.z;

	return matScale;
}

Matrix4 GameScene::RotateX(float worldTransform_rotation_x) {
	// X軸回転行列を宣言して単位行列を代入する
	Matrix4 matRotX = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	// X軸回転行列に回転角を設定する
	matRotX.m[1][1] = cos(worldTransform_rotation_x);
	matRotX.m[1][2] = sin(worldTransform_rotation_x);
	matRotX.m[2][1] = -sin(worldTransform_rotation_x);
	matRotX.m[2][2] = cos(worldTransform_rotation_x);

	return matRotX;
}

Matrix4 GameScene::RotateY(float worldTransform_rotation_y) {
	// Y軸回転行列を宣言して単位行列を代入する
	Matrix4 matRotY = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	// Y軸回転行列に回転角を設定する
	matRotY.m[0][0] = cos(worldTransform_rotation_y);
	matRotY.m[0][2] = -sin(worldTransform_rotation_y);
	matRotY.m[2][0] = sin(worldTransform_rotation_y);
	matRotY.m[2][2] = cos(worldTransform_rotation_y);

	return matRotY;
}

Matrix4 GameScene::RotateZ(float worldTransform_rotate_z) {
	// Z軸回転行列を宣言して単位行列を代入する
	Matrix4 matRotZ = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	// Z軸回転行列に回転角を設定する
	matRotZ.m[0][0] = cos(worldTransform_rotate_z);
	matRotZ.m[0][1] = sin(worldTransform_rotate_z);
	matRotZ.m[1][0] = -sin(worldTransform_rotate_z);
	matRotZ.m[1][1] = cos(worldTransform_rotate_z);

	return matRotZ;
}

Matrix4 GameScene::Translation(Vector3 worldTransform_translation_) {
	//平行移動行列を宣言して単位行列を代入する
	Matrix4 matTrans = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	//平行移動を行列に設定する
	matTrans.m[3][0] = worldTransform_translation_.x;
	matTrans.m[3][1] = worldTransform_translation_.y;
	matTrans.m[3][2] = worldTransform_translation_.z;

	return matTrans;
}

Matrix4 GameScene::MatWorld(Matrix4 matScale, Matrix4 matRot, Matrix4 matTrans) {

	WorldTransform worldTransform;
	worldTransform.matWorld_ = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	//ワールドトランスフォームにスケーリング行列を代入する
	worldTransform.matWorld_ *= matScale *= matRot *= matTrans;

	return worldTransform.matWorld_;
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

// clamp
float GameScene::clamp(float value, float min, float max) {
	if (value < min) {
		return min;
	}
	else if (value > max) {
		return max;
	}
	return value;
}

void GameScene::WorldTransformUpdate(int i) {
	//各回転角を掛け算して合成用回転行列に代入する
	matRot[i] = RotateZ(worldTransforms_[i].rotation_.z) *=
		RotateX(worldTransforms_[i].rotation_.x) *=
		RotateY(worldTransforms_[i].rotation_.y);
	//ワールド行列を設定する
	worldTransforms_[i].matWorld_ = MatWorld(
		Scale(worldTransforms_[i].scale_),
		matRot[i],
		Translation(worldTransforms_[i].translation_));
	if (i != 0) {
		worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
	}
	//行列の転送
	worldTransforms_[i].TransferMatrix();
}