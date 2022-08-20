#include "Enemy.h"
#include <cassert>

void Enemy::Initialize() {
	//3D���f���̐���
	model_ = Model::Create();

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("potta-.jpg");

	worldTransform_.Initialize();
	
	worldTransform_.translation_ = Vector3(0,3,10);

	speed = Vector3(0, 0, -0.5);
}

void Enemy::Update() {
	worldTransform_.translation_ += speed;


	//���[���h�s���ݒ肷��
	worldTransform_.matWorld_ = Affin_->MatWorld(
		Affin_->Scale(worldTransform_.scale_),
		Affin_->Rotate(Affin_->RotateX(worldTransform_.rotation_.x),
			Affin_->RotateY(worldTransform_.rotation_.y),
			Affin_->RotateZ(worldTransform_.rotation_.z)),
		Affin_->Translation(worldTransform_.translation_));

	worldTransform_.TransferMatrix();
}

void Enemy::Draw( const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}