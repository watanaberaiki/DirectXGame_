#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "DebugText.h"
#include "Input.h"
#include"affin/Affin.h"
#include"playerbullet/PlayerBullet.h"
#include<memory>
#include<list>
#include "TextureManager.h"
#include<WinApp.h>
#include"MathUtility.h"


class Player {

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	Affin* Affin_;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//3Dレティクル用トランスフォーム
	WorldTransform worldTransform3DReticle_;

	//2Dレティクル用スプライト
	std::unique_ptr<Sprite>sprite2DReticle_;

	int playerLife_ = 5;

	int isHit_ = false;

	const float gravity = 0.2f;

	int isUp = 0;

public:

	///<summary>
	///初期化
	///</summary>
	///<param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, Model* bulletmodel);

	///< summary>
	///初期化
	///</summary>
	void Update(ViewProjection viewProjection);

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

	//衝突判定
	void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	void SetParent(WorldTransform* worldTransform);

	void DrawUI();

	int GetPlayerLife();

};
