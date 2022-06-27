#pragma once
#include "WorldTransform.h"
#include "math.h"


class Affin
{
public: // �����o�֐�
/// <summary>
/// �R���X�g�N���^
/// </summary>
	Affin();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Affin();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�P�[��
	/// </summary>
	Matrix4 Scale(Vector3 worldTransform_scale_);

	/// <summary>
	/// X������̉�]
	/// </summary>
	Matrix4 RotateX(float worldTransform_rotation_x);

	/// <summary>
	/// Y������̉�]
	/// </summary>
	Matrix4 RotateY(float worldTransform_rotation_y);

	/// <summary>
	/// Z������̉�]
	/// </summary>
	Matrix4 RotateZ(float worldTransform_rotation_z);
	
	/// <summary>
	/// Z������̉�]
	/// </summary>
	Matrix4 Rotate(Matrix4 RotateX, Matrix4 RotateY, Matrix4 RotateZ);
	
	/// <summary>
	/// ���s�ړ�
	/// </summary>
	Matrix4 Translation(Vector3 worldTransform_translation_);

	/// <summary>
	/// ���[���h�s��̐���
	/// </summary>
	Matrix4 MatWorld(Matrix4 matScale, Matrix4 matRot, Matrix4 matTrans);

	/// <summary>
	/// �x�N�g���ƍs��̊|���Z
	/// </summary>
	Vector3 MatVector(Vector3 vector, WorldTransform worldTransform);
};

