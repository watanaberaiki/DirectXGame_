#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "DebugText.h"
#include "Input.h"
#include"Affin.h"
#include"EnemyBullet.h"
class Enemy
{
	enum class Phase {
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};
public:

	void Initialize();

	void Update();

	void Approach();

	void Leave();

	void Draw(const ViewProjection& viewProjection);

	void Fire();

	void PhaseInitialize();

private:
	
	//���ˊԊu
	static const int kFireInterval = 60;

	//���˃^�C�}�[
	int32_t FireTimer = 0;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Affin* Affin_;

	Vector3 Approachspeed;

	Vector3 Leavespeed;

	Phase phase_= Phase::Approach;

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
};

