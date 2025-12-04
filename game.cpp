//************************************************************
//
//　ゲームの処理[game.cpp]
//　Author:Hatori Kensaku
//
//************************************************************

// インクルード
#include "game.h"
#include "manager.h"
#include "object2D.h"
#include "pausemanager.h"
#include "math.h"
#include "object3D.h"
#include "fade.h"
#include "mapmanager.h"
#include "Factories.h"

// 規定値を設定
// プレイヤー
const D3DXVECTOR3 CGame::Config::Player::Pos = { 120.0f,0.0f,-1700.0f };
const D3DXVECTOR3 CGame::Config::Player::Rot = { 0.0f,D3DX_PI,0.0f };
// カメラ
const D3DXVECTOR3 CGame::Config::Camera::Rot = { 0.0f,D3DX_PI,0.0f };
const D3DXVECTOR3 CGame::Config::Camera::PosR = { 120.0f,50.0f,-1700.0f };
const D3DXVECTOR3 CGame::Config::Camera::PosV = { 119.9f,60.0f,-1900.0f };
// 空
const D3DXVECTOR3 CGame::Config::Sky::Pos = VEC3_NULL;

// 静的メンバ変数
CPauseManager* CGame::m_pPauseManager = NULL;
CMapManager* CGame::m_pMapManager = NULL;
bool CGame::m_isPause = false;

// ネームスペース
using ordered_json = nlohmann::ordered_json;
using namespace std;

//***************************************
// コンストラクタ
//***************************************
CGame::CGame() : CScene(MODE_GAME)
{
}

//***************************************
// デストラクタ
//***************************************
CGame::~CGame()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CGame::Init(void)
{
	m_pMapManager = CMapManager::Instance();
	m_pMapManager->Load("data\\TEXT\\StageInfo.json");
	Factories::makePlayer(GetReg());
	Factories::makeEnemy(GetReg());
	MeshFactories::makePatrolPointFromFile(GetReg(), "data\\TEXT\\Patrol.json");
	return S_OK;
}

//***************************************
// 更新処理
//***************************************
void CGame::Update(void)
{
	if (m_pPauseManager != NULL)
	{
		m_pPauseManager->Update();
	}
	if (m_pMapManager != nullptr)
	{
		m_pMapManager->Update();
	}
}

//***************************************
// 終了処理
//***************************************
void CGame::Uninit(void)
{
	if (m_pPauseManager != NULL)
	{
		m_pPauseManager->Uninit();
		m_pPauseManager = NULL;
	}
	GetReg().clear();
	delete this;
}

//***************************************
// 描画処理
//***************************************
void CGame::Draw(void)
{
	if (m_pMapManager != nullptr)
	{
		m_pMapManager->Draw();
	}
}

//***************************************
// プレイヤーをリスポーンさせる
//***************************************
void CGame::ResetPlayer(void)
{
}