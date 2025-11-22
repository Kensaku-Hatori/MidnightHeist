//****************************************************************
//
// 3D更新システムの処理[3DUpdatesystem.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "baceUpdatesystem.hpp"

// 3D更新処理
struct Update3DSystem : BaceSystem
{
	// コンストラクタ
	Update3DSystem() = default;
	// デストラクタ
	~Update3DSystem() = default;
	// 更新処理
	void Update(entt::registry& reg)override;
};