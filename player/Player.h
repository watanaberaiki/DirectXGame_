#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "DebugText.h"
#include "Input.h"
#include"Affin.h"
#include"PlayerBullet.h"
#include<memory>
#include<list>
class Player {

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	Affin* Affin_;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;



public:
	///<summary>
	///初期化
	///</summary>
	///<param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	///< summary>
	///初期化
	///</summary>
	void Update(ViewProjection viewProjection_);

	///< summary>
	///初期化
	///</summary>
	/*void Move();*/

	///< summary>
	///初期化
	///</summary>
	void Draw(ViewProjection viewProjection_);


	//回転
	void Rotate();

	//攻撃
	void Attack();

	//ワールド座標を取得
	Vector3 GetWorldPosition();
};
