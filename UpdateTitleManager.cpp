//****************************************************************
//
// タイトルマネージャーの更新処理[UpdateTitleSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateTitleManager.h"
#include "manager.h"
#include "TitleManagerComp.hpp"
#include "TitleMenuComp.hpp"
#include "TagComp.hpp"
#include "fade.h"
#include "game.h"
#include "ColorComponent.hpp"
#include "math_T.h"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateTitleManagerSystem::Update(entt::registry& reg)
{
	auto view = reg.view<TitleManagerComponent>();

	for (auto entity : view)
	{
		auto& SelectMenuCmp = reg.get<TitleSelectComp>(entity);
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true) {
			SelectMenuCmp.SelectMenu = Wrap(SelectMenuCmp.SelectMenu - 1, TitleMenu::MENUTYPE::START, TitleMenu::MENUTYPE::EXIT);
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true) {
			SelectMenuCmp.SelectMenu = Wrap(SelectMenuCmp.SelectMenu + 1, TitleMenu::MENUTYPE::START, TitleMenu::MENUTYPE::EXIT);
		}
		UpdateTitleMenu(reg, entity);
	}
}

//*********************************************
// メニューの更新
//*********************************************
void UpdateTitleManagerSystem::UpdateTitleMenu(entt::registry& Reg, entt::entity& Manager)
{
	auto& SelectMenuCmp = Reg.get<TitleSelectComp>(Manager);
	auto view = Reg.view<TitleMenuComponent>();

	for (auto entity : view)
	{
		auto& myType = Reg.get<TitleMenuComp>(entity);
		auto& ColorCmp = Reg.get<ColorComp>(entity);
		ColorCmp.Col = WHITE;
		if (SelectMenuCmp.SelectMenu != myType.myType) continue;
		ColorCmp.Col = RED;
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
	auto& myType = Reg.get<TitleMenuComp>(Menu);
	if (myType.myType == TitleMenu::MENUTYPE::START)CFade::SetFade(new CGame);
	else if (myType.myType == TitleMenu::MENUTYPE::EXIT)PostQuitMessage(0);
}