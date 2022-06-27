#pragma once
#include "WorldTransform.h"
#include "math.h"


class Affin
{
public: // メンバ関数
/// <summary>
/// コンストクラタ
/// </summary>
	Affin();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Affin();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

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
	/// Z軸周りの回転
	/// </summary>
	Matrix4 Rotate(Matrix4 RotateX, Matrix4 RotateY, Matrix4 RotateZ);
	
	/// <summary>
	/// 平行移動
	/// </summary>
	Matrix4 Translation(Vector3 worldTransform_translation_);

	/// <summary>
	/// ワールド行列の生成
	/// </summary>
	Matrix4 MatWorld(Matrix4 matScale, Matrix4 matRot, Matrix4 matTrans);

	/// <summary>
	/// ベクトルと行列の掛け算
	/// </summary>
	Vector3 MatVector(Vector3 vector, WorldTransform worldTransform);
};

