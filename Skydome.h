#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include"affin/Affin.h"
class Skydome
{

public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ViewProjection viewProjection_);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�A�t�B��
	Affin* affin_;
};

