//****************************************************************
//
// 更新システムの基底クラスの処理[baceUpdatesystem.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 基底クラス
struct BaceSystem
{
	// コンストラクタ
	BaceSystem() = default;
	// デストラクタ
	virtual ~BaceSystem() = default;
	// 更新
	virtual void Update(entt::registry& reg) = 0;
};