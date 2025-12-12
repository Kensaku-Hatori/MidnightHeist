//****************************************************************
//
// 変身のコンポーネント[ObjectXComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 3D変身コンポーネント情報
struct Transform3D {
	// イニシャライズコンストラクタ
	Transform3D(const float _QuatSpeed = 1.0f, const D3DXVECTOR3 _Pos = VEC3_NULL, const D3DXVECTOR3 _Scale = { 1.0f,1.0f,1.0f }, const D3DXQUATERNION _Quat = QUAT_NULL)
		: Pos(_Pos), Quat(_Quat), Scale(_Scale), QuatDest(_Quat), QuatSpeed(_QuatSpeed) {};
	// 位置
	D3DXVECTOR3 Pos;
	// 向き
	D3DXQUATERNION Quat;
	// 目標のクォータニオン
	D3DXQUATERNION QuatDest;
	// クォータニオンの補完スピード
	float QuatSpeed;
	// 拡大率
	D3DXVECTOR3 Scale;

	// ワールドマトリックスを取得
	D3DXMATRIX GetWorldMatrix(void) {
		// 拡大率のマトリックス、回転行列、平行移動行列を計算
		D3DXMATRIX mtxScale, mtxRotation, mtxTransform, mtxWorld;
		D3DXQuaternionSlerp(&Quat, &Quat, &QuatDest, QuatSpeed);
		D3DXMatrixIdentity(&mtxWorld);
		D3DXMatrixScaling(&mtxScale, Scale.x, Scale.y, Scale.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);
		D3DXMatrixRotationQuaternion(&mtxRotation, &Quat);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRotation);
		D3DXMatrixTranslation(&mtxTransform, Pos.x, Pos.y, Pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTransform);

		// かけ合わせたものを返す
		return mtxWorld;
	}

	// ワールドマトリックスを取得
	D3DXMATRIX GetMultiplyWorldMatrix(D3DXMATRIX Parent) const {
		// 拡大率のマトリックス、回転行列、平行移動行列を計算
		D3DXMATRIX mtxScale, mtxRotation, mtxTransform, mtxWorld;
		D3DXMatrixIdentity(&mtxWorld);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &Parent);
		D3DXMatrixScaling(&mtxScale, Scale.x, Scale.y, Scale.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);
		D3DXMatrixRotationQuaternion(&mtxRotation, &Quat);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRotation);
		D3DXMatrixTranslation(&mtxTransform, Pos.x, Pos.y, Pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTransform);

		// かけ合わせたものを返す
		return mtxWorld;
	}

	// ワールドマトリックスを取得
	D3DXMATRIX GetWorldMatrixInv(void) const {
		// 拡大率のマトリックス、回転行列、平行移動行列を計算
		D3DXMATRIX mtxScale, mtxRotation, mtxTransform, mtxWorld,Out;
		D3DXMatrixIdentity(&mtxWorld);
		D3DXMatrixScaling(&mtxScale, Scale.x, Scale.y, Scale.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);
		D3DXMatrixRotationQuaternion(&mtxRotation, &Quat);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRotation);
		D3DXMatrixTranslation(&mtxTransform, Pos.x, Pos.y, Pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTransform);
		D3DXMatrixInverse(&Out, NULL, &mtxWorld);

		// かけ合わせたものを返す
		return Out;
	}

	// 回転行列を取得
	D3DXMATRIX GetBasis(void) const {
		// 出力用行列
		D3DXMATRIX mtxRotation;
		// 回転行列を作成
		D3DXMatrixRotationQuaternion(&mtxRotation, &Quat);
		// 返す
		return mtxRotation;
	}
};

// 2D変身コンポーネント情報
struct Transform2D {
	// イニシャライズコンストラクタ
	Transform2D(D3DXVECTOR2 _Pos = VEC2_NULL, float _Angle = 0.0f, D3DXVECTOR2 _Scale = {1.0f,1.0f})
		: Pos(_Pos), Angle(_Angle), Scale(_Scale) {};
	// 位置
	D3DXVECTOR2 Pos = { 0.0f,0.0f };
	// 向き
	float Angle = 0.0f;
	// 拡大率
	D3DXVECTOR2 Scale = { 1.0f,1.0f };
};