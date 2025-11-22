//****************************************************************
//
// 色のコンポーネント[ColorComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// テクスチャ情報
struct ColorComp {
    // イニシャライズコンストラクタ
    ColorComp(D3DXCOLOR _Col = WHITE) : Col(_Col) {};
    // ポリゴンの色
    D3DXCOLOR Col;
};