#pragma once

#include"WorldTransform.h"
#include"ViewProjection.h"
#include "Input.h"
#include "DebugText.h"
#include "affin/Affin.h"

class RailCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3 pos,const Vector3 angle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	ViewProjection GetViewProjection();

	WorldTransform* GetWorldTransform();
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	Affin* affin_;

};

