//****************************************************************
//
// 分割数のコンポーネント[DivisionComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インデックスバッファ情報
struct DivisionComp {
    // 水平方向の分割数
    int nDivHorizontal;
    // 垂直方向の分割数
    int nDivVertical;
    // コンストラクタ
    DivisionComp(const int DivH = 1, const int DivV = 1) : nDivHorizontal(DivH), nDivVertical(DivV) {}
};