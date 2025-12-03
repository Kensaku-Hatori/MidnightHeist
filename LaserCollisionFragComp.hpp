//****************************************************************
//
// レーザーの当たり判定状況のコンポーネント[LaserCollisionFragComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// テクスチャ情報
struct LaserCollisionFragComp {
    // イニシャライズコンストラクタ
    LaserCollisionFragComp() : IsRayCollision(false), IsLaserCollision(false) {};
    // コリジョンフラグ
    bool IsRayCollision;
    bool IsLaserCollision;
};