#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "DebugText.h"
#include "Input.h"
#include"Affin.h"
#include"EnemyBullet.h"
#include <list>

class Player;

class Enemy
{
	enum class Phase {
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};


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

	Phase phase_ = Phase::Approach;

	//�e
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	

public:

	//�v���C���[
	Player* player_ = nullptr;

	//������
	void Initialize(Model* model, uint32_t textureHandle);

	//�X�V����
	void Update();
	
	//�ڋ߃t�F�[�Y
	void Approach();

	//���E�t�F�[�Y
	void Leave();

	//�`�揈��
	void Draw(const ViewProjection& viewProjection);

	//����
	void Fire();

	//�t�F�[�Y������
	void PhaseInitialize();

	//�v���C���[�Z�b�^�[
	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	//�Փ˔���
	void OnCollision();

	//�e���X�g���擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

};

