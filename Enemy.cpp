#include "Enemy.h"
#include "GameScene.h"
#include <cassert>

void Enemy::Initialize(Model* model, Vector3 pos) {
	// NULLポインタチェック
	assert(model);

	//3Dモデルの生成
	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.scale_ = Vector3(2, 2, 2);

	worldTransform_.translation_ = pos;

	MoveYSpeed = Vector3(0, 0.1, 0);

	MoveXSpeed = Vector3(0.1, 0, 0);

	isDead_ = false;


	//接近フェーズの初期化
	PhaseInitialize();
}

void Enemy::MoveY() {
	worldTransform_.translation_ += MoveYSpeed;
	//既定の位置で変更
	if (worldTransform_.translation_.y < -20.0f || worldTransform_.translation_.y>20) {
		MoveYSpeed = -MoveYSpeed;
	}

	//体力一定以下になると早くなる
	if (enemyLife <= 30) {
		//発射タイマーをデクリメント
		FireTimer--;
		if (FireTimer == 0) {
			//弾を発射
			PlayerAim();
			//発射タイマーを初期化
			FireTimer = kFireIntervalFast;
		}

	}else {
		//発射タイマーをデクリメント
		FireTimer--;
		if (FireTimer == 0) {
			//弾を発射
			PlayerAim();
			//発射タイマーを初期化
			FireTimer = kFireInterval;
		}
	}
}

void Enemy::MoveX() {
	worldTransform_.translation_ += MoveXSpeed;
	//既定の位置で変更
	if (worldTransform_.translation_.x < -35.0f || worldTransform_.translation_.x>35) {
		MoveXSpeed = -MoveXSpeed;
	}
	//体力一定以下になると早くなる
	if (enemyLife <= 30) {
		//発射タイマーをデクリメント
		FireTimer--;
		if (FireTimer == 0) {
			//弾を発射
			PlayerAim();
			//発射タイマーを初期化
			FireTimer = kFireIntervalFast;
		}
	}
	else {
		//発射タイマーをデクリメント
		FireTimer--;
		if (FireTimer == 0) {
			//弾を発射
			PlayerAim();
			//発射タイマーを初期化
			FireTimer = kFireInterval;
		}
	}

}

void Enemy::Stop() {
	//体力一定以下になると早くなる
	if (enemyLife <= 30) {
		//発射タイマーをデクリメント
		FireTimer--;
		AroundFireTimer--;
		if (FireTimer == 0) {
			//弾を発射
			PlayerAim();
			//発射タイマーを初期化
			FireTimer = kFireIntervalFast;
		}
		if (AroundFireTimer == 0) {
			Cross();
			AroundFireTimer = kAroundFireIntervalFast;
		}
	}
	else {
		//発射タイマーをデクリメント
		FireTimer--;
		AroundFireTimer--;
		if (FireTimer == 0) {
			//弾を発射
			PlayerAim();
			//発射タイマーを初期化
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

	////デスフラグの立った球を削除
	//bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
	//	return bullet->IsDead();
	//	});

	//ワールド行列を設定する
	worldTransform_.matWorld_ = Affin_->MatWorld(
		Affin_->Scale(worldTransform_.scale_),
		Affin_->Rotate(Affin_->RotateX(worldTransform_.rotation_.x),
			Affin_->RotateY(worldTransform_.rotation_.y),
			Affin_->RotateZ(worldTransform_.rotation_.z)),
		Affin_->Translation(worldTransform_.translation_));

	worldTransform_.TransferMatrix();

	////弾発射
	//for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
	//	bullet->Update();
	//}

	DebugText::GetInstance()->SetPos(20, 50);
	DebugText::GetInstance()->Printf(
		"enemyLife:(%d)", enemyLife);

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);

	////弾描画
	//for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
	//	bullet->Draw(viewProjection);
	//}
}

void Enemy::PlayerAim() {

	assert(player_);
	assert(gamescene_);

	//弾の速度
	const float kBulletSpeed = 0.5f;

	//自キャラ
	Vector3 playerWorld = player_->GetWorldPosition();
	//敵キャラ
	Vector3 enemyWorld = GetWorldPosition();
	//差分ベクトル
	Vector3 vector = playerWorld -= enemyWorld;

	//正規化
	float len = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	vector /= len;

	//スピードを掛ける
	vector *= kBulletSpeed;
	Vector3 velocity(vector);

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	gamescene_->AddEnemyBullet(newBullet);

}



void Enemy::Cross() {
	assert(player_);
	assert(gamescene_);

	srand((unsigned int)time(NULL));
	int ramdom=rnd() % 2;
	//弾の速度
	const float kBulletSpeed = 0.5f;
	if (ramdom == 0) {
		//自キャラ
		Vector3 playerWorld = player_->GetWorldPosition();
		for (int i = -25; i < 25; i++) {

			Vector3 vector = { 1.0f + i,1.0f+i,playerWorld.z };

			//正規化
			float len = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
			vector /= len;

			//スピードを掛ける
			vector *= kBulletSpeed;
			Vector3 velocity(vector);

			//弾を生成し、初期化
			std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
			newBullet->Initialize(model_, worldTransform_.translation_, velocity);

			gamescene_->AddEnemyBullet(newBullet);
		}
		for (int i = -25; i < 25; i++) {
			Vector3 vector = { 1.0f-i,1.0f + i,playerWorld.z };

			//正規化
			float len = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
			vector /= len;

			//スピードを掛ける
			vector *= kBulletSpeed;
			Vector3 velocity(vector);

			//弾を生成し、初期化
			std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
			newBullet->Initialize(model_, worldTransform_.translation_, velocity);

			gamescene_->AddEnemyBullet(newBullet);
		}

	}
	else if (ramdom == 1) {
		//自キャラ
		Vector3 playerWorld = player_->GetWorldPosition();
		for (int i = -25; i < 25; i++) {



			Vector3 vector = { 1.0f + i,1.0f,playerWorld.z };

			//正規化
			float len = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
			vector /= len;

			//スピードを掛ける
			vector *= kBulletSpeed;
			Vector3 velocity(vector);

			//弾を生成し、初期化
			std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
			newBullet->Initialize(model_, worldTransform_.translation_, velocity);

			gamescene_->AddEnemyBullet(newBullet);
		}
		for (int i = -25; i < 25; i++) {
			Vector3 vector = { 1.0f,1.0f + i,playerWorld.z };

			//正規化
			float len = std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
			vector /= len;

			//スピードを掛ける
			vector *= kBulletSpeed;
			Vector3 velocity(vector);

			//弾を生成し、初期化
			std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
			newBullet->Initialize(model_, worldTransform_.translation_, velocity);

			gamescene_->AddEnemyBullet(newBullet);
		}
	}
}

void Enemy::PhaseInitialize() {
	//発射タイマーを初期化
	FireTimer = kFireInterval;
	AroundFireTimer = kAroundFireInterval;
}

Vector3 Enemy::GetWorldPosition() {
	//ワールドに入れる座標
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得(ワールド座標)
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
		//発射タイマーを初期化
		FireTimer = kFireInterval;
		AroundFireTimer = kFireInterval;
	}
	else if (phasernd == 1) {
		phase_ = Phase::MoveX;
		//発射タイマーを初期化
		FireTimer = kFireInterval;
		AroundFireTimer = kFireInterval;
	}
	else if (phasernd == 2) {
		worldTransform_.translation_ = Vector3(0, 0, 30);
		phase_ = Phase::Stop;
		//発射タイマーを初期化
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