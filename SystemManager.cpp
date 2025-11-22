#include "SystemManager.h"

// Ã“Iƒƒ“ƒo•Ï”éŒ¾
std::vector<BaceSystem*> CSystemManager::m_UpdateSystems = {};
std::vector<BaceRenderingSystem*> CSystemManager::m_RenderingSystems = {};

void CSystemManager::UpdateAll(entt::registry& Reg)
{
	for (auto Systems = m_UpdateSystems.begin(); Systems != m_UpdateSystems.end(); Systems++)
	{
		(*Systems)->Update(Reg);
	}
}

void CSystemManager::RenderingAll(entt::registry& Reg)
{
	for (auto Systems = m_RenderingSystems.begin(); Systems != m_RenderingSystems.end(); Systems++)
	{
		(*Systems)->Rendering(Reg);
	}
}

void CSystemManager::AddUpdateSystem(BaceSystem* System)
{
	if (System == nullptr) return;
	m_UpdateSystems.push_back(System);
}

void CSystemManager::AddRenderingSystem(BaceRenderingSystem* System)
{
	if (System == nullptr) return;
	m_RenderingSystems.push_back(System);
}

void CSystemManager::EndSystem(void)
{
	for (auto Systems = m_UpdateSystems.begin(); Systems != m_UpdateSystems.end(); Systems)
	{
		delete (*Systems);
		Systems = m_UpdateSystems.erase(Systems);
		if (Systems == m_UpdateSystems.end()) break;
	}
	for (auto Systems = m_RenderingSystems.begin(); Systems != m_RenderingSystems.end(); Systems++)
	{
		delete (*Systems);
		Systems = m_RenderingSystems.erase(Systems);
		if (Systems == m_RenderingSystems.end()) break;
	}
}