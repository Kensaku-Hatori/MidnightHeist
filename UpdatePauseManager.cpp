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
		// コンポーネントを取得
		auto& SelectMenuCmp = reg.get<Select2DComp>(entity);

		// ポーズ中じゃなかったら早期リターン
		if (CSystemManager::IsPause() == false) continue;

		// 今選んでいるメニューの更新
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true ||
			CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_UP) == true) {
			SelectMenuCmp.SelectMenu = static_cast<int>(Wrap(static_cast<MENUTYPE>(SelectMenuCmp.SelectMenu - 1), MENUTYPE::CONTINUE, MENUTYPE::QUIT));
		}
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true ||
			CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_DOWN) == true) {
			SelectMenuCmp.SelectMenu = static_cast<int>(Wrap(static_cast<MENUTYPE>(SelectMenuCmp.SelectMenu + 1), MENUTYPE::CONTINUE, MENUTYPE::QUIT));
		}
		// メニューの更新
		UpdatePauseMenu(reg, entity);
	}
}

//*********************************************
// メニューの更新
//*********************************************
void UpdatePauseManagerSystem::UpdatePauseMenu(entt::registry& Reg, entt::entity& Manager)
{
	// コンポーネントを取得
	auto& SelectMenuCmp = Reg.get<Select2DComp>(Manager);
	// ビューを生成
	auto view = Reg.view<PauseMenuComponent>();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネントを取得
		auto& myType = Reg.get<Menu2DComp>(entity);
		auto& ColorCmp = Reg.get<ColorComp>(entity);
		// 白色に設定
		ColorCmp.Col = WHITE;
		// 選ばれていなかったら切り上げる
		if (SelectMenuCmp.SelectMenu != myType.myType) continue;
		// 赤色に設定
		ColorCmp.Col = RED;
		// 決定ボタンを押したら
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
			CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
		{
			// メニューに応じた処理を実行
			FunctionMenu(Reg, entity);
			// 切り上げる
			break;
		}
	}
}

//*********************************************
// メニューの機能
//*********************************************
void UpdatePauseManagerSystem::FunctionMenu(entt::registry& Reg, entt::entity& Menu)
{
	// コンポーネントを取得
	auto& myType = Reg.get<Menu2DComp>(Menu);
	// メニューに応じた処理を実行
	if (MENUTYPE::CONTINUE == myType.myType)CSystemManager::SetPause(false);
	else if (MENUTYPE::RETRY == myType.myType)CFade::SetFade(new CGame);
	else if (MENUTYPE::QUIT == myType.myType)CFade::SetFade(new CTitle);
}