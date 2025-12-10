//************************************************************
//
//　ゲームの処理[game.cpp]
//　Author:Hatori Kensaku
//
//************************************************************

// インクルード
#include "game.h"
#include "manager.h"
#include "math.h"
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
	// ステージ読み込み
	CMapManager::Instance()->Load("data\\TEXT\\StageInfo.json");
	// プレイヤー生成
	Factories::makePlayer(GetReg());
	// 巡回ポイント読み込み
	MeshFactories::makePatrolPointFromFile(GetReg(), "data\\TEXT\\Patrol.json");
	// 敵管理エンティティを生成
	ManagerFactories::makeEnemyManager(GetReg());
	// ポーズマネージャー生成
	ManagerFactories::makePauseManager(GetReg());
	// テスト
	Factories::makeObject2D(GetReg(), 3, "data/TEXTURE/XDay.png", { 175.0f,125.0f }, { 100.0f,50.0f });
	Factories::makeObject2D(GetReg(), 3, "data/TEXTURE/images.png", { 1150.0f,125.0f }, { 50.0f,50.0f });
	Factories::makeObject2D(GetReg(), 3, "data/TEXTURE/CameraWork.png", { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f }, { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f });

	// カメラの初期化
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