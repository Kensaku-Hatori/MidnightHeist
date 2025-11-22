//****************************************************************
//
// インデックスバッファのコンポーネント[IndexBufferComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インデックスバッファ情報
struct IndexBufferComp {
    // インデックスバッファ
    LPDIRECT3DINDEXBUFFER9 pIdx;
    // コンストラクタ
    IndexBufferComp() {
        pIdx = nullptr;
    }
    // デストラクタ
    ~IndexBufferComp() {
        if (pIdx != nullptr) { pIdx->Release(); pIdx = nullptr; };
    }
};