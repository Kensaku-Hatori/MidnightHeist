//****************************************************************
//
// 光線のコンポーネント[RayComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 光線の情報
struct RayComponent {
	// コンスタント
	RayComponent(const D3DXVECTOR3 _Origin = VEC3_NULL, const D3DXVECTOR3 _Dir = VEC_UP) : Origin(_Origin), Dir(_Dir) {}
	// 光線情報
	D3DXVECTOR3 Origin, Dir;
};