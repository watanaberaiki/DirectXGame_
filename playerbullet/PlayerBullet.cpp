#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3 position) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {
	//ワールド行列を設定する
	worldTransform_.matWorld_ = Affin_->MatWorld(
		Affin_->Scale(worldTransform_.scale_),
		Affin_->Rotate(Affin_->RotateX(worldTransform_.rotation_.x),
			Affin_->RotateY(worldTransform_.rotation_.y),
			Affin_->RotateZ(worldTransform_.rotation_.z)),
		Affin_->Translation(worldTransform_.translation_));

	worldTransform_.TransferMatrix();

}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}


