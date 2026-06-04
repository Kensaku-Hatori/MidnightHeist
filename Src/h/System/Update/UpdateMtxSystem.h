//================================================================
//
// マトリックスの計算処理[UpdateMtxsystem.h]
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
#include "BaseUpdatesystem.hpp"

//****************************************************************
// マトリックスの更新処理
//****************************************************************
struct UpdateMtxSystem final : BaseSystem
{
	UpdateMtxSystem();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	void Update(entt::registry& Reg)override;
private:
	/// <summary>
	/// トランスフォームコンポーネントが追加されたら
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Entity">エンティティ</param>
	void OnTransformComponentConstruct(entt::registry& Reg, entt::entity Entity);
};