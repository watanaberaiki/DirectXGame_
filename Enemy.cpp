#include "Enemy.h"
#include "GameScene.h"
#include <cassert>

void Enemy::Initialize() {
	//3D���f���̐���
	model_ = Model::Create();

	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("potta-.jpg");

	worldTransform_.Initialize();

	worldTransform_.translation_ = Vector3(10, 3, 100);

	Approachspeed = Vector3(0, 0, -0.5);

	Leavespeed = Vector3(0.1, 0.1, 0);

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

	//�f�X�t���O�̗����������폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

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

	assert(player_);

	//�e�̑��x
	const float kBulletSpeed = 1.0f;

	//���L����
	Vector3 playerWorld = player_->GetWorldPosition();
	//�G�L����
	Vector3 enemyWorld = GetWorldPosition();
	//�����x�N�g��
	Vector3 vector = playerWorld -= enemyWorld;

	//���K��
	float len = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	vector /= len;

	//�X�s�[�h���|����
	vector *= kBulletSpeed;
	Vector3 velocity(vector);

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

Vector3 Enemy::GetWorldPosition() {
	//���[���h�ɓ������W
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {

}