#include "Player.h"
#include <cassert>
#include"PrimitiveDrawer.h"
#include"Affin.h"
#include"GameScene.h"


void Player::Initialize(Model* model, Model* bulletmodel) {
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	bulletModel_ = bulletmodel;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,50 };

	worldTransform3DReticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("reticle.png");
	//�X�v���C�g����
	sprite2DReticle_.reset(Sprite::Create(textureReticle, Vector2{ 640,360 }, Vector4{ 1,1,1,1 }, Vector2(0.5f, 0.5f)));
};

void Player::Update(ViewProjection viewProjection) {
	//���������烉�C�t����
	if (isHit_ == true) {
		playerLife_ -= 1;
		isHit_ = false;
	}
	DebugText::GetInstance()->SetPos(20, 80);
	DebugText::GetInstance()->Printf(
		"playerLife_:(%d)", playerLife_);

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr <PlayerBullet>& bullet) {
		return bullet->IsDead();
		});


	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0, 0, 0 };

	//�L�����N�^�[�̃X�s�[�h
	const float charaSpeed = 0.2f;
	//�L�[�{�[�h���͂ɂ��ړ�����	
	if (input_->PushKey(DIK_A)) {
		move = { -charaSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_D)) {
		move = { +charaSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_W)) {
		move = { 0, +charaSpeed, 0 };
	}
	else if (input_->PushKey(DIK_S)) {
		move = { 0, -charaSpeed, 0 };
	}


	////�Q�[���p�b�h�̏�Ԃ𓾂�ϐ��iXINPUT�j
	//XINPUT_STATE joyState;
	////�W���C�X�e�B�b�N��Ԏ擾
	//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * charaSpeed;
	//	move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * charaSpeed;
	//}


	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	//�ړ����E���W

	const float kMoveLimitX = 13.0f;
	const float kMoveLimitY = 7.0f;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	////�L�����N�^�[�̐��񏈗�
	//Rotate();

	//���[���h�s���ݒ肷��
	worldTransform_.matWorld_ = Affin_->MatWorld(
		Affin_->Scale(worldTransform_.scale_),
		Affin_->Rotate(Affin_->RotateX(worldTransform_.rotation_.x),
			Affin_->RotateY(worldTransform_.rotation_.y),
			Affin_->RotateZ(worldTransform_.rotation_.z)),
		Affin_->Translation(worldTransform_.translation_));


	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	worldTransform_.TransferMatrix();


	//�L�����N�^�[�̍U������
	Attack();

	//�e����
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//���@�̃��[���h���W������3D���e�B�N���̃��[���h���W���v�Z
	{
		//���@����3D���e�B�N���̋���
		const float kDistancePlayerTo3DReticle = 20.0f;
		//���@����3D���e�B�N���ւ̃I�t�Z�b�g
		Vector3 offSet = { 0, 0, 1.0f };
		//���@�̃��[���h���W�̉�]�𔽉f
		offSet = Affin_->MatVector(offSet, worldTransform_);
		float len = std::sqrt(offSet.x * offSet.x + offSet.y * offSet.y + offSet.z * offSet.z);

		offSet /= len;
		offSet *= kDistancePlayerTo3DReticle;
		worldTransform3DReticle_.translation_ = offSet += Affin_->GetWorldPosition(worldTransform_);

		//���[���h�s���ݒ肷��
		worldTransform3DReticle_.matWorld_ = Affin_->MatWorld(
			Affin_->Scale(worldTransform3DReticle_.scale_),
			Affin_->Rotate(Affin_->RotateX(worldTransform3DReticle_.rotation_.x),
				Affin_->RotateY(worldTransform3DReticle_.rotation_.y),
				Affin_->RotateZ(worldTransform3DReticle_.rotation_.z)),
			Affin_->Translation(worldTransform3DReticle_.translation_));

		worldTransform3DReticle_.TransferMatrix();

	}

	//3D���e�B�N���̃��[���h���W����2D���e�B�N���̃X�N���[�����W���v�Z
	{
		// 3D���e�B�N���̃��[���h�s�񂩂�,���[���h���W���擾
		Vector3 positionReticle = Affin_->GetWorldPosition(worldTransform3DReticle_);

		//�r���[�|�[�g�s��
		Matrix4 Viewport =
		{ 1280 / 2,0,0,0,
		0,-720 / 2,0,0,
		0,0,1,0,
		1280 / 2, 720 / 2,0,1 };

		//�r���[�|�[�g�s��
		Matrix4 matViewProjectionViewport;
		matViewProjectionViewport = viewProjection.matView;
		matViewProjectionViewport *= viewProjection.matProjection;
		matViewProjectionViewport *= Viewport;

		positionReticle = Affin_->WDivision(matViewProjectionViewport, positionReticle);

		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	}

	//�}�E�X�J�[�\��
	{
		POINT mousePosition;
		//�}�E�X���W(�X�N���[�����W)���擾����
		GetCursorPos(&mousePosition);

		//�N���C�A���g�G���A���W�ɕϊ�����
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		//�}�E�X���W��2D���e�B�N���ɑ��
		sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));



		//�r���[�|�[�g�s��
		Matrix4 Viewport =
		{ 1280 / 2,0,0,0,
		0,-720 / 2,0,0,
		0,0,1,0,
		1280 / 2, 720 / 2,0,1 };

		// �r���[�v���W�F�N�V�����r���[�|�[�g����
		Matrix4 matVPV = viewProjection.matView *= viewProjection.matProjection *= Viewport;

		//�����s��̋t�s����v�Z����
		Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);

		//�X�N���[�����W			  ���}�E�X���R���g���[���[��
		Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
		Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);

		//�X�N���[�����W�n���烏�[���h���W�n��
		posNear = Affin_->WDivision(matInverseVPV, posNear);
		posFar = Affin_->WDivision(matInverseVPV, posFar);

		//�}�E�X���C�̕���
		Vector3 mouseDirection = posFar -= posNear;

		float len = sqrt(mouseDirection.x * mouseDirection.x + mouseDirection.y * mouseDirection.y + mouseDirection.z * mouseDirection.z);
		if (len != 0)
		{
			mouseDirection /= len;
		}

		//�J��������Ə��I�u�W�F�N�g�̋���
		const float kDistanceTestObject = 180.0f;
		worldTransform3DReticle_.translation_ = posNear += (mouseDirection *= kDistanceTestObject);

		//���[���h�s���ݒ肷��
		worldTransform3DReticle_.matWorld_ = Affin_->MatWorld(
			Affin_->Scale(worldTransform3DReticle_.scale_),
			Affin_->Rotate(Affin_->RotateX(worldTransform3DReticle_.rotation_.x),
				Affin_->RotateY(worldTransform3DReticle_.rotation_.y),
				Affin_->RotateZ(worldTransform3DReticle_.rotation_.z)),
			Affin_->Translation(worldTransform3DReticle_.translation_));

		worldTransform3DReticle_.TransferMatrix();

		////�X�v���C�g�̌��ݍ��W���擾(�R���g���[���[�p)
		//Vector2 spritePosition = sprite2DReticle_->GetPosition();

		//XINPUT_STATE joyState;

		////�W���C�X�e�B�b�N���
		//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//	spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		//	spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		//	//�X�v���C�g�̍��W�ύX�𔽉f
		//	sprite2DReticle_->SetPosition(spritePosition);
		//}

	/*	DebugText::GetInstance()->SetPos(20, 200);
		DebugText::GetInstance()->Printf(
			"Mouse ScreenPos:(%d,%d)", mousePosition.x, mousePosition.y);
		DebugText::GetInstance()->SetPos(20, 220);
		DebugText::GetInstance()->Printf("Near:(%f,%f,%f)", posNear.x, posNear.y, posNear.z);
		DebugText::GetInstance()->SetPos(20, 240);
		DebugText::GetInstance()->Printf("Far:(%f,%f,%f)", posFar.x, posFar.y, posFar.z);
		DebugText::GetInstance()->SetPos(20, 260);
		DebugText::GetInstance()->Printf(
			"MouseObject:(%f,%f,%f)", worldTransform3DReticle_.translation_.x,
			worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);*/

	}
}
void Player::Draw(ViewProjection viewProjection_) {

	model_->Draw(worldTransform_, viewProjection_);
	/*model_->Draw(worldTransform3DReticle_, viewProjection_, textureHandle_);*/

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}
}

