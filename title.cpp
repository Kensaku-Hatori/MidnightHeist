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
#include "Factories.h"
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

	Factories::makeObject2D(GetReg(), 3, "data/TEXTURE/Title/MidNightHeist.png", { 340.0f,160.0f }, { 200.0f,100.0f });
	Factories::makeObject3D(GetReg());
	Factories::makePlayer(GetReg());
	Factories::makeMapobject(GetReg(), "data\\MODEL\\Museum.x");
	ManagerFactories::makeTitleManager(GetReg());
	MeshFactories::makeMeshField(GetReg(), 100, 100, { 100.0f,100.0f });
	MeshFactories::makeSkyBox(GetReg());

	CManager::GetCamera()->SetRot(CCamera::Config::Title::Rot);
	CManager::GetCamera()->SetPosV(CCamera::Config::Title::PosV);
	CManager::GetCamera()->SetPosVDest(CCamera::Config::Title::PosV);
	CManager::GetCamera()->SetPosR(CCamera::Config::Title::PosR);
	CManager::GetCamera()->SetPosRDest(CCamera::Config::Title::PosR);

	return S_OK;
}

//***************************************
// 更新処理
//***************************************
void CTitle::Update(void)
{
	//static entt::entity test;
	if (CManager::GetInputKeyboard()->GetPress(DIK_SPACE) == true)
	{
		Factories::makeObject3D(GetReg());
	}
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_G) == true)
	//{
	//	test = Factories::makeObject2D(GetReg());
	//}
	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_H) == true)
	//{
	//	GetReg().destroy(test);
	//}
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