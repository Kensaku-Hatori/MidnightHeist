//****************************************************************
//
// スタッツアニメーション更新システムの処理[UpdateStutsAnimSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// プレイヤーのアップデート
struct UpdateStutsAnimSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateStutsAnimSystem() = default;
	~UpdateStutsAnimSystem() = default;
	// 描画
	void Update(entt::registry& reg)override;
};