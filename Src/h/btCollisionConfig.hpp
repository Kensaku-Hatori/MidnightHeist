//****************************************************************
//
// 当たり判定のコンフィグ[btCollisionConfig.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 当たり判定のグループ
namespace CollisionGroupAndMasks {
	constexpr int GROUP_ENEMY = 1 << 0;		// 敵グループ
	constexpr int GROUP_PLAYER = 2 << 1;	// プレイヤーグループ
	constexpr int GROUP_MAPOBJECT = 3 << 2;	// マップグループ
	constexpr int MASK_ENEMY = GROUP_PLAYER | GROUP_MAPOBJECT;						// 敵のマスク
	constexpr int MASK_PLAYER = GROUP_ENEMY | GROUP_PLAYER | GROUP_MAPOBJECT;		// プレイヤーのマスク
	constexpr int MASK_MAPOBJECT = GROUP_ENEMY | GROUP_PLAYER | GROUP_MAPOBJECT;	// 設置物のマスク
}