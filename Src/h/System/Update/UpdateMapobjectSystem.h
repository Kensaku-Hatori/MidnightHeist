//================================================================
//
// マップオブジェクト更新システムの処理[UpdateMapobjectSystem.h]
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
#include "System/Update/BaseUpdatesystem.hpp"
#include "Component/TransformComponent.hpp"
#include "Component/SizeComponent.hpp"

//****************************************************************
// マップオブジェクトの更新処理
//****************************************************************
struct UpdateMapobjectSystem final : BaseSystem
{
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	void Update(entt::registry& Reg)override;
private:
	/// <summary>
	/// 脱出判定
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="MapObject">マップオブジェクト</param>
	void CollisionExitGate(entt::registry& Reg, entt::entity MapObject);
};