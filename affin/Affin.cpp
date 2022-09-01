#include "Affin.h"
#include<cmath>  //sprt
Affin::Affin() {

}

Affin::~Affin() {

}


Matrix4 Affin::Scale(Vector3 worldTransform_scale_) {

	//単位行列を代入する
	Matrix4 matScale = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	//スケーリング倍率を行列に設定する
	matScale.m[0][0] = worldTransform_scale_.x;
	matScale.m[1][1] = worldTransform_scale_.y;
	matScale.m[2][2] = worldTransform_scale_.z;

	return matScale;
}

Matrix4 Affin::RotateX(float worldTransform_rotation_x) {
	// X軸回転行列を宣言して単位行列を代入する
	Matrix4 matRotX = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	// X軸回転行列に回転角を設定する
	matRotX.m[1][1] = cos(worldTransform_rotation_x);
	matRotX.m[1][2] = sin(worldTransform_rotation_x);
	matRotX.m[2][1] = -sin(worldTransform_rotation_x);
	matRotX.m[2][2] = cos(worldTransform_rotation_x);

	return matRotX;
}

Matrix4 Affin::RotateY(float worldTransform_rotation_y) {
	// Y軸回転行列を宣言して単位行列を代入する
	Matrix4 matRotY = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	// Y軸回転行列に回転角を設定する
	matRotY.m[0][0] = cos(worldTransform_rotation_y);
	matRotY.m[0][2] = -sin(worldTransform_rotation_y);
	matRotY.m[2][0] = sin(worldTransform_rotation_y);
	matRotY.m[2][2] = cos(worldTransform_rotation_y);

	return matRotY;
}

Matrix4 Affin::RotateZ(float worldTransform_rotate_z) {
	// Z軸回転行列を宣言して単位行列を代入する
	Matrix4 matRotZ = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	// Z軸回転行列に回転角を設定する
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
	//平行移動行列を宣言して単位行列を代入する
	Matrix4 matTrans = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	//平行移動を行列に設定する
	matTrans.m[3][0] = worldTransform_translation_.x;
	matTrans.m[3][1] = worldTransform_translation_.y;
	matTrans.m[3][2] = worldTransform_translation_.z;

	return matTrans;
}

Matrix4 Affin::MatWorld(Matrix4 matScale, Matrix4 matRot, Matrix4 matTrans) {

	WorldTransform worldTransform;
	worldTransform.matWorld_ = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

	//ワールドトランスフォームにスケーリング行列を代入する
	worldTransform.matWorld_ *= matScale *= matRot *= matTrans;

	return worldTransform.matWorld_;
}

Vector3 Affin::MatVector(Vector3 vector, WorldTransform worldTransform) {
	Vector3 vector_;
	vector_.x = vector.x * worldTransform.matWorld_.m[0][0] + vector.y * worldTransform.matWorld_.m[1][0] + vector.z * worldTransform.matWorld_.m[2][0] + 0 * worldTransform.matWorld_.m[3][0];
	vector_.y = vector.x * worldTransform.matWorld_.m[0][1] + vector.y * worldTransform.matWorld_.m[1][1] + vector.z * worldTransform.matWorld_.m[2][1] + 0 * worldTransform.matWorld_.m[3][1];
	vector_.z = vector.x * worldTransform.matWorld_.m[0][2] + vector.y * worldTransform.matWorld_.m[1][2] + vector.z * worldTransform.matWorld_.m[2][2] + 0 * worldTransform.matWorld_.m[3][2];
	return vector_;
}

Vector3 Affin::GetWorldPosition(WorldTransform worldTransform) {
	//ワールドに入れる座標
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform.matWorld_.m[3][0];
	worldPos.y = worldTransform.matWorld_.m[3][1];
	worldPos.z = worldTransform.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Affin::WDivision(Matrix4 mat, Vector3 vector)
{
	Vector4 division;
	division.x = mat.m[0][0] * vector.x + mat.m[1][0] * vector.y + mat.m[2][0] * vector.z + mat.m[3][0] * 1;
	division.y = mat.m[0][1] * vector.x + mat.m[1][1] * vector.y + mat.m[2][1] * vector.z + mat.m[3][1] * 1;
	division.z = mat.m[0][2] * vector.x + mat.m[1][2] * vector.y + mat.m[2][2] * vector.z + mat.m[3][2] * 1;
	division.w = mat.m[0][3] * vector.x + mat.m[1][3] * vector.y + mat.m[2][3] * vector.z + mat.m[3][3] * 1;

	division.x = division.x / division.w;
	division.y = division.y / division.w;
	division.z = division.z / division.w;

		return {division.x,division.y,division.z};
}