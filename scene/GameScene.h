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

	/// <summary>
	/// ラジアン変換
	/// </summary>
	float Radian(float angle);

	/// <summary>
	/// 度数変換
	/// </summary>
	float Angle(float Radian);

	/// <summary>
	/// clamp
	/// </summary>
	float clamp(float value, float min, float max);

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
	

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//π
	float PI;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
