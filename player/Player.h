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
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	Affin* Affin_;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//3D���e�B�N���p�g�����X�t�H�[��
	WorldTransform worldTransform3DReticle_;

	//2D���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite>sprite2DReticle_;

	int playerLife_ = 5;

	int isHit_ = false;

	const float gravity = 0.2f;

	int isUp = 0;

public:

	///<summary>
	///������
	///</summary>
	///<param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, Model* bulletmodel);

	///< summary>
	///������
	///</summary>
	void Update(ViewProjection viewProjection);

	///< summary>
	///������
	///</summary>
	/*void Move();*/

	///< summary>
	///������
	///</summary>
	void Draw(ViewProjection viewProjection_);


	//��]
	void Rotate();

	//�U��
	void Attack();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˔���
	void OnCollision();

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	void SetParent(WorldTransform* worldTransform);

	void DrawUI();

	int GetPlayerLife();

};
