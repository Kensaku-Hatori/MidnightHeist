//************************************************************
//
//　タイトルの処理[title.cpp]
//　Author:Hatori Kensaku
//
//************************************************************

// インクルード
#include "title.h"
#include "manager.h"
#include "modelmanager.h"
#include "object2D.h"
#include "titlelogo.h"
#include "titlemanager.h"

// 規定値を設定
// カメラ
const D3DXVECTOR3 CTitle::Config::Camera::PosV = { -231.75f,79.0f,-1025.75f };
const D3DXVECTOR3 CTitle::Config::Camera::PosR = { -430.0f,75.0f,-1000.0f };
const D3DXVECTOR3 CTitle::Config::Camera::Rot = { -0.03f,1.7f,0.0f };

// プレイヤー
const D3DXVECTOR3 CTitle::Config::Player::Pos = { -340.0f,0.0f,-1045.0f };

// 静的メンバ変数
CTitleManager* CTitle::m_pTitleManager = NULL;

//***************************************
// コンストラクタ
//***************************************
CTitle::CTitle() : CScene(MODE_TITLE)
{
}

//***************************************
// デストラクタ
//***************************************
CTitle::~CTitle()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CTitle::Init(void)
{
	// 平行光源の色をリセット
	CManager::GetLight()->ResetCol();
		
	m_pTitleManager = CTitleManager::CreateSingleton();

	return S_OK;
}

//***************************************
// 更新処理
//***************************************
void CTitle::Update(void)
{
}

//***************************************
// 終了処理
//***************************************
void CTitle::Uninit(void)
{
	delete this;
}

//***************************************
// 描画処理
//***************************************
void CTitle::Draw(void)
{
}