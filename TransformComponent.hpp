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
	Transform3D(const D3DXVECTOR3 _Pos = VEC3_NULL, const D3DXVECTOR3 _Scale = { 1.0f,1.0f,1.0f }, const D3DXQUATERNION _Quat = QUAT_NULL)
		: Pos(_Pos), Quat(_Quat), Scale(_Scale) {
		D3DXMatrixIdentity(&mtxWorld);
	};
	// 位置
	D3DXVECTOR3 Pos;
	// 向き
	D3DXQUATERNION Quat;
	// 拡大率
	D3DXVECTOR3 Scale;
	// ワールドマトリックス
	D3DXMATRIX mtxWorld;
	// 回転行列
	D3DXMATRIX mtxBasis;
	// 位置行列
	D3DXMATRIX mtxTrans;
	// 拡大率行列
	D3DXMATRIX mtxScale;
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