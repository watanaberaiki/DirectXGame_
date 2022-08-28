#include "Enemy.h"
#include "GameScene.h"
#include <cassert>

void Enemy::Initialize() {
	//3Dモデルの生成
	model_ = Model::Create();

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("potta-.jpg");

	worldTransform_.Initialize();

	worldTransform_.translation_ = Vector3(10, 3, 100);

	Approachspeed = Vector3(0, 0, -0.5);

	Leavespeed = Vector3(0.1, 0.1, 0);

	////敵の弾発生
	//Fire();

	//接近フェーズの初期化
	PhaseInitialize();
}

void Enemy::Approach() {
	worldTransform_.translation_ += Approachspeed;
	//既定の位置で変更
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}

	//発射タイマーをデクリメント
	FireTimer--;
	if (FireTimer == 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
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

	//デスフラグの立った球を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});

	//ワールド行列を設定する
	worldTransform_.matWorld_ = Affin_->MatWorld(
		Affin_->Scale(worldTransform_.scale_),
		Affin_->Rotate(Affin_->RotateX(worldTransform_.rotation_.x),
			Affin_->RotateY(worldTransform_.rotation_.y),
			Affin_->RotateZ(worldTransform_.rotation_.z)),
		Affin_->Translation(worldTransform_.translation_));

	worldTransform_.TransferMatrix();

	//弾発射
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire() {

	assert(player_);

	//弾の速度
	const float kBulletSpeed = 1.0f;

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

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::PhaseInitialize() {
	//発射タイマーを初期化
	FireTimer = kFireInterval;
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

}