//****************************************************************
//
// 選んでいるのコンポーネント[Select2DComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// テクスチャ情報
struct Select2DComponent {
    // イニシャライズテンプレートコンストラクタ
    template <typename T>
    Select2DComponent(T Default) : SelectMenu(static_cast<int>(Default)) {};
    // 選んでいるメニュー
    int SelectMenu;
};