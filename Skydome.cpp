#include "Skydome.h"

/// ������
void Skydome::Initialize(Model* model) {
	// NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.scale_ = Vector3(150, 150, 150);
	//���[���h�s���ݒ肷��
	worldTransform_.matWorld_ = affin_->MatWorld(
		affin_->Scale(worldTransform_.scale_),
		affin_->Rotate(affin_->RotateX(worldTransform_.rotation_.x),
			affin_->RotateY(worldTransform_.rotation_.y),
			affin_->RotateZ(worldTransform_.rotation_.z)),
		affin_->Translation(worldTransform_.translation_));

	worldTransform_.TransferMatrix();

}


/// ���t���[������
void Skydome::Update() {

}


/// �`��
void Skydome::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_);
}