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

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//立方体
	Vector3 vec3;

	Vector3 objectVertex[8];

	Vector3 moveObjectVertex[8];

	Vector3 scaleObjectVertex[8];

	Vector3 rotateObjectVertex[8];

	int edgeList[12][2] = {
	  {0, 1},
      {1, 2},
      {2, 3},
      {3, 0},
      {0, 4},
      {1, 5},
	  {2, 6},
      {3, 7},
      {4, 5},
      {5, 6},
      {6, 7},
      {7, 4}
    };

	//平行移動宣言
	float move[4][4] = {
	  {1.0f, 0.0f, 0.0f, 20.0f},
	  {0.0f, 1.0f, 0.0f, 20.0f},
	  {0.0f, 0.0f, 1.0f, 20.0f},
	  {0.0f, 0.0f, 0.0f, 1.0f }
    };

	//拡大縮小宣言
	float scale[4][4] = {
	  {2.0f, 0.0f, 0.0f, 0.0f},
	  {0.0f, 2.0f, 0.0f, 0.0f},
	  {0.0f, 0.0f, 2.0f, 0.0f},
	  {0.0f, 0.0f, 0.0f, 1.0f}
    };

	//回転宣言
	double rotate[4][4] = {
	  {cos(3.14 / 4), -sin(3.14 / 4), 0.0f, 0.0f},
	  {sin(3.14 / 4), cos(3.14 / 4),  0.0f, 0.0f},
	  {0.0f,          0.0f,           1.0f, 0.0f},
	  {0.0f,          0.0f,           0.0f, 1.0f}
    };

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
