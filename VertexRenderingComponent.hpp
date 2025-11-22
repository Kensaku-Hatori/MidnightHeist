//****************************************************************
//
// 頂点バッファ情報のコンポーネント[VertexRenderingComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 頂点バッファ情報
struct VertexComp {
	// 頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 pVertex = nullptr;
	VertexComp() {
		pVertex = nullptr;
	}
	// デストラクタ
	~VertexComp() {
		// 頂点バッファの破棄
		if (pVertex) { pVertex->Release(); pVertex = nullptr; }
	}
};