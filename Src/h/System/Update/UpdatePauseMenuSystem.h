//****************************************************************
//
// ポーズメニューの更新処理[UpdatePauseMenuSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// 敵のアップデート
struct UpdatePauseMenuSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdatePauseMenuSystem() : BaceSystem(false) {};
	~UpdatePauseMenuSystem() = default;
	// 更新
	void Update(entt::registry& reg)override;
};