//================================================================
//
// プレイヤー描画システムの処理[RenderingPlayerSystem.hpp]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************
#include "System/Rendering/baseRenderingsystem.hpp"

//****************************************************************
// プレイヤーのアップデート
//****************************************************************
struct RenderingGamePlayerSystem final : BaseRenderingSystem
{
	// コンストラクタ・デストラクタ
	RenderingGamePlayerSystem() = default;
	~RenderingGamePlayerSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
	void RenderingShape(entt::registry& Reg, entt::entity Entity);
};