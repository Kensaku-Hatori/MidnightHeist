//****************************************************************
//
// 移動方向情報のコンポーネント[Velocity.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 頂点バッファ情報
struct VelocityComponent {
	// 頂点バッファ
	D3DXVECTOR3 Velocity;
	VelocityComponent(D3DXVECTOR3 _Velocity = VEC3_NULL) : Velocity(_Velocity) {};
};