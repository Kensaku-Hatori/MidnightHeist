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
#include "Factories.h"
#include "titlemanager.h"
#include "fade.h"
#include "game.h"

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

	//m_pTitleManager = CTitleManager::CreateSingleton();

	Factories::makeObject3D(GetReg());
	Factories::makePlayer(GetReg());
	Factories::makeMapobject(GetReg(), "data\\MODEL\\rack.x", VEC3_NULL, D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f));
	MeshFactories::makeMeshField(GetReg(), 100, 100, { 10.0f,10.0f });

	return S_OK;
}

//***************************************
// 更新処理
//***************************************
void CTitle::Update(void)
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_SPACE) == true)
	{
		Factories::makeObject3D(GetReg());
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_H) == true)
	{
		Factories::makeObject2D(GetReg(), 2, {});
	}
	if (CManager::GetInputKeyboard()->GetPress(DIK_B) == true)
	{
		Factories::makeObject2D(GetReg(), 1, "data\\TEXTURE\\grass003.png");
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{
		CFade::SetFade(new CGame);
	}
}

//***************************************
// 終了処理
//***************************************
void CTitle::Uninit(void)
{
	GetReg().clear();
	delete this;
}

//***************************************
// 描画処理
//***************************************
void CTitle::Draw(void)
{
}