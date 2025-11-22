//****************************************************************
//
// 描画順のコンポーネント[LayerComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 描画順コンポーネントを宣言
struct LayerComp {
	// イニシャライズコンストラクタ
	LayerComp(const int Layer = 1) : nLayer(Layer) {}
	// 描画順
	int nLayer;
};