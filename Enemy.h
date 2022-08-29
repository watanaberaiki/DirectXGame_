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
		Approach,	//接近する
		Leave,		//離脱する
	};


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

	Phase phase_ = Phase::Approach;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	

public:

	//プレイヤー
	Player* player_ = nullptr;

	//初期化
	void Initialize(Model* model, uint32_t textureHandle);

	//更新処理
	void Update();
	
	//接近フェーズ
	void Approach();

	//離脱フェーズ
	void Leave();

	//描画処理
	void Draw(const ViewProjection& viewProjection);

	//発射
	void Fire();

	//フェーズ初期化
	void PhaseInitialize();

	//プレイヤーセッター
	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	//衝突判定
	void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

};

