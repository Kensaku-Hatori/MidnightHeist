//================================================================
//
// メッシュレーザーの更新処理[UpdateMeshLaser.h]
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

//****************************************************************
// メッシュレーザーの更新
//****************************************************************
struct UpdateMeshLaserSystem final : BaseSystem 
{
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	void Update(entt::registry& Reg)override;
private:
	/// <summary>
	/// 頂点バッファの更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Entity">レーザー</param>
	void UpdateVertex(entt::registry& Reg, entt::entity Entity);
	/// <summary>
	/// マップオブジェクトとの当たり判定
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Entity">レーザー</param>
	/// <param name="ToEntity">マップオブジェクト</param>
	/// <param name="Distance">距離(出力用)</param>
	/// <returns>結果(true = 当たった)</returns>
	bool CollisionEntity(entt::registry& Reg, entt::entity Entity, entt::entity ToEntity, float& Distance);
};