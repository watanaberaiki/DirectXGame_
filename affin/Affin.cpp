#include "Affin.h"
Affin::Affin() {

}

Affin::~Affin() {

}


Matrix4 Affin::Scale(Vector3 worldTransform_scale_) {

	//�P�ʍs���������
	Matrix4 matScale = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	//�X�P�[�����O�{�����s��ɐݒ肷��
	matScale.m[0][0] = worldTransform_scale_.x;
	matScale.m[1][1] = worldTransform_scale_.y;
	matScale.m[2][2] = worldTransform_scale_.z;

	return matScale;
}

Matrix4 Affin::RotateX(float worldTransform_rotation_x) {
	// X����]�s���錾���ĒP�ʍs���������
	Matrix4 matRotX = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	// X����]�s��ɉ�]�p��ݒ肷��
	matRotX.m[1][1] = cos(worldTransform_rotation_x);
	matRotX.m[1][2] = sin(worldTransform_rotation_x);
	matRotX.m[2][1] = -sin(worldTransform_rotation_x);
	matRotX.m[2][2] = cos(worldTransform_rotation_x);

	return matRotX;
}

Matrix4 Affin::RotateY(float worldTransform_rotation_y) {
	// Y����]�s���錾���ĒP�ʍs���������
	Matrix4 matRotY = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	// Y����]�s��ɉ�]�p��ݒ肷��
	matRotY.m[0][0] = cos(worldTransform_rotation_y);
	matRotY.m[0][2] = -sin(worldTransform_rotation_y);
	matRotY.m[2][0] = sin(worldTransform_rotation_y);
	matRotY.m[2][2] = cos(worldTransform_rotation_y);

	return matRotY;
}

Matrix4 Affin::RotateZ(float worldTransform_rotate_z) {
	// Z����]�s���錾���ĒP�ʍs���������
	Matrix4 matRotZ = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	// Z����]�s��ɉ�]�p��ݒ肷��
	matRotZ.m[0][0] = cos(worldTransform_rotate_z);
	matRotZ.m[0][1] = sin(worldTransform_rotate_z);
	matRotZ.m[1][0] = -sin(worldTransform_rotate_z);
	matRotZ.m[1][1] = cos(worldTransform_rotate_z);

	return matRotZ;
}

Matrix4 Affin::Rotate(Matrix4 RotateX, Matrix4 RotateY, Matrix4 RotateZ) {
	Matrix4 matRot = RotateX *= RotateY *= RotateZ;
	
	return matRot;
}

Matrix4 Affin::Translation(Vector3 worldTransform_translation_) {
	//���s�ړ��s���錾���ĒP�ʍs���������
	Matrix4 matTrans = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	//���s�ړ����s��ɐݒ肷��
	matTrans.m[3][0] = worldTransform_translation_.x;
	matTrans.m[3][1] = worldTransform_translation_.y;
	matTrans.m[3][2] = worldTransform_translation_.z;

	return matTrans;
}

Matrix4 Affin::MatWorld(Matrix4 matScale, Matrix4 matRot, Matrix4 matTrans) {

	WorldTransform worldTransform;
	worldTransform.matWorld_ = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	//���[���h�g�����X�t�H�[���ɃX�P�[�����O�s���������
	worldTransform.matWorld_ *= matScale *= matRot *= matTrans;

	return worldTransform.matWorld_;
}