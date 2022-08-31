#include "RailCamera.h"

void RailCamera::Initialize(const Vector3 pos, const Vector3 angle) {


	//�����Ŏ󂯎�����̂���
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = angle;
	worldTransform_.Initialize();
	//�r���[�v���W�F�N�V����������
	viewProjection_.farZ = 200.0f;
	viewProjection_.Initialize();

	debugText_ = DebugText::GetInstance();

	input_ = Input::GetInstance();
}

void RailCamera::Update() {
	//�ړ�
	Vector3 cameraMove = { 0,0,0 };
	//��]
	Vector3 cameraAngle = { 0,0,0 };

	const float movespeed = 0.1f;

	const float anglespeed = 0.01f;

	if (input_->PushKey(DIK_Z))
	{
		cameraMove += {0, 0, movespeed};
	}
	if (input_->PushKey(DIK_X))
	{
		cameraMove -= {0, 0, movespeed};
	}
	if (input_->PushKey(DIK_A))
	{
		cameraAngle += {0, 0, anglespeed};
	}
	if (input_->PushKey(DIK_D))
	{
		cameraAngle -= {0, 0, anglespeed};
	}


	worldTransform_.translation_ += cameraMove;
	worldTransform_.rotation_ += cameraAngle;

	//���[���h�s���ݒ肷��
	worldTransform_.matWorld_ =
		affin_->MatWorld(
			affin_->Scale(Vector3(1, 1, 1)),
			affin_->Rotate(
				affin_->RotateX(worldTransform_.rotation_.x),
				affin_->RotateY(worldTransform_.rotation_.y),
				affin_->RotateZ(worldTransform_.rotation_.z)),
			affin_->Translation(worldTransform_.translation_));

	//���[���h�s��Čv�Z
	worldTransform_.TransferMatrix();

	viewProjection_.eye = worldTransform_.translation_;

	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = affin_->MatVector(forward, worldTransform_);
	//���_����O���ɓK���ȋ����i�񂾈ʒu�������_
	forward += viewProjection_.eye;
	viewProjection_.target =  forward;
	//���[���h����x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f(���[���J�����̏���x�N�g��)
	viewProjection_.up = affin_->MatVector(up, worldTransform_);
	//�r���[�v���W�F�N�V�������X�V
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	debugText_->SetPos(0, 200);
	debugText_->Printf("eye=%f,%f,%f", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}

ViewProjection RailCamera::GetViewProjection() {
	return viewProjection_;

}

WorldTransform* RailCamera::GetWorldTransform() {
	return &worldTransform_;
}