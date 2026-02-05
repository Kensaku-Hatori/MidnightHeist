//****************************************************************
//
// 大きさのコンポーネント[SizeComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "math.h"

// テクスチャ情報
struct SizeComponent {
    // イニシャライズコンストラクタ
    SizeComponent(D3DXVECTOR2 _Size = { 1.0f,1.0f }) : Size(_Size) {};
    // ポリゴンの大きさ
    D3DXVECTOR2 Size;
};

// テクスチャ情報
struct Size3DComponent {
    // イニシャライズコンストラクタ
    Size3DComponent(D3DXVECTOR3 _Size = { 1.0f,1.0f,1.0f }) : Size(_Size) {};
    Size3DComponent(std::string ModelPath) : Size(CMath::CalcModelSize(ModelPath)) {};
    // ポリゴンの大きさ
    D3DXVECTOR3 Size;
};