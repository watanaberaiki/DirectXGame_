#include "Enemy.h"
#include <cassert>

void Enemy::Initialize() {
	//3D���f���̐���
	model_ = Model::Create();

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("potta-.jpg");

	worldTransform_.Initialize();

	worldTransform_.translation_ = Vector3(10, 3, 300);

	Approachspeed = Vector3(0, 0, -0.5);

	Leavespeed = Vector3(0.1,0.1, 0);

	////�G�̒e����
	//Fire();

	//�ڋ߃t�F�[�Y�̏�����
	PhaseInitialize();
}

void Enemy::Approach() {
	worldTransform_.translation_ += Approachspeed;
	//����̈ʒu�ŕύX
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	//���˃^�C�}�[���f�N�������g
	FireTimer--;
	if (FireTimer == 0) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		FireTimer = kFireInterval;
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



	//���[���h�s���ݒ肷��
	worldTransform_.matWorld_ = Affin_->MatWorld(
		Affin_->Scale(worldTransform_.scale_),
		Affin_->Rotate(Affin_->RotateX(worldTransform_.rotation_.x),
			Affin_->RotateY(worldTransform_.rotation_.y),
			Affin_->RotateZ(worldTransform_.rotation_.z)),
		Affin_->Translation(worldTransform_.translation_));

	worldTransform_.TransferMatrix();

	//�e����
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire() {
	//�e�̑��x
	const float kBulletSpeed = -1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//���x�x�N�g����ڐ��̓����ɍ��킹�ĉ�]������
	velocity = Affin_->MatVector(velocity, worldTransform_);
	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::PhaseInitialize() {
	//���˃^�C�}�[��������
	FireTimer = kFireInterval;
}