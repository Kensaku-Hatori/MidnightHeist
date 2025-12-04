//****************************************************************
//
// レーザーの当たり判定状況のコンポーネント[LaserCollisionFragComp.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// コリジョン情報
namespace LasetCollisionInfo {
    // 最接近オブジェクトへの距離のエラー値
    constexpr float InvalidNearDistance = -1;
}
// テクスチャ情報
struct LaserCollisionInfoComp {
    // イニシャライズコンストラクタ
    LaserCollisionInfoComp() : IsRayCollision(false), IsLaserCollision(false), NearObjectDistance(LasetCollisionInfo::InvalidNearDistance) {};
    // コリジョンフラグ
    bool IsRayCollision;
    bool IsLaserCollision;
    // 最接近オブジェクトへの距離
    float NearObjectDistance;
};