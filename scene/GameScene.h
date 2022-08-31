#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"Enemy.h"
#include"Player.h"
#include"Skydome.h"
#include"RailCamera.h"
#include<sstream>


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//衝突判定
	void CheckAllCollisions();

	//敵弾を追加する
	void AddEnemyBullet(std::unique_ptr<EnemyBullet>&enemybullet);

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return enemyBullets_; }

	void MakeEnemy(Vector3 pos);

	void LoadEnemyPopData();

	void UpdateEnemyPopCommands();


  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;
	float playerRadius = 1.0;
	float playerBulletRadius = 1.0;

	//敵キャラ
	/*Enemy* enemy_ = nullptr;*/
	std::list<std::unique_ptr<Enemy>> enemys_;
	float enemyRadius = 1.0f;
	float enemyBulletRadius =1.0f;

	//敵弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;

	//天球
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;

	//レールカメラ
	RailCamera* railcamera_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//テクスチャハンドル
	uint32_t playertextureHandle_ = 0;
	//テクスチャハンドル
	uint32_t enemytextureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;
	
	//デバッグカメラ有効
	bool isDebugCameraActive_ =false;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	bool isWait_ = false;

	int waitTimer = 100;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
