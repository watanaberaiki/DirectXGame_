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

	/// <summary>
	/// スケール
	/// </summary>
	Matrix4 Scale(Vector3 worldTransform_scale_);

	/// <summary>
	/// X軸周りの回転
	/// </summary>
	Matrix4 RotateX(float worldTransform_rotation_x);

	/// <summary>
	/// Y軸周りの回転
	/// </summary>
	Matrix4 RotateY(float worldTransform_rotation_y);

	/// <summary>
	/// Z軸周りの回転
	/// </summary>
	Matrix4 RotateZ(float worldTransform_rotation_z);

	/// <summary>
	/// 平行移動
	/// </summary>
	Matrix4 Translation(Vector3 worldTransform_translation_);

	/// <summary>
	/// ワールド行列の生成
	/// </summary>
	Matrix4 MatWorld( Matrix4 matScale, Matrix4 matRot, Matrix4 matTrans);

  private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransforms_[100];
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;
	
	//π
	float PI;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
