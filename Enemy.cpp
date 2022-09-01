#include "Enemy.h"
#include "GameScene.h"
#include <cassert>

void Enemy::Initialize(Model* model, Vector3 pos) {
	// NULL�|�C���^�`�F�b�N
	assert(model);

	//3D���f���̐���
	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.scale_ = Vector3(2, 2, 2);

	worldTransform_.translation_ = pos;

	MoveYSpeed = Vector3(0, 0.1, 0);

	MoveXSpeed = Vector3(0.1, 0, 0);

	isDead_ = false;


	//�ڋ߃t�F�[�Y�̏�����
	PhaseInitialize();
}

void Enemy::MoveY() {
	worldTransform_.translation_ += MoveYSpeed;
	//����̈ʒu�ŕύX
	if (worldTransform_.translation_.y < -20.0f || worldTransform_.translation_.y>20) {
		MoveYSpeed = -MoveYSpeed;
	}

	//�̗͈��ȉ��ɂȂ�Ƒ����Ȃ�
	if (enemyLife <= 30) {
		//���˃^�C�}�[���f�N�������g
		FireTimer--;
		if (FireTimer == 0) {
			//�e�𔭎�
			PlayerAim();
			//���˃^�C�}�[��������
			FireTimer = kFireIntervalFast;
		}

	}else {
		//���˃^�C�}�[���f�N�������g
		FireTimer--;
		if (FireTimer == 0) {
			//�e�𔭎�
			PlayerAim();
			//���˃^�C�}�[��������
			FireTimer = kFireInterval;
		}
	}
}

void Enemy::MoveX() {
	worldTransform_.translation_ += MoveXSpeed;
	//����̈ʒu�ŕύX
	if (worldTransform_.translation_.x < -35.0f || worldTransform_.translation_.x>35) {
		MoveXSpeed = -MoveXSpeed;
	}
	//�̗͈��ȉ��ɂȂ�Ƒ����Ȃ�
	if (enemyLife <= 30) {
		//���˃^�C�}�[���f�N�������g
		FireTimer--;
		if (FireTimer == 0) {
			//�e�𔭎�
			PlayerAim();
			//���˃^�C�}�[��������
			FireTimer = kFireIntervalFast;
		}
	}
	else {
		//���˃^�C�}�[���f�N�������g
		FireTimer--;
		if (FireTimer == 0) {
			//�e�𔭎�
			PlayerAim();
			//���˃^�C�}�[��������
			FireTimer = kFireInterval;
		}
	}

}

void Enemy::Stop() {
	//�̗͈��ȉ��ɂȂ�Ƒ����Ȃ�
	if (enemyLife <= 30) {
		//���˃^�C�}�[���f�N�������g
		FireTimer--;
		AroundFireTimer--;
		if (FireTimer == 0) {
			//�e�𔭎�
			PlayerAim();
			//���˃^�C�}�[��������
			FireTimer = kFireIntervalFast;
		}
		if (AroundFireTimer == 0) {
			Cross();
			AroundFireTimer = kAroundFireIntervalFast;
		}
	}
	else {
		//���˃^�C�}�[���f�N�������g
		FireTimer--;
		AroundFireTimer--;
		if (FireTimer == 0) {
			//�e�𔭎�
			PlayerAim();
			//���˃^�C�}�[��������
			FireTimer = kFireInterval;
		}
		if (AroundFireTimer == 0) {
			Cross();
			AroundFireTimer = kAroundFireInterval;
		}
	}
}

void Enemy::Update() {
	

	switch (phase_) {
	case Phase::MoveY:
		MoveY();
		break;

	case Phase::MoveX:
		MoveX();
		break;

	case Phase::Stop:
		Stop();
		break;
	}

	////�f�X�t���O�̗����������폜
	//bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
	//	return bullet->IsDead();
	//	});

	//���[���h�s���ݒ肷��
	worldTransform_.matWorld_ = Affin_->MatWorld(
		Affin_->Scale(worldTransform_.scale_),
		Affin_->Rotate(Affin_->RotateX(worldTransform_.rotation_.x),
			Affin_->RotateY(worldTransform_.rotation_.y),
			Affin_->RotateZ(worldTransform_.rotation_.z)),
		Affin_->Translation(worldTransform_.translation_));

	worldTransform_.TransferMatrix();

	////�e����
	//for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
	//	bullet->Update();
	//}

	DebugText::GetInstance()->SetPos(20, 50);
	DebugText::GetInstance()->Printf(
		"enemyLife:(%d)", enemyLife);

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);

	////�e�`��
	//for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
	//	bullet->Draw(viewProjection);
	//}
}

