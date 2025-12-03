//****************************************************************
//
// プレイヤー描画システムの処理[RenderingPlayerSystem.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"
#include "TransformComponent.hpp"
#include "RigitBodyComponent.hpp"

// プレイヤーのアップデート
struct PlayerRenderingSystem : BaceRenderingSystem
{
	// コンストラクタ・デストラクタ
	PlayerRenderingSystem() = default;
	~PlayerRenderingSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
	void RenderingOutLine(entt::registry& Reg, entt::entity Entity);
	void RenderingShape(entt::registry& Reg, entt::entity Entity);
};