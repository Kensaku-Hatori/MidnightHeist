//****************************************************************
//
// システムを管理[SystemManager.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

#include "baceUpdatesystem.hpp"
#include "baceRenderingsystem.hpp"

class CSystemManager
{
public:
	~CSystemManager() {};

	static void UpdateAll(entt::registry& Reg);
	static void RenderingAll(entt::registry& Reg);
	static void AddUpdateSystem(BaceSystem* System);
	static void AddRenderingSystem(BaceRenderingSystem* System);
	static void SetPause(bool Set) { m_IsPause = Set; };
	static bool IsPause(void) { return m_IsPause; }
	static void EndSystem(void);
	// セッター
	static void AddDestroyList(entt::entity Entity);
	// ゲッター
	static size_t GetUpdateSystemSize(void) { return m_UpdateSystems.size(); }
	static size_t GetRenderingSystemSize(void) { return m_RenderingSystems.size(); }
private:
	CSystemManager() {}
	static std::vector<BaceSystem*> m_UpdateSystems;				// 更新処理の配列
	static std::vector<BaceRenderingSystem*> m_RenderingSystems;	// 描画処理の配列
	static std::vector<entt::entity> m_DestroyList;					// 削除予定のエンティティ
	static bool m_IsPause;											// ポーズ中かどうか
};