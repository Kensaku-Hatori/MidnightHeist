//****************************************************************
//
// アウトラインのコンポーネント[OutLineComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 光線の情報
struct OutLineComp {
	// コンスタント
	OutLineComp(const float _Thickness = 6.0f, const D3DXVECTOR4 _Color = { 1.0f, 0.3f, 0.5f, 1.0f }, const float _Height = NULL) :
		Height(_Height), Thickness(_Thickness), Color(_Color) {};
	// 情報
	float Height;		// 高さ
	float Thickness;	// 太さ(係数)
	D3DXVECTOR4 Color;	// 色
};