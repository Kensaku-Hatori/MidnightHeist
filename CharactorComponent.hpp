//****************************************************************
//
// ベーシックなキャラクタのコンポーネント[CharactorComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// キャラクターコンポーネントを宣言
struct CharactorComponent {
	// イニシャライズコンストラクタ
	CharactorComponent(const float _QuatSpeed = 1.0f, const float _Speed = 5.0f, const D3DXQUATERNION _Dest = QUAT_NULL) :
		QuatSpeed(_QuatSpeed), Speed(_Speed), QuatDest(_Dest) {}
	// 目標の向き
	D3DXQUATERNION QuatDest;
	// 補完スピード
	float QuatSpeed;
	// 歩く速さ
	float Speed;
};