#pragma once

#include"WorldTransform.h"
#include"ViewProjection.h"
#include "Input.h"
#include "DebugText.h"
#include "affin/Affin.h"

class RailCamera
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const Vector3 pos,const Vector3 angle);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	ViewProjection GetViewProjection();

	WorldTransform* GetWorldTransform();
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	Affin* affin_;

};

