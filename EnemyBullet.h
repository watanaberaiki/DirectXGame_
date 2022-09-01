#include "WorldTransform.h"
#include "Model.h"
#include "DebugText.h"
#include "Input.h"
#include"Affin.h"

#pragma once

class EnemyBullet
{
public:

	//初期化
	void Initialize(Model* model, const Vector3 position, const Vector3 velocity);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//弾の消滅
	bool IsDead()const { return isDead_; }

	//衝突判定
	void OnCollision();

	//ワールド座標取得
	Vector3 GetWorldPosition();

	void EnemyBulletDead();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Affin* Affin_;

	//速度
	Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 7;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

};

