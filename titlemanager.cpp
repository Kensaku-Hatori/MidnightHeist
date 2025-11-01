//****************************************************************
//
// タイトルマネジャーの処理[titlemanager.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "titlemanager.h"
#include "manager.h"
#include "math_T.h"
#include "sound.h"

// 規定値を設定
const D3DXVECTOR3 CTitleManager::Config::Menu::Bace = { 250.0f,400.0f,0.0f };
const D3DXVECTOR3 CTitleManager::Config::Logo::Apper = { 250.0f,0.0f,0.0f };
const D3DXVECTOR3 CTitleManager::Config::Logo::Dest = { 250.0f,200.0f,0.0f };
const D3DXVECTOR2 CTitleManager::Config::Logo::Size = { 200.0f,200.0f };

// シングルトンを宣言
CTitleManager* CTitleManager::m_Singleton = NULL;

//***************************************
// デストラクタ
//***************************************
CTitleManager::~CTitleManager()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CTitleManager::Init(void)
{
	// 選んでいつメニュー
	m_SelectMenu = CTitleMenu::START;
	// メニューを一気に生成
	for (int nCount = 0; nCount < CTitleMenu::MAX; nCount++)
	{
		CTitleMenu* pTitleMenu = CTitleMenu::Create(D3DXVECTOR3(CTitleManager::Config::Menu::Bace.x, CTitleManager::Config::Menu::Bace.y + CTitleManager::Config::Menu::OffSet * nCount, 0.0f), (CTitleMenu::Menu)nCount);
		m_apTitleMenu.push_back(pTitleMenu);
	}
	//w ロゴを生成
	m_TitleLogo = CTitleLogo::Create(CTitleManager::Config::Logo::Apper, VEC3_NULL, CTitleManager::Config::Logo::Size, CTitleManager::Config::Logo::FilePath);
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CTitleManager::Uninit(void)
{
	// シングルトンの終了処理
	if (m_Singleton != NULL)
	{
		m_apTitleMenu.clear();
		m_Singleton = NULL;
	}
	// 自分自身の破棄
	Release();
}

//***************************************
// 更新処理
//***************************************
void CTitleManager::Update(void)
{
	// メニューを選ぶ
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true || CManager::GetInputMouse()->GetMouseState().lZ < 0 ||
		CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_UP) == true)
	{
		//CManager::GetSound()->Play(CSound::LABEL_MOVESELECT);
		m_SelectMenu = (CTitleMenu::Menu)Wrap(m_SelectMenu - 1, 0, (int)CTitleMenu::MAX - 1);
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true || CManager::GetInputMouse()->GetMouseState().lZ > 0 ||
		CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_DOWN) == true)
	{
		//CManager::GetSound()->Play(CSound::LABEL_MOVESELECT);
		m_SelectMenu = (CTitleMenu::Menu)Wrap(m_SelectMenu + 1, 0, (int)CTitleMenu::MAX - 1);
	}
}

//***************************************
// シングルトンを生成
//***************************************
CTitleManager* CTitleManager::CreateSingleton(void)
{
	// シングルトンが生成されていたら
	if (m_Singleton != NULL) return m_Singleton;
	// メモリ確保
	m_Singleton = new CTitleManager;
	// 初期化処理
	m_Singleton->Init();
	// 返す
	return m_Singleton;
}