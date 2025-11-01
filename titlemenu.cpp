//****************************************************************
//
// タイトルメニューの処理[titlemenu.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "titlemenu.h"
#include "titlemanager.h"
#include "manager.h"
#include "math_T.h"
#include "fade.h"
#include "game.h"

// 規定値を設定
const D3DXCOLOR CTitleMenu::Config::DefoultColor = WHITE;
const D3DXCOLOR CTitleMenu::Config::SelectColor = BLUE;

//***************************************
// 基底クラスの処理
//***************************************
//***************************************
// コンストラクタ
//***************************************
CTitleMenu::CTitleMenu() : CObject2D(5)
{
}

//***************************************
// デストラクタ
//***************************************
CTitleMenu::~CTitleMenu()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CTitleMenu::Init(void)
{
	CObject2D::Init();
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CTitleMenu::Uninit(void)
{
	CObject2D::Uninit();
}

//***************************************
// 更新処理
//***************************************
void CTitleMenu::Update(void)
{
	CObject2D::Update();
}

//***************************************
// 描画処理
//***************************************
void CTitleMenu::Draw(void)
{
	CObject2D::Draw();
}

//***************************************
// 生成
//***************************************
CTitleMenu* CTitleMenu::Create(D3DXVECTOR3 Pos, Menu Menu)
{
	// 種類が最大数以上だったら
	if (Menu >= CTitleMenu::MAX) return NULL;

	// ポインタを生成
	CTitleMenu* pMenu = NULL;

	// 種類を判別
	switch (Menu)
	{
	case CTitleMenu::START:
		// メモリ確保
		pMenu = new CStart;
		break;
	case CTitleMenu::EXIT:
		// メモリ確保
		pMenu = new CExit;
		break;
	}

	// 各種処理
	pMenu->Init();
	pMenu->SetPosition(Pos);
	pMenu->SetSize({ CTitleManager::Config::Menu::Width,CTitleManager::Config::Menu::Height });
	// 返す
	return pMenu;
}


//***************************************
// 派生クラスの処理(Start)
//***************************************
//***************************************
// コンストラクタ
//***************************************
CStart::CStart()
{
}

//***************************************
// デストラクタ
//***************************************
CStart::~CStart()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CStart::Init(void)
{
	CTitleMenu::Init();
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CStart::Uninit(void)
{
	CTitleMenu::Uninit();
}

//***************************************
// 更新処理
//***************************************
void CStart::Update(void)
{
	// 初期の大きさ
	D3DXVECTOR2 Apper = { CTitleManager::Config::Menu::Width,CTitleManager::Config::Menu::Height };
	// 目標の大きさ
	D3DXVECTOR2 Dest = { CTitleManager::Config::Menu::DestWidth,CTitleManager::Config::Menu::DestHeight };

	// 今選んでいるのがスタートメニューだったら
	if (CTitleManager::GetSingleton()->GetSelectMenu() == START)
	{
		// アニメーションカウンターを進める
		if (m_nAnimCounter < CTitleManager::Config::Menu::Frame)
		{
			m_nAnimCounter++;
		}
		// 設定する大きさの変数
		D3DXVECTOR2 Size = {};
		// 今のアニメーションの進行割合を計算
		float Ratio = EaseInOutSine((float)m_nAnimCounter / CTitleManager::Config::Menu::Frame);
		// 最終的な大きさから初期の大きさからの差分
		D3DXVECTOR2 Diff = { Dest.x - Apper.x,Dest.y - Apper.y };
		// 今の大きさを計算
		Size = Apper + Diff * Ratio;

		CObject2D::SetSize(Size);
		CTitleMenu::SetCol(CTitleMenu::Config::SelectColor);

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputMouse()->OnDown(0) == true ||
			CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START) == true)
		{
			if (CTitleManager::GetLower() == false)
			{
				CTitleManager::GetSingleton()->GetTitleLogo()->Lower();
			}
			else
			{
				//CManager::GetSound()->Play(CSound::LABEL_ENTER);
				CFade::SetFade(new CGame);
				CManager::OffPause();
			}
		}
	}
	else
	{
		m_nAnimCounter = 0;
		CObject2D::SetSize(Apper);
		CTitleMenu::SetCol(CTitleMenu::Config::DefoultColor);
	}
	CTitleMenu::Update();
}

//***************************************
// 描画処理
//***************************************
void CStart::Draw(void)
{
	CTitleMenu::Draw();
}


//***************************************
// 派生クラスの処理(Exit)
//***************************************
//***************************************
// コンストラクタ
//***************************************
CExit::CExit()
{
}

//***************************************
// デストラクタ
//***************************************
CExit::~CExit()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CExit::Init(void)
{
	CTitleMenu::Init();
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CExit::Uninit(void)
{
	CTitleMenu::Uninit();
}

//***************************************
// 更新処理
//***************************************
void CExit::Update(void)
{
	// 初期の大きさ
	D3DXVECTOR2 Apper = { CTitleManager::Config::Menu::Width,CTitleManager::Config::Menu::Height };
	// 目標の大きさ
	D3DXVECTOR2 Dest = { CTitleManager::Config::Menu::DestWidth,CTitleManager::Config::Menu::DestHeight };

	// 今選んでいるのがExitメニューだったら
	if (CTitleManager::GetSingleton()->GetSelectMenu() == EXIT)
	{
		// アニメーションカウンターを進める
		if (m_nAnimCounter < CTitleManager::Config::Menu::Frame)
		{
			m_nAnimCounter++;
		}
		// 設定する大きさの変数
		D3DXVECTOR2 Size = {};
		// 今のアニメーションの進行割合を計算
		float Ratio = EaseInOutSine((float)m_nAnimCounter / CTitleManager::Config::Menu::Frame);
		// 最終的な大きさから初期の大きさからの差分
		D3DXVECTOR2 Diff = { Dest.x - Apper.x,Dest.y - Apper.y };
		// 今の大きさを計算
		Size = Apper + Diff * Ratio;

		CObject2D::SetSize(Size);
		CTitleMenu::SetCol(CTitleMenu::Config::SelectColor);

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputMouse()->OnDown(0) == true ||
			CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START) == true)
		{
			if (CTitleManager::GetLower() == false)
			{
				CTitleManager::GetSingleton()->GetTitleLogo()->Lower();
			}
			else
			{
				//CManager::GetSound()->Play(CSound::LABEL_ENTER);
				PostQuitMessage(0);
			}
		}
	}
	else
	{
		m_nAnimCounter = 0;
		CObject2D::SetSize(Apper);
		CTitleMenu::SetCol(CTitleMenu::Config::DefoultColor);
	}
	CTitleMenu::Update();
}

//***************************************
// 描画処理
//***************************************
void CExit::Draw(void)
{
	CTitleMenu::Draw();
}