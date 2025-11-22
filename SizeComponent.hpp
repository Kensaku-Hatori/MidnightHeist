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
struct SizeComp {
    // イニシャライズコンストラクタ
    SizeComp(D3DXVECTOR2 _Size = { 1.0f,1.0f }) : Size(_Size) {};
    // ポリゴンの大きさ
    D3DXVECTOR2 Size;
};

// テクスチャ情報
struct Size3DComp {
    // イニシャライズコンストラクタ
    Size3DComp(D3DXVECTOR3 _Size = { 1.0f,1.0f,1.0f }) : Size(_Size) {};
    Size3DComp(std::string ModelPath) : Size(CMath::CalcModelSize(ModelPath)) {};
    // ポリゴンの大きさ
    D3DXVECTOR3 Size;
};