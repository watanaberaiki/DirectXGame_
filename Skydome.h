#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include"affin/Affin.h"
class Skydome
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection viewProjection_);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//アフィン
	Affin* affin_;
};

