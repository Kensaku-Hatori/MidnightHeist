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
#include "TitleCamera.h"

// 規定値を設定

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

	Factories::makeObject2D(GetReg(), 3, "data/TEXTURE/Title/MidNightHeist.png", { 340.0f,160.0f }, { 200.0f,100.0f });
	Factories::makeObject3D(GetReg());
	Factories::makePlayer(GetReg());
	Factories::makeMapobject(GetReg(), "data\\MODEL\\Museum.x");
	ManagerFactories::makeTitleManager(GetReg());
	MeshFactories::makeMeshField(GetReg(), 100, 100, { 100.0f,100.0f });
	MeshFactories::makeSkyBox(GetReg());

	CManager::GetCamera()->AddSystem(new CTitleCamera);
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
}

//***************************************
// 終了処理
//***************************************
void CTitle::Uninit(void)
{
	CManager::GetCamera()->EndSystems();
	GetReg().clear();
	delete this;
}

//***************************************
// 描画処理
//***************************************
void CTitle::Draw(void)
{
}