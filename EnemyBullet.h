#include "WorldTransform.h"
#include "Model.h"
#include "DebugText.h"
#include "Input.h"
#include"affin/Affin.h"

#pragma once

class EnemyBullet
{
public:

	//������
	void Initialize(Model* model, const Vector3 position, const Vector3 velocity);

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

	//�e�̏���
	bool IsDead()const { return isDead_; }

	//�Փ˔���
	void OnCollision();

	//���[���h���W�擾
	Vector3 GetWorldPosition();

	void EnemyBulletDead();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Affin* Affin_;

	//���x
	Vector3 velocity_;

	//����
	static const int32_t kLifeTime = 60 * 7;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;

};

