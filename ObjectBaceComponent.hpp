//****************************************************************
//
// トランスフォームのコンポーネント[ObjectBaceComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "TransformComponent.hpp"

// オブジェクトの基本情報
struct ObjectBase {
    // 描画の優先度
    int Priority = 3;
    // ID
    int nID = 0;
    // 死亡フラグ
    bool Death = false;
};