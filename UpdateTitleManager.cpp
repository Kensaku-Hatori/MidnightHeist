//****************************************************************
//
// タイトルマネージャーの更新処理[UpdateTitleSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateTitleManager.h"
#include "manager.h"
#include "Select2DComponent.hpp"
#include "Menu2DComponent.hpp"
#include "TitleUI.hpp"
#include "TagComp.hpp"
#include "fade.h"
#include "game.h"
#include "ColorComponent.hpp"
#include "math_T.h"

// 名前空間
using namespace Tag;
using namespace TitleMenu;

//*********************************************
// 更新
//*********************************************
void UpdateTitleManagerSystem::Update(entt::registry& reg)
{
	auto view = reg.view<TitleManagerComponent>();

	for (auto entity : view)
	{
		auto& SelectMenuCmp = reg.get<Select2DComp>(entity);
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true) {
			SelectMenuCmp.SelectMenu = static_cast<int>(Wrap(static_cast<MENUTYPE>(SelectMenuCmp.SelectMenu - 1), MENUTYPE::START, MENUTYPE::EXIT));
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true) {
			SelectMenuCmp.SelectMenu = static_cast<int>(Wrap(static_cast<MENUTYPE>(SelectMenuCmp.SelectMenu + 1), MENUTYPE::START, MENUTYPE::EXIT));
		}
		UpdateTitleMenu(reg, entity);
	}
}

//*********************************************
// メニューの更新
//*********************************************
void UpdateTitleManagerSystem::UpdateTitleMenu(entt::registry& Reg, entt::entity& Manager)
{
	auto& SelectMenuCmp = Reg.get<Select2DComp>(Manager);
	auto view = Reg.view<TitleMenuComponent>();

	for (auto entity : view)
	{
		auto& myType = Reg.get<Menu2DComp>(entity);
		auto& ColorCmp = Reg.get<ColorComp>(entity);
		ColorCmp.Col = WHITE;
		if (SelectMenuCmp.SelectMenu != myType.myType) continue;
		ColorCmp.Col = BLUE;
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
		{
			FunctionMenu(Reg, entity);
			break;
		}
	}
}

//*********************************************
// メニューの機能
//*********************************************
void UpdateTitleManagerSystem::FunctionMenu(entt::registry& Reg, entt::entity& Menu)
{
	auto& myType = Reg.get<Menu2DComp>(Menu);
	if (MENUTYPE::START == myType.myType)CFade::SetFade(new CGame);
	else if (MENUTYPE::EXIT == myType.myType)PostQuitMessage(0);
}