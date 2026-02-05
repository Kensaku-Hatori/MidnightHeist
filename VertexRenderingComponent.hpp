//****************************************************************
//
// 頂点バッファ情報のコンポーネント[VertexRenderingComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 頂点バッファ情報
struct VertexComponent {
	// 頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 pVertex = nullptr;
	VertexComponent() {
		pVertex = nullptr;
	}
	// コピー禁止
	VertexComponent& operator=(const VertexComponent&) = delete;
	// デストラクタ
	~VertexComponent() {
		// 頂点バッファの破棄
		if (pVertex) { pVertex->Release(); pVertex = nullptr; }
	}
};