void Player::Rotate() {
	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y -= 0.01;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y += 0.01;
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE) || input_->IsTriggerMouse(0)) {
		//�e�̑��x
		const float kBulletSpeed = 2.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g����ڐ��̓����ɍ��킹�ĉ�]������
		velocity = Affin_->GetWorldPosition(worldTransform3DReticle_) -= Affin_->GetWorldPosition(worldTransform_);
		//���K��
		float len = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		velocity /= len;

		velocity *= kBulletSpeed;
		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(bulletModel_, GetWorldPosition(), velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));

	}

	//XINPUT_STATE joyState;
	////�R���g���[���[���ڑ����ĂȂ������甲����
	//if (!Input::GetInstance()->GetJoystickState(0, joyState))
	//{
	//	return;
	//}
	//if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) 
	//{
	//	//�e�̑��x
	//	const float kBulletSpeed = 1.0f;
	//	Vector3 velocity(0, 0, kBulletSpeed);

	//	//���x�x�N�g����ڐ��̓����ɍ��킹�ĉ�]������
	//	velocity = Affin_->GetWorldPosition(worldTransform3DReticle_) -= Affin_->GetWorldPosition(worldTransform_);
	//	//���K��
	//	float len = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	//	velocity /= len;

	//	velocity *= kBulletSpeed;
	//	//�e�𐶐����A������
	//	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//	newBullet->Initialize(model_, GetWorldPosition(), velocity);

	//	//�e��o�^����
	//	bullets_.push_back(std::move(newBullet));
	//}
}

Vector3 Player::GetWorldPosition() {
	//���[���h�ɓ������W
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {
	isHit_ = true;
}

void Player::SetParent(WorldTransform* worldTransform) {
	worldTransform_.parent_ = worldTransform;
}
//���e�B�N��
void Player::DrawUI() {
	sprite2DReticle_->Draw();
}
int Player::GetPlayerLife() {
	return playerLife_;
}