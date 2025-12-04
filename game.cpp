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
#include "SystemManager.h"

// 静的メンバ変数
CMapManager* CGame::m_pMapManager = NULL;

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
	Factories::makePauseManager(GetReg());
	return S_OK;
}

//***************************************
// 更新処理
//***************************************
void CGame::Update(void)
{
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
	CSystemManager::SetPause(false);
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