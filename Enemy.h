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
		Approach,	//接近する
		Leave,		//離脱する
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
	
	//発射間隔
	static const int kFireInterval = 60;

	//発射タイマー
	int32_t FireTimer = 0;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Affin* Affin_;

	Vector3 Approachspeed;

	Vector3 Leavespeed;

	Phase phase_= Phase::Approach;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
};

