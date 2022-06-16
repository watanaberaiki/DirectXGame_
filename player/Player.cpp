#include "Player.h"
#include <cassert>
#include"PrimitiveDrawer.h"
#include"Affin.h"

void Player::Initialize(Model* model, uint32_t textureHandle){
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;
	
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

};

void Player::Update(ViewProjection viewProjection_) {
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = {0, 0, 0};

	//�L�����N�^�[�̃X�s�[�h
	const float charaSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move = {-charaSpeed, 0, 0};
	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {+charaSpeed, 0, 0};
	} else if (input_->PushKey(DIK_UP)) {
		move = {0, +charaSpeed, 0};
	} else if (input_->PushKey(DIK_DOWN)) {
		move = {0, -charaSpeed, 0};
	}

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;


	//���[���h�s���ݒ肷��
	worldTransform_.matWorld_ = Affin_->MatWorld(
		Affin_->Scale(worldTransform_.scale_),
		Affin_->Rotate(Affin_->RotateX(worldTransform_.rotation_.x), 
			Affin_->RotateY(worldTransform_.rotation_.y),
			Affin_->RotateZ(worldTransform_.rotation_.z)),
		Affin_->Translation(worldTransform_.translation_));

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

	//�L�[�{�[�h���͂ɂ��ړ�����	
	//�ړ����E���W

	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

}
void Player::Draw(ViewProjection viewProjection_){ 
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
