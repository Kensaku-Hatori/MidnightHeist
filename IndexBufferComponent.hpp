//****************************************************************
//
// インデックスバッファのコンポーネント[IndexBufferComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インデックスバッファ情報
struct IndexBufferComponent {
    // インデックスバッファ
    LPDIRECT3DINDEXBUFFER9 pIdx;
    // コンストラクタ
    IndexBufferComponent() {
        pIdx = nullptr;
    }
    // デストラクタ
    ~IndexBufferComponent() {
        if (pIdx != nullptr) { pIdx->Release(); pIdx = nullptr; };
    }
};