//****************************************************************
//
// カプセルのコンポーネント[CapsuleComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 光線の情報
struct CapsuleComp {
	// コンストラクタ
	CapsuleComp(const float _AllHeight = NULL, const float _Radius = NULL,const float _ToCenterOffset = NULL) :
		AllHeight(_AllHeight), HalfHeight(_AllHeight * 0.5f), Radius(_Radius), ToCenterOffset(_ToCenterOffset) {};
	// カプセル情報
	float AllHeight, HalfHeight, Radius,ToCenterOffset;
};