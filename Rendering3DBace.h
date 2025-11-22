//****************************************************************
//
// 3D描画基本システムのクラスの処理[3Drenderingbace.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// ３Dポリゴン描画システムクラスを定義
struct Render3DSystem : BaceRenderingSystem
{
	// コンストラクタ
	Render3DSystem() = default;
	// デストラクタ
	~Render3DSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};