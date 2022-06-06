#include "Player.h"
#include <cassert>
#include"PrimitiveDrawer.h"


void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	
	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

};

void Player::Update(ViewProjection viewProjection_) {
	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターのスピード
	const float charaSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move = {-charaSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {+charaSpeed, 0, 0};
	} else if (input_->PushKey(DIK_UP)) {
		move = {0, -charaSpeed, 0};
	} else if (input_->PushKey(DIK_DOWN)) {
		move = {0, +charaSpeed, 0};
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	debugText_->SetPos(50, 150);
	debugText_->Printf(
	"(%f,%f,%f)", 
	worldTransform_.translation_.x, 
	worldTransform_.translation_.y,
	worldTransform_.translation_.z);

	debugText_->SetPos(50, 100);
	debugText_->Printf(
	  "(%f,%f,%f)", 
		viewProjection_.eye.x, 
		viewProjection_.eye.y, 
		viewProjection_.eye.z);

	worldTransform_.TransferMatrix();

}
void Player::Draw(ViewProjection viewProjection_){ 
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
