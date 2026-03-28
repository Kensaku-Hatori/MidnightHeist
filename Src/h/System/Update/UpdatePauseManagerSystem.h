//****************************************************************
//
// ポーズマネージャーの更新処理[UpdatePauseSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// 敵のアップデート
struct UpdatePauseManagerSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdatePauseManagerSystem() : BaceSystem(false) {};
	~UpdatePauseManagerSystem() = default;
	// 更新
	void Update(entt::registry& reg)override;
	void UpdatePauseMenu(entt::registry& Reg, entt::entity& Manager);
	void FunctionMenu(entt::registry& Reg, entt::entity& Menu);
};