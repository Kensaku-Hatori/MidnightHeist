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
	// ビューを生成
	auto view = reg.view<TitleManagerComponent>();

	// アクセス
	for (auto entity : view)
	{
		// 選択しているメニューを更新
		auto& SelectMenuCmp = reg.get<Select2DComp>(entity);
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true) {
			SelectMenuCmp.SelectMenu = static_cast<int>(Wrap(static_cast<MENUTYPE>(SelectMenuCmp.SelectMenu - 1), MENUTYPE::START, MENUTYPE::EXIT));
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true) {
			SelectMenuCmp.SelectMenu = static_cast<int>(Wrap(static_cast<MENUTYPE>(SelectMenuCmp.SelectMenu + 1), MENUTYPE::START, MENUTYPE::EXIT));
		}
		// メニューを更新
		UpdateTitleMenu(reg, entity);
	}
}

//*********************************************
// メニューの更新
//*********************************************
void UpdateTitleManagerSystem::UpdateTitleMenu(entt::registry& Reg, entt::entity& Manager)
{
	// コンポーネントを取得
	auto& SelectMenuCmp = Reg.get<Select2DComp>(Manager);
	// ビューを生成
	auto view = Reg.view<TitleMenuComponent>();
	// アクセス
	for (auto entity : view)
	{
		// コンポーネントを取得
		auto& myType = Reg.get<Menu2DComp>(entity);
		auto& ColorCmp = Reg.get<ColorComp>(entity);
		// 白にする
		ColorCmp.Col = WHITE;
		// 選ばれていなかったら早期リターン
		if (SelectMenuCmp.SelectMenu != myType.myType) continue;
		// 青にする
		ColorCmp.Col = BLUE;
		// エンターキーが押されたら
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
		{
			// メニューに応じた処理を実行
			FunctionMenu(Reg, entity);
			// 処理を切り上げる
			break;
		}
	}
}

//*********************************************
// メニューの機能
//*********************************************
void UpdateTitleManagerSystem::FunctionMenu(entt::registry& Reg, entt::entity& Menu)
{
	// コンポーネントを取得
	auto& myType = Reg.get<Menu2DComp>(Menu);
	// タイプに応じた処理を実行
	if (MENUTYPE::START == myType.myType)CFade::SetFade(new CGame);
	else if (MENUTYPE::EXIT == myType.myType)PostQuitMessage(0);
}