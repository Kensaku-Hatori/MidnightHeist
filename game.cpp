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
#include "EnemyAIComponent.hpp"

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
	CMapManager::Instance()->Load("data\\TEXT\\StageInfo.json");
	Factories::makePlayer(GetReg());
	std::vector<EnemyState::PatrolMap> test;

	MeshFactories::makePatrolPointFromFile(GetReg(), "data\\TEXT\\Patrol.json");
	ManagerFactories::makeEnemyManager(GetReg());
	ManagerFactories::makePauseManager(GetReg());

	CManager::GetCamera()->SetRot(CCamera::Config::Game::Rot);
	CManager::GetCamera()->SetPosV(CCamera::Config::Game::PosV);
	CManager::GetCamera()->SetPosVDest(CCamera::Config::Game::PosV);
	CManager::GetCamera()->SetPosR(CCamera::Config::Game::PosR);
	CManager::GetCamera()->SetPosRDest(CCamera::Config::Game::PosR);

	return S_OK;
}

//***************************************
// 更新処理
//***************************************
void CGame::Update(void)
{
	CMapManager::Instance()->Update();
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
	CMapManager::Instance()->Draw();
}

//***************************************
// プレイヤーをリスポーンさせる
//***************************************
void CGame::ResetPlayer(void)
{
}