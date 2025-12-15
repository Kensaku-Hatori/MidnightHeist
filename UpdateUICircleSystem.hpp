//****************************************************************
//
// 円形UIの更新処理[UpdateUICircleSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// 敵のアップデート
struct UpdateUICircleSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateUICircleSystem() = default;
	~UpdateUICircleSystem() = default;
	// 更新
	void Update(entt::registry& reg)override;
};