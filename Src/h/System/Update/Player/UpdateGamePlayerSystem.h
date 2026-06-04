//================================================================
//
// プレイヤー更新システムの処理[UpdateGamePlayerSystem.h]
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

//****************************************************************
// ゲームプレイヤーのアップデート
//****************************************************************
struct UpdateGamePlayerSystem final : BaseSystem
{
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="reg">レジストリー</param>
	void Update(entt::registry& Reg)override;
private:
	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Player">プレイヤー</param>
	void UpdateMovement(entt::registry& Reg, entt::entity Player);
	/// <summary>
	/// ロックオンアニメーション
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Player">プレイヤー</param>
	void UpdateLockOn(entt::registry& Reg, entt::entity Player);
	/// <summary>
	/// 盗み
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Player">プレイヤー</param>
	void UpdateUnLock(entt::registry& Reg, entt::entity Player);
	/// <summary>
	/// ステートごとの更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Player">プレイヤー</param>
	void UpdateState(entt::registry& Reg, entt::entity Player);
	/// <summary>
	/// 敵が近づいた時のバイブレーション
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	/// <param name="Player">プレイヤー</param>
	void UpdateToEnemyVibration(entt::registry& Reg, entt::entity Player);
};