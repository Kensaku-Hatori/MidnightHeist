//****************************************************************
//
// タイトルマネージャーの更新処理[UpdateTitleSystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// 敵のアップデート
struct UpdateTitleManagerSystem : BaceSystem
{
	// コンストラクタ・デストラクタ
	UpdateTitleManagerSystem() = default;
	~UpdateTitleManagerSystem() = default;
	// 更新
	void Update(entt::registry& reg)override;
	void UpdateTitleMenu(entt::registry& Reg,entt::entity& Manager);
	void FunctionMenu(entt::registry& Reg, entt::entity& Menu);
};