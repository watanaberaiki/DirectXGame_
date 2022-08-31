﻿#pragma once

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

//#include"Player.h"

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
	Enemy* enemy_ = nullptr;
	float enemyRadius = 1.0f;
	float enemyBulletRadius =1.0f;

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

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
