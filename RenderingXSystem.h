//****************************************************************
//
// モデル描画基本システムのクラスの処理[Xrenderingbace.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceRenderingsystem.hpp"

// モデル描画クラスを定義
struct RenderXSystem : BaceRenderingSystem
{
	// コンストラクタ
	RenderXSystem() = default;
	// デストラクタ
	~RenderXSystem() = default;
	// 描画
	void Rendering(entt::registry& reg)override;
};