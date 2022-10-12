#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "DebugText.h"
#include "Input.h"
#include"affin/Affin.h"
#include"EnemyBullet.h"
#include <list>
#include <random>
#include <time.h>

class Player;
class GameScene;

class Enemy
{
	enum class Phase {
		MoveY,	//�c�ɓ���
		MoveX,		//���ɓ���
		Stop,		//��~����
	};


private:

	//���ˊԊu
	static const int kFireInterval = 60;
	static const int kAroundFireInterval = 120;

	static const int kFireIntervalFast = 30;
	static const int kAroundFireIntervalFast = 60;

	//���˃^�C�}�[
	int32_t FireTimer = 0;
	int32_t AroundFireTimer = 0;

	int enemyLife = 100;

	//�f�X�t���O
	bool isDead_ = false;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;

	Affin* Affin_;

	Vector3 MoveYSpeed;

	Vector3 MoveXSpeed;


	Phase phase_ = Phase::MoveX;

	//����
	std::random_device rnd;

	////�e
	//std::list<std::unique_ptr<EnemyBullet>> bullets_;

	

public:

	//�v���C���[
	Player* player_ = nullptr;

	//�Q�[���V�[��
	GameScene* gamescene_ = nullptr;

	//������
	void Initialize(Model* model,Vector3 pos);

	//�X�V����
	void Update();
	
	//�ڋ߃t�F�[�Y
	void MoveY();

	//���E�t�F�[�Y
	void MoveX();

	//��~�t�F�[�Y
	void Stop();

	//�`�揈��
	void Draw(const ViewProjection& viewProjection);

	//���@�_���e����
	void PlayerAim();
	//���@�̎���
	void Cross();
	//�t�F�[�Y������
	void PhaseInitialize();

	//�v���C���[�Z�b�^�[
	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	//�Փ˔���
	void OnCollision();

	//�Q�[���V�[���w�b�_�[
	void SetGameScene(GameScene* gamescene) { gamescene_ = gamescene; }

	//�G�̏���
	bool IsDead()const { return isDead_; }

	void RandomPhase();

	void EnemyDead();

	bool GetEnemyIsDead();
};

