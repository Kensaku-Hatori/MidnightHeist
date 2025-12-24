//****************************************************************
//
// タイトルマネージャーの更新処理[UpdateTitleSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdatePauseManager.h"
#include "manager.h"
#include "Select2DComponent.hpp"
#include "Menu2DComponent.hpp"
#include "PauseUI.hpp"
#include "TagComp.hpp"
#include "fade.h"
#include "game.h"
#include "ColorComponent.hpp"
#include "SystemManager.h"
#include "title.h"
#include "math_T.h"

// 名前空間
using namespace Tag;
using namespace PauseMenu;

//*********************************************
// 更新
//*********************************************
void UpdatePauseManagerSystem::Update(entt::registry& reg)
{
	// ビューを生成
	auto view = reg.view<PauseManagerComponent>();

	// アクセス
	for (auto entity : view)
	{
		// 今選んでいるメニューの更新
		auto& SelectMenuCmp = reg.get<Select2DComp>(entity);

		// ポーズ中じゃなかったら早期リターン
		if (CSystemManager::IsPause() == false) continue;

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true) {
			SelectMenuCmp.SelectMenu = static_cast<int>(Wrap(static_cast<MENUTYPE>(SelectMenuCmp.SelectMenu - 1), MENUTYPE::CONTINUE, MENUTYPE::QUIT));
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true) {
			SelectMenuCmp.SelectMenu = static_cast<int>(Wrap(static_cast<MENUTYPE>(SelectMenuCmp.SelectMenu + 1), MENUTYPE::CONTINUE, MENUTYPE::QUIT));
		}
		UpdatePauseMenu(reg, entity);
	}
}

//*********************************************
// メニューの更新
//*********************************************
void UpdatePauseManagerSystem::UpdatePauseMenu(entt::registry& Reg, entt::entity& Manager)
{
	auto& SelectMenuCmp = Reg.get<Select2DComp>(Manager);
	auto view = Reg.view<PauseMenuComponent>();

	for (auto entity : view)
	{
		auto& myType = Reg.get<Menu2DComp>(entity);
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
void UpdatePauseManagerSystem::FunctionMenu(entt::registry& Reg, entt::entity& Menu)
{
	auto& myType = Reg.get<Menu2DComp>(Menu);
	if (MENUTYPE::CONTINUE == myType.myType)CSystemManager::SetPause(false);
	else if (MENUTYPE::RETRY == myType.myType)CFade::SetFade(new CGame);
	else if (MENUTYPE::QUIT == myType.myType)CFade::SetFade(new CTitle);
}