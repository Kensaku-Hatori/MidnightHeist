//****************************************************************
//
// 円形UIのコンポーネント[UICircleComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// テクスチャ情報
struct UICircleComponent {
    // コンストラクタ
    UICircleComponent(const float _Radius = NULL, const float _MaxFillAngle = D3DX_PI * 2.0f, const float _FillAmount = 0.0f) :
        FillAmount(_FillAmount), Radius(_Radius), MaxFillAngle(_MaxFillAngle) {};
    // 情報
    float FillAmount;   // 塗りつぶす量
    float Radius;       // 半径
    float MaxFillAngle; // 塗りつぶす最大量(角度)
};