//****************************************************************
//
// カプセルのコンポーネント[CapsuleComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 光線の情報
struct CapsuleComponent {
	// コンストラクタ
	CapsuleComponent(const float _AllHeight = NULL, const float _Radius = NULL) :
		AllHeight(_AllHeight), HalfHeight(_AllHeight * 0.5f), Radius(_Radius), ToCenterOffset(HalfHeight + Radius) {};
	// カプセル情報
	float AllHeight, HalfHeight, Radius,ToCenterOffset;
};