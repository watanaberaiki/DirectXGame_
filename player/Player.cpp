#include "Player.h"
#include <cassert>
#include"PrimitiveDrawer.h"
#include"Affin.h"
#include"GameScene.h"


void Player::Initialize(Model* model, Model* bulletmodel) {
	//NULLポインタチェック
	assert(model);

	model_ = model;
	bulletModel_ = bulletmodel;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0,0,50 };

	worldTransform3DReticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("reticle.png");
	//スプライト生成
	sprite2DReticle_.reset(Sprite::Create(textureReticle, Vector2{ 640,360 }, Vector4{ 1,1,1,1 }, Vector2(0.5f, 0.5f)));
};

void Player::Update(ViewProjection viewProjection) {
	//当たったらライフ減少
	if (isHit_ == true) {
		playerLife_ -= 1;
		isHit_ = false;
	}
	DebugText::GetInstance()->SetPos(20, 80);
	DebugText::GetInstance()->Printf(
		"playerLife_:(%d)", playerLife_);

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr <PlayerBullet>& bullet) {
		return bullet->IsDead();
		});


	//キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	//キャラクターのスピード
	const float charaSpeed = 0.2f;
	//キーボード入力による移動処理	
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


	////ゲームパッドの状態を得る変数（XINPUT）
	//XINPUT_STATE joyState;
	////ジョイスティック状態取得
	//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * charaSpeed;
	//	move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * charaSpeed;
	//}


	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	//移動限界座標

	const float kMoveLimitX = 13.0f;
	const float kMoveLimitY = 7.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	////キャラクターの旋回処理
	//Rotate();

	//ワールド行列を設定する
	worldTransform_.matWorld_ = Affin_->MatWorld(
		Affin_->Scale(worldTransform_.scale_),
		Affin_->Rotate(Affin_->RotateX(worldTransform_.rotation_.x),
			Affin_->RotateY(worldTransform_.rotation_.y),
			Affin_->RotateZ(worldTransform_.rotation_.z)),
		Affin_->Translation(worldTransform_.translation_));


	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	worldTransform_.TransferMatrix();


	//キャラクターの攻撃処理
	Attack();

	//弾発射
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//自機のワールド座標化から3Dレティクルのワールド座標を計算
	{
		//自機から3Dレティクルの距離
		const float kDistancePlayerTo3DReticle = 20.0f;
		//自機から3Dレティクルへのオフセット
		Vector3 offSet = { 0, 0, 1.0f };
		//自機のワールド座標の回転を反映
		offSet = Affin_->MatVector(offSet, worldTransform_);
		float len = std::sqrt(offSet.x * offSet.x + offSet.y * offSet.y + offSet.z * offSet.z);

		offSet /= len;
		offSet *= kDistancePlayerTo3DReticle;
		worldTransform3DReticle_.translation_ = offSet += Affin_->GetWorldPosition(worldTransform_);

		//ワールド行列を設定する
		worldTransform3DReticle_.matWorld_ = Affin_->MatWorld(
			Affin_->Scale(worldTransform3DReticle_.scale_),
			Affin_->Rotate(Affin_->RotateX(worldTransform3DReticle_.rotation_.x),
				Affin_->RotateY(worldTransform3DReticle_.rotation_.y),
				Affin_->RotateZ(worldTransform3DReticle_.rotation_.z)),
			Affin_->Translation(worldTransform3DReticle_.translation_));

		worldTransform3DReticle_.TransferMatrix();

	}

	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		// 3Dレティクルのワールド行列から,ワールド座標を取得
		Vector3 positionReticle = Affin_->GetWorldPosition(worldTransform3DReticle_);

		//ビューポート行列
		Matrix4 Viewport =
		{ 1280 / 2,0,0,0,
		0,-720 / 2,0,0,
		0,0,1,0,
		1280 / 2, 720 / 2,0,1 };

		//ビューポート行列
		Matrix4 matViewProjectionViewport;
		matViewProjectionViewport = viewProjection.matView;
		matViewProjectionViewport *= viewProjection.matProjection;
		matViewProjectionViewport *= Viewport;

		positionReticle = Affin_->WDivision(matViewProjectionViewport, positionReticle);

		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	}

	//マウスカーソル
	{
		POINT mousePosition;
		//マウス座標(スクリーン座標)を取得する
		GetCursorPos(&mousePosition);

		//クライアントエリア座標に変換する
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		//マウス座標を2Dレティクルに代入
		sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));



		//ビューポート行列
		Matrix4 Viewport =
		{ 1280 / 2,0,0,0,
		0,-720 / 2,0,0,
		0,0,1,0,
		1280 / 2, 720 / 2,0,1 };

		// ビュープロジェクションビューポート合成
		Matrix4 matVPV = viewProjection.matView *= viewProjection.matProjection *= Viewport;

		//合成行列の逆行列を計算する
		Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);

		//スクリーン座標			  ↓マウスかコントローラーか
		Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
		Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);

		//スクリーン座標系からワールド座標系へ
		posNear = Affin_->WDivision(matInverseVPV, posNear);
		posFar = Affin_->WDivision(matInverseVPV, posFar);

		//マウスレイの方向
		Vector3 mouseDirection = posFar -= posNear;

		float len = sqrt(mouseDirection.x * mouseDirection.x + mouseDirection.y * mouseDirection.y + mouseDirection.z * mouseDirection.z);
		if (len != 0)
		{
			mouseDirection /= len;
		}

		//カメラから照準オブジェクトの距離
		const float kDistanceTestObject = 180.0f;
		worldTransform3DReticle_.translation_ = posNear += (mouseDirection *= kDistanceTestObject);

		//ワールド行列を設定する
		worldTransform3DReticle_.matWorld_ = Affin_->MatWorld(
			Affin_->Scale(worldTransform3DReticle_.scale_),
			Affin_->Rotate(Affin_->RotateX(worldTransform3DReticle_.rotation_.x),
				Affin_->RotateY(worldTransform3DReticle_.rotation_.y),
				Affin_->RotateZ(worldTransform3DReticle_.rotation_.z)),
			Affin_->Translation(worldTransform3DReticle_.translation_));

		worldTransform3DReticle_.TransferMatrix();

		////スプライトの現在座標を取得(コントローラー用)
		//Vector2 spritePosition = sprite2DReticle_->GetPosition();

		//XINPUT_STATE joyState;

		////ジョイスティック状態
		//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		//	spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		//	spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		//	//スプライトの座標変更を反映
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

	//弾描画
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
		//弾の速度
		const float kBulletSpeed = 2.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを目線の動きに合わせて回転させる
		velocity = Affin_->GetWorldPosition(worldTransform3DReticle_) -= Affin_->GetWorldPosition(worldTransform_);
		//正規化
		float len = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
		velocity /= len;

		velocity *= kBulletSpeed;
		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(bulletModel_, GetWorldPosition(), velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

	}

	//XINPUT_STATE joyState;
	////コントローラーが接続してなかったら抜ける
	//if (!Input::GetInstance()->GetJoystickState(0, joyState))
	//{
	//	return;
	//}
	//if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) 
	//{
	//	//弾の速度
	//	const float kBulletSpeed = 1.0f;
	//	Vector3 velocity(0, 0, kBulletSpeed);

	//	//速度ベクトルを目線の動きに合わせて回転させる
	//	velocity = Affin_->GetWorldPosition(worldTransform3DReticle_) -= Affin_->GetWorldPosition(worldTransform_);
	//	//正規化
	//	float len = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	//	velocity /= len;

	//	velocity *= kBulletSpeed;
	//	//弾を生成し、初期化
	//	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//	newBullet->Initialize(model_, GetWorldPosition(), velocity);

	//	//弾を登録する
	//	bullets_.push_back(std::move(newBullet));
	//}
}

Vector3 Player::GetWorldPosition() {
	//ワールドに入れる座標
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得(ワールド座標)
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
//レティクル
void Player::DrawUI() {
	sprite2DReticle_->Draw();
}
int Player::GetPlayerLife() {
	return playerLife_;
}