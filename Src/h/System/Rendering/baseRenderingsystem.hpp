//================================================================
//
// 描画システムの基底クラスの処理[baceRenderingsystem.hpp]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// 描画システムの基底クラス
//****************************************************************
struct BaseRenderingSystem
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="RefPause">ポーズを参照するかどうか</param>
	BaseRenderingSystem(const bool RefPause = false) { IsRefPause = RefPause; };

	virtual ~BaseRenderingSystem() = default;
	virtual void Rendering(entt::registry& reg) = 0;
	// ポーズを参照するかどうか
	bool IsRefPause;
};