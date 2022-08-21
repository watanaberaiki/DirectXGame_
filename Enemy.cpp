#include "Enemy.h"
#include <cassert>

void Enemy::Initialize() {
	//3Dモデルの生成
	model_ = Model::Create();

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("potta-.jpg");

	worldTransform_.Initialize();

	worldTransform_.translation_ = Vector3(0, 3, 10);

	Approachspeed = Vector3(0, 0, -0.5);

	Leavespeed = Vector3(0.1,0.1, 0);

}

void Enemy::Approach() {
	worldTransform_.translation_ += Approachspeed;
	//既定の位置で変更
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {
	worldTransform_.translation_ += Leavespeed;
}

void Enemy::Update() {
	switch (phase_) {
	case Phase::Approach:
		Approach();
		break;

	case Phase::Leave:
		Leave();
		break;
	}



	//ワールド行列を設定する
	worldTransform_.matWorld_ = Affin_->MatWorld(
		Affin_->Scale(worldTransform_.scale_),
		Affin_->Rotate(Affin_->RotateX(worldTransform_.rotation_.x),
			Affin_->RotateY(worldTransform_.rotation_.y),
			Affin_->RotateZ(worldTransform_.rotation_.z)),
		Affin_->Translation(worldTransform_.translation_));

	worldTransform_.TransferMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}