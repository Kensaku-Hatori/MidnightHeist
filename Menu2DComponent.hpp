//****************************************************************
//
// メニューのコンポーネント[Menu2DComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// テクスチャ情報
struct Menu2DComponent {
    // イニシャライズコンストラクタ
    Menu2DComponent(int _myType) : myType(_myType) {};
    // 自分のメニューの種類
    const int myType;
};