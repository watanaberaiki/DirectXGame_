#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include"PrimitiveDrawer.h"
#include<list>
#include<math.h>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	//自キャラの解放
	delete player_;
	delete enemy_;
	delete skydome_;
	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//3Dモデルの生成
	model_ = Model::Create();
	//3Dモデルの生成
	modelSkydome_=Model::CreateFromOBJ("skydome", true);
	//ファイル名を指定してテクスチャを読み込む
	playertextureHandle_ = TextureManager::Load("mario.jpg");
	//テクスチャ読み込み
	enemytextureHandle_ = TextureManager::Load("potta-.jpg");

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);



	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, playertextureHandle_);

	//敵キャラの生成
	enemy_ = new Enemy();
	enemy_->Initialize(model_, enemytextureHandle_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);


	//天球生成
	skydome_ = new Skydome();
	//天球初期化
	skydome_->Initialize(modelSkydome_);
	
	//レールカメラ生成
	railcamera_ = new RailCamera();
	//レールカメラ初期化
	railcamera_->Initialize(Vector3(0,0,-50),Vector3(0,0,0));

	//プレイヤーとカメラ親子
	player_->SetParent(railcamera_->GetWorldTransform());
}

void GameScene::Update() {

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_C) && !isDebugCameraActive_) {
		isDebugCameraActive_ = true;
	}
#endif
	//カメラの更新
	if (isDebugCameraActive_) {
		/*デバッグカメラの更新*/
		debugCamera_->Update();
	}




	//自キャラの更新
	player_->Update();
	//敵キャラの更新
	enemy_->Update();
	//天球の更新
	skydome_->Update();
	//レールカメラの更新
	railcamera_->Update();

	//行列の再計算
	viewProjection_.UpdateMatrix();

	CheckAllCollisions();

	debugText_->SetPos(10, 30);
	debugText_->Printf("%d", isDebugCameraActive_);
	debugText_->SetPos(10, 50);
	debugText_->Printf("eye=%f,%f,%f", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//自キャラの描画
	player_->Draw(railcamera_->GetViewProjection());
	//敵キャラの描画
	enemy_->Draw(railcamera_->GetViewProjection());
	//天球の描画
	skydome_->Draw(railcamera_->GetViewProjection());


	////自キャラの描画
	//player_->Draw(viewProjection_);
	////敵キャラの描画
	//enemy_->Draw(viewProjection_);
	////天球の描画
	//skydome_->Draw(viewProjection_);
	 
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	//判定対象のAとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet.get()->GetWorldPosition();

		float a = posB.x - posA.x;
		float b = posB.y - posA.y;
		float c = posB.z - posA.z;

		float d = (a * a) + (b * b) + (c * c);

		float r = (playerRadius + enemyBulletRadius) * (playerRadius + enemyBulletRadius);

		if (d <= r) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();

		}
	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	//敵キャラの座標
	posA = enemy_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//自弾の座標
		posB = bullet.get()->GetWorldPosition();

		float a = posB.x - posA.x;
		float b = posB.y - posA.y;
		float c = posB.z - posA.z;

		float d = (a * a) + (b * b) + (c * c);

		float r = (playerRadius + enemyBulletRadius) * (playerRadius + enemyBulletRadius);

		if (d <= r) {
			//敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定

	//自弾と敵弾すべての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		for (const std::unique_ptr<EnemyBullet>& bullet2 : enemyBullets) {

			//自弾の座標
			posA = bullet.get()->GetWorldPosition();

			//敵弾の座標
			posB = bullet2.get()->GetWorldPosition();

			float a = posB.x - posA.x;
			float b = posB.y - posA.y;
			float c = posB.z - posA.z;

			float d = (a * a) + (b * b) + (c * c);

			float r = (playerRadius + enemyBulletRadius) * (playerRadius + enemyBulletRadius);

			if (d <= r) {
				//自キャラの衝突時コールバックを呼び出す
				bullet->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				bullet2->OnCollision();
			}
		}
	}

#pragma endregion
}