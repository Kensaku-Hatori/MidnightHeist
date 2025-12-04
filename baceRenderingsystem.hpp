//****************************************************************
//
// 描画システムの基底クラスの処理[baceRenderingsystem.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 描画システムの基底クラス
struct BaceRenderingSystem
{
	// コンストラクタ
	BaceRenderingSystem(const bool RefPause = false) { IsRefPause = RefPause; };
	// デストラクタ
	virtual ~BaceRenderingSystem() = default;
	// 純粋仮想関数
	virtual void Rendering(entt::registry& reg) = 0;
	// ポーズを参照するかどうか
	bool IsRefPause;
};