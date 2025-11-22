//****************************************************************
//
// 法線のコンポーネント[NormalComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// テクスチャ情報
struct NorComp {
	// イニシャライズコンストラクタ
	NorComp(D3DXVECTOR3 _Nor = VEC3_NULL) : Nor(_Nor) {};
	// 法線
	D3DXVECTOR3 Nor;
};