void Enemy::PlayerAim() {

	assert(player_);
	assert(gamescene_);

	//�e�̑��x
	const float kBulletSpeed = 0.5f;

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

	gamescene_->AddEnemyBullet(newBullet);

}



void Enemy::Cross() {
	assert(player_);
	assert(gamescene_);

	srand((unsigned int)time(NULL));
	int ramdom=rnd() % 2;
	//�e�̑��x
	const float kBulletSpeed = 0.5f;
	if (ramdom == 0) {
		//���L����
		Vector3 playerWorld = player_->GetWorldPosition();
		for (int i = -25; i < 25; i++) {

			Vector3 vector = { 1.0f + i,1.0f+i,playerWorld.z };

			//���K��
			float len = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
			vector /= len;

			//�X�s�[�h���|����
			vector *= kBulletSpeed;
			Vector3 velocity(vector);

			//�e�𐶐����A������
			std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
			newBullet->Initialize(model_, worldTransform_.translation_, velocity);

			gamescene_->AddEnemyBullet(newBullet);
		}
		for (int i = -25; i < 25; i++) {
			Vector3 vector = { 1.0f-i,1.0f + i,playerWorld.z };

			//���K��
			float len = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
			vector /= len;

			//�X�s�[�h���|����
			vector *= kBulletSpeed;
			Vector3 velocity(vector);

			//�e�𐶐����A������
			std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
			newBullet->Initialize(model_, worldTransform_.translation_, velocity);

			gamescene_->AddEnemyBullet(newBullet);
		}

	}
	else if (ramdom == 1) {
		//���L����
		Vector3 playerWorld = player_->GetWorldPosition();
		for (int i = -25; i < 25; i++) {



			Vector3 vector = { 1.0f + i,1.0f,playerWorld.z };

			//���K��
			float len = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
			vector /= len;

			//�X�s�[�h���|����
			vector *= kBulletSpeed;
			Vector3 velocity(vector);

			//�e�𐶐����A������
			std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
			newBullet->Initialize(model_, worldTransform_.translation_, velocity);

			gamescene_->AddEnemyBullet(newBullet);
		}
		for (int i = -25; i < 25; i++) {
			Vector3 vector = { 1.0f,1.0f + i,playerWorld.z };

			//���K��
			float len = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
			vector /= len;

			//�X�s�[�h���|����
			vector *= kBulletSpeed;
			Vector3 velocity(vector);

			//�e�𐶐����A������
			std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
			newBullet->Initialize(model_, worldTransform_.translation_, velocity);

			gamescene_->AddEnemyBullet(newBullet);
		}
	}
}

void Enemy::PhaseInitialize() {
	//���˃^�C�}�[��������
	FireTimer = kFireInterval;
	AroundFireTimer = kAroundFireInterval;
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
	enemyLife -= 1;
	if (enemyLife == 0) {
		isDead_ = true;
	}
}

void Enemy::RandomPhase() {
	srand((unsigned int)time(NULL));
	int phasernd = rnd() % 3;
	if (phasernd == 0) {
		phase_ = Phase::MoveY;
		//���˃^�C�}�[��������
		FireTimer = kFireInterval;
		AroundFireTimer = kFireInterval;
	}
	else if (phasernd == 1) {
		phase_ = Phase::MoveX;
		//���˃^�C�}�[��������
		FireTimer = kFireInterval;
		AroundFireTimer = kFireInterval;
	}
	else if (phasernd == 2) {
		worldTransform_.translation_ = Vector3(0, 0, 30);
		phase_ = Phase::Stop;
		//���˃^�C�}�[��������
		FireTimer = kFireInterval;
		AroundFireTimer = kAroundFireInterval;
	}

}

void Enemy::EnemyDead() {
	isDead_ = true;
}

bool Enemy::GetEnemyIsDead() {
	return isDead_;
}