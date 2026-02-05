//****************************************************************
//
// 描画するかどうかのコンポーネント[RenderFragComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// テクスチャ情報
struct RenderFragComponent {
    // イニシャライズコンストラクタ
    RenderFragComponent(const bool _IsRendering = true) : IsRendering(_IsRendering) {};
    // 自分のメニューの種類
    bool IsRendering;
};