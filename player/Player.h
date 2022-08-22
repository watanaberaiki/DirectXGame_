#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "DebugText.h"
#include "Input.h"
#include"Affin.h"
#include"PlayerBullet.h"
#include<memory>
#include<list>
class Player {

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	Affin* Affin_;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;



public:
	///<summary>
	///������
	///</summary>
	///<param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);

	///< summary>
	///������
	///</summary>
	void Update(ViewProjection viewProjection_);

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
};
