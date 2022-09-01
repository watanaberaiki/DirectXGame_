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
#include <random>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
	enum class Scene {
		title,		//タイトル
		game,		//ゲーム中
		clear,		//クリア
		gameover,	//ゲームオーバー
	};

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

	void TitleUpdate();

	void GameUpdate();

	void ClearUpdate();

	void GameoverUpdate();

	void TitleDraw();

	void GameDraw();

	void ClearDraw();

	void GameoverDraw();

	void GameoverChange();

	void ClearChange();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;
	float playerRadius = 1.0f;
	float playerBulletRadius = 1.0f;
	int playerLife = 0;
	Model* modelPlayer_ = nullptr;

	//敵キャラ
	/*Enemy* enemy_ = nullptr;*/
	std::list<std::unique_ptr<Enemy>> enemys_;
	float enemyRadius = 8.0f;
	float enemyBulletRadius =1.0f;
	Model* modelEnemy_ = nullptr;
	bool enemyIsDead = false;

	//弾
	Model* modelPlayerbullet_ = nullptr;

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
	//テクスチャハンドル
	uint32_t titletextureHandle_ = 0;
	//テクスチャハンドル
	uint32_t gameovertextureHandle_ = 0;
	//テクスチャハンドル
	uint32_t cleartextureHandle_ = 0;


	//タイトルレティクル用スプライト
	std::unique_ptr<Sprite>title_;

	//タイトルレティクル用スプライト
	std::unique_ptr<Sprite>gameover_;

	//タイトルレティクル用スプライト
	std::unique_ptr<Sprite>clear_;

	//3Dモデル
	Model* model_ = nullptr;
	
	//デバッグカメラ有効
	bool isDebugCameraActive_ =false;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	bool isWait_ = false;

	int waitTimer = 100;

	Scene scene_ = Scene::title;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
