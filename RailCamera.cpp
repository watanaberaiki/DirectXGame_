#include "RailCamera.h"

void RailCamera::Initialize(const Vector3 pos, const Vector3 angle) {


	//引数で受け取ったのを代入
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_ = angle;
	worldTransform_.Initialize();
	//ビュープロジェクション初期化
	viewProjection_.farZ = 200.0f;
	viewProjection_.Initialize();

	debugText_ = DebugText::GetInstance();

	input_ = Input::GetInstance();
}

void RailCamera::Update() {
	//移動
	Vector3 cameraMove = { 0,0,0 };
	//回転
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

	//ワールド行列を設定する
	worldTransform_.matWorld_ =
		affin_->MatWorld(
			affin_->Scale(Vector3(1, 1, 1)),
			affin_->Rotate(
				affin_->RotateX(worldTransform_.rotation_.x),
				affin_->RotateY(worldTransform_.rotation_.y),
				affin_->RotateZ(worldTransform_.rotation_.z)),
			affin_->Translation(worldTransform_.translation_));

	//ワールド行列再計算
	worldTransform_.TransferMatrix();

	viewProjection_.eye = worldTransform_.translation_;

	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	forward = affin_->MatVector(forward, worldTransform_);
	//視点から前方に適当な距離進んだ位置が注視点
	forward += viewProjection_.eye;
	viewProjection_.target =  forward;
	//ワールド上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映(レールカメラの上方ベクトル)
	viewProjection_.up = affin_->MatVector(up, worldTransform_);
	//ビュープロジェクションを更新
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