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
	~CSystemManager() {}

	static void UpdateAll(entt::registry& Reg);
	static void RenderingAll(entt::registry& Reg);
	static void AddUpdateSystem(BaceSystem* System);
	static void AddRenderingSystem(BaceRenderingSystem* System);
	static void EndSystem(void);
private:
	CSystemManager() {}
	static std::vector<BaceSystem*> m_UpdateSystems;				// 更新処理の配列
	static std::vector<BaceRenderingSystem*> m_RenderingSystems;	// 描画処理の配列
};