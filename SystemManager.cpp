#include "SystemManager.h"
#include "manager.h"
#include "scene.h"

// Ã“Iƒƒ“ƒo•Ï”éŒ¾
std::vector<BaceSystem*> CSystemManager::m_UpdateSystems = {};
std::vector<BaceRenderingSystem*> CSystemManager::m_RenderingSystems = {};
std::vector<entt::entity> CSystemManager::m_DestroyList = {};
bool CSystemManager::m_IsPause = false;

void CSystemManager::UpdateAll(entt::registry& Reg)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_P) == true) {
		m_IsPause = !m_IsPause;
	}

	for (auto Systems = m_UpdateSystems.begin(); Systems != m_UpdateSystems.end(); Systems++)
	{
		if ((*Systems)->IsRefPause == true && m_IsPause == false)
		{
			(*Systems)->Update(Reg);
		}
		else if ((*Systems)->IsRefPause == false)
		{
			(*Systems)->Update(Reg);
		}
	}
}

void CSystemManager::RenderingAll(entt::registry& Reg)
{
	for (auto Systems = m_RenderingSystems.begin(); Systems != m_RenderingSystems.end(); Systems++)
	{
		if ((*Systems)->IsRefPause == true && m_IsPause == false)
		{
			(*Systems)->Rendering(Reg);
		}
		else if ((*Systems)->IsRefPause == false)
		{
			(*Systems)->Rendering(Reg);
		}
	}
	for (auto Destroys = m_DestroyList.begin(); Destroys != m_DestroyList.end();)
	{
		CManager::GetScene()->GetReg().destroy((*Destroys));
		Destroys = m_DestroyList.erase(Destroys);
		if (Destroys == m_DestroyList.end()) break;
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
	for (auto Systems = m_UpdateSystems.begin(); Systems != m_UpdateSystems.end();)
	{
		delete (*Systems);
		Systems = m_UpdateSystems.erase(Systems);
		if (Systems == m_UpdateSystems.end()) break;
	}
	for (auto Systems = m_RenderingSystems.begin(); Systems != m_RenderingSystems.end();)
	{
		delete (*Systems);
		Systems = m_RenderingSystems.erase(Systems);
		if (Systems == m_RenderingSystems.end()) break;
	}
}

void CSystemManager::AddDestroyList(entt::entity Entity)
{
	m_DestroyList.push_back(Entity);
}