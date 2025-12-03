//****************************************************************
//
// タイトルのコンポーネント[TitleSelectComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "TitleManagerComp.hpp"

// テクスチャ情報
struct TitleMenuComp {
    // イニシャライズコンストラクタ
    TitleMenuComp(TitleMenu::MENUTYPE _myType) : myType(_myType) {};
    // 自分のメニューの種類
    TitleMenu::MENUTYPE myType;
};