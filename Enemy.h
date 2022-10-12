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
		MoveY,	//縦に動く
		MoveX,		//横に動く
		Stop,		//停止する
	};


private:

	//発射間隔
	static const int kFireInterval = 60;
	static const int kAroundFireInterval = 120;

	static const int kFireIntervalFast = 30;
	static const int kAroundFireIntervalFast = 60;

	//発射タイマー
	int32_t FireTimer = 0;
	int32_t AroundFireTimer = 0;

	int enemyLife = 100;

	//デスフラグ
	bool isDead_ = false;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;

	Affin* Affin_;

	Vector3 MoveYSpeed;

	Vector3 MoveXSpeed;


	Phase phase_ = Phase::MoveX;

	//乱数
	std::random_device rnd;

	////弾
	//std::list<std::unique_ptr<EnemyBullet>> bullets_;

	

public:

	//プレイヤー
	Player* player_ = nullptr;

	//ゲームシーン
	GameScene* gamescene_ = nullptr;

	//初期化
	void Initialize(Model* model,Vector3 pos);

	//更新処理
	void Update();
	
	//接近フェーズ
	void MoveY();

	//離脱フェーズ
	void MoveX();

	//停止フェーズ
	void Stop();

	//描画処理
	void Draw(const ViewProjection& viewProjection);

	//自機狙い弾発射
	void PlayerAim();
	//自機の周り
	void Cross();
	//フェーズ初期化
	void PhaseInitialize();

	//プレイヤーセッター
	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	//衝突判定
	void OnCollision();

	//ゲームシーンヘッダー
	void SetGameScene(GameScene* gamescene) { gamescene_ = gamescene; }

	//敵の消滅
	bool IsDead()const { return isDead_; }

	void RandomPhase();

	void EnemyDead();

	bool GetEnemyIsDead();
};

