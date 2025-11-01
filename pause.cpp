//****************************************************************
//
// ポーズの処理[pause.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "pause.h"
#include "manager.h"
#include "pausemanager.h"
#include "math_T.h"
#include "scene.h"
#include "manager.h"
#include "fade.h"
#include "game.h"
#include "title.h"

// 規定値を設定
const D3DXCOLOR CPause::Config::DefoultColor = { 1.0f,1.0f,1.0f,0.5f };
const D3DXCOLOR CPause::Config::SelectColor = { 1.0f,1.0f,1.0f,1.0f };

//***************************************
// ポーズメニューの基底クラス
//***************************************
//***************************************
// コンストラクタ
//***************************************
CPause::CPause() : CObject2D(7)
{
}

//***************************************
// デストラクタ
//***************************************
CPause::~CPause()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CPause::Init(void)
{
	CObject2D::Init();
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CPause::Uninit(void)
{
	CObject2D::Uninit();
}

//***************************************
// 更新処理
//***************************************
void CPause::Update(void)
{
	CObject2D::Update();
}

//***************************************
// 描画処理
//***************************************
void CPause::Draw(void)
{
	if (CManager::isPause() == false) return;
	CObject2D::Draw();
}

//***************************************
// 生成
//***************************************
CPause* CPause::Create(D3DXVECTOR3 Pos, Menu Menu)
{
	if (Menu >= CPause::MAX) return NULL;
	CPause* pPause = NULL;
	switch (Menu)
	{
	case CONTINUE:
		pPause = new CContinue;
		break;
	case RETRY:
		pPause = new CRetry;
		break;
	case QUIT:
		pPause = new CQuit;
		break;
	}
	pPause->Init();
	pPause->SetPosition(Pos);
	pPause->SetSize({ CPauseManager::Config::Width,CPauseManager::Config::Height });
	return pPause;
}


//***************************************
// ポーズメニューの派生クラス
//***************************************
//***************************************
// コンストラクタ
//***************************************
CContinue::CContinue()
{
}

//***************************************
// デストラクタ
//***************************************
CContinue::~CContinue()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CContinue::Init(void)
{
	CPause::Init();
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CContinue::Uninit(void)
{
	CPause::Uninit();
}

//***************************************
// 更新処理
//***************************************
void CContinue::Update(void)
{
	// 初期の大きさ
	D3DXVECTOR2 Apper = { CPauseManager::Config::Width,CPauseManager::Config::Height };
	// 目標の大きさ
	D3DXVECTOR2 Dest = { CPauseManager::Config::DestWidth,CPauseManager::Config::DestHeight };

	if (CManager::isPause() == false) return;
	if (CPauseManager::GetSingleton()->GetSelectMenu() == CONTINUE)
	{
		// アニメーションカウンターを進める
		m_nAnimCounter++;

		// 設定する大きさの変数
		D3DXVECTOR2 Size = {};
		// 今のアニメーションの進行割合を計算
		float Ratio = EaseInOutSine((float)m_nAnimCounter / CPauseManager::Config::Frame);
		// 最終的な大きさから初期の大きさからの差分
		D3DXVECTOR2 Diff = { Dest.x - Apper.x,Dest.y - Apper.y };
		// 今の大きさを計算
		Size = Apper + Diff * Ratio;

		CObject2D::SetSize(Size);
		CPause::SetCol(CPause::Config::SelectColor);
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
		{
			//CManager::GetSound()->Play(CSound::LABEL_ENTER);
			CManager::OffPause();
		}
	}
	else
	{
		m_nAnimCounter = 0;
		CObject2D::SetSize(Apper);
		CPause::SetCol(CPause::Config::DefoultColor);
	}
	CPause::Update();
}

//***************************************
// 描画処理
//***************************************
void CContinue::Draw(void)
{
	CPause::Draw();
}


//***************************************
// ポーズメニューの派生クラス
//***************************************
//***************************************
// コンストラクタ
//***************************************
CRetry::CRetry()
{
}

//***************************************
// デストラクタ
//***************************************
CRetry::~CRetry()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CRetry::Init(void)
{
	CPause::Init();
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CRetry::Uninit(void)
{
	CPause::Uninit();
}

//***************************************
// 更新処理
//***************************************
void CRetry::Update(void)
{
	// 初期の大きさ
	D3DXVECTOR2 Apper = { CPauseManager::Config::Width,CPauseManager::Config::Height };
	// 目標の大きさ
	D3DXVECTOR2 Dest = { CPauseManager::Config::DestWidth,CPauseManager::Config::DestHeight };

	if (CManager::isPause() == false) return;
	if (CPauseManager::GetSingleton()->GetSelectMenu() == RETRY)
	{
		// アニメーションカウンターを進める
		m_nAnimCounter++;

		// 設定する大きさの変数
		D3DXVECTOR2 Size = {};
		// 今のアニメーションの進行割合を計算
		float Ratio = EaseInOutSine((float)m_nAnimCounter / CPauseManager::Config::Frame);
		// 最終的な大きさから初期の大きさからの差分
		D3DXVECTOR2 Diff = { Dest.x - Apper.x,Dest.y - Apper.y };
		// 今の大きさを計算
		Size = Apper + Diff * Ratio;

		CObject2D::SetSize(Size);
		CPause::SetCol(CPause::Config::SelectColor);
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
		{
			if (CManager::GetScene()->GetMode() == CScene::MODE::MODE_GAME)
			{
				//CManager::GetSound()->Play(CSound::LABEL_ENTER);
				CFade::SetFade(new CGame);
			}
			CManager::OffPause();
		}
	}
	else
	{
		m_nAnimCounter = 0;
		CObject2D::SetSize(Apper);
		CPause::SetCol(CPause::Config::DefoultColor);
	}
	CPause::Update();
}

//***************************************
// 描画処理
//***************************************
void CRetry::Draw(void)
{
	CPause::Draw();
}


//***************************************
// ポーズメニューの派生クラス
//***************************************
//***************************************
// コンストラクタ
//***************************************
CQuit::CQuit()
{
}

//***************************************
// デストラクタ
//***************************************
CQuit::~CQuit()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CQuit::Init(void)
{
	CPause::Init();
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CQuit::Uninit(void)
{
	CPause::Uninit();
}

//***************************************
// 更新処理
//***************************************
void CQuit::Update(void)
{
	// 初期の大きさ
	D3DXVECTOR2 Apper = { CPauseManager::Config::Width,CPauseManager::Config::Height };
	// 目標の大きさ
	D3DXVECTOR2 Dest = { CPauseManager::Config::DestWidth,CPauseManager::Config::DestHeight };

	if (CManager::isPause() == false) return;
	if (CPauseManager::GetSingleton()->GetSelectMenu() == QUIT)
	{
		// アニメーションカウンターを進める
		m_nAnimCounter++;

		// 設定する大きさの変数
		D3DXVECTOR2 Size = {};
		// 今のアニメーションの進行割合を計算
		float Ratio = EaseInOutSine((float)m_nAnimCounter / CPauseManager::Config::Frame);
		// 最終的な大きさから初期の大きさからの差分
		D3DXVECTOR2 Diff = { Dest.x - Apper.x,Dest.y - Apper.y };
		// 今の大きさを計算
		Size = Apper + Diff * Ratio;

		CObject2D::SetSize(Size);
		CPause::SetCol(CPause::Config::SelectColor);
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
		{
			//CManager::GetSound()->Play(CSound::LABEL_ENTER);
			CFade::SetFade(new CTitle);
			CManager::OffPause();
		}
	}
	else
	{
		m_nAnimCounter = 0;
		CObject2D::SetSize(Apper);
		CPause::SetCol(CPause::Config::DefoultColor);
	}
	CPause::Update();
}

//***************************************
// 描画処理
//***************************************
void CQuit::Draw(void)
{
	CPause::Draw();
}