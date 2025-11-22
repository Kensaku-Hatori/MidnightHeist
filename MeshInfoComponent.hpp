//****************************************************************
//
// インデックスバッファの数のコンポーネント[IndexNumComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インデックスバッファの数情報
struct MeshInfoComp {
    // インデックスバッファの数
    int nNumIdx;
    // 描画するポリゴン数
    int nNumPoly;
    // 頂点バッファの頂点数
    int nNumVtx;
    // コンストラクタ
    MeshInfoComp(const int DivH = 1, const int DivV = 1) : nNumVtx((DivH + 1)* (DivV + 1)), nNumIdx((2 * DivH * DivV + (DivV - 1) * 4) + 2), nNumPoly(nNumIdx - 2) {}
};