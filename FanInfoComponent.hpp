//****************************************************************
//
// 扇のコンポーネント[FanComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 光線の情報
struct FanComp {
	// コンスタント
	FanComp(const D3DXVECTOR3 _Origin = VEC3_NULL, const D3DXVECTOR3 _Dir = VEC_UP, const float _RangeDegree = 90.0f, const float _Length = 100.0f) :
		Origin(_Origin), Dir(_Dir), RangeDegree(_RangeDegree), Length(_Length) {}
	// 扇形情報
	D3DXVECTOR3 Origin, Dir;
	float RangeDegree, Length;
};