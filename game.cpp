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
#include "distortion.h"
#include "GameCamera.h"
#include "Sound2D.h"

// 静的メンバ変数宣言
bool CGame::m_IsFinishedFirstNoise = false;
bool CGame::m_IsOlfFinishedFirstNoise = false;

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
	// フラグを初期化
	m_IsFinishedFirstNoise = false;
	// ステージ読み込み
	CMapManager::Instance()->Load("data\\TEXT\\StageInfo.json");
	// プレイヤー生成
	entt::entity Player = Factories::makeBacePlayer(GetReg(), D3DXVECTOR3(1000.0f, 500.0f, 375.0f));
	Factories::InitGamePlayer(GetReg(), Player);
	// 巡回ポイント読み込み
	MeshFactories::makePatrolPointFromFile(GetReg(), "data\\TEXT\\Patrol.json");
	// 敵管理エンティティを生成
	ManagerFactories::makeEnemyManager(GetReg());
	// ポーズマネージャー生成
	ManagerFactories::makePauseManager(GetReg());
	// アイテムマネージャー
	ManagerFactories::makeItemManager(GetReg());

	CSound2D::Instance()->Play(SoundDevice::LABEL_GAMEBGM);

	// テスト
	Factories::makeObject2D(GetReg(), 3, "data/TEXTURE/XDay.png", { 175.0f,125.0f }, { 100.0f,50.0f });
	Factories::makeObject2D(GetReg(), 3, "data/TEXTURE/images.png", { 1150.0f,125.0f }, { 50.0f,50.0f });
	Factories::makeObject2D(GetReg(), 3, "data/TEXTURE/CameraWork.png", { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f }, { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f });
	// ノイズスタート
	CDistortion::Instance()->StartNoise();

	// システム追加
	CManager::GetCamera()->AddSystem(new CGameCamera);

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
	// マップマネージャーの更新
	CMapManager::Instance()->Update();

	m_IsOlfFinishedFirstNoise = m_IsFinishedFirstNoise;
	if (m_IsFinishedFirstNoise == false && CDistortion::Instance()->IsNoised() == false) m_IsFinishedFirstNoise = true;

	if (rand() % 100 + 0 < 1 && CDistortion::Instance()->IsNoised() == false)
	{
		// ノイズスタート
		CDistortion::Instance()->StartNoise(0.1f, 0.1f, 15.0f);
	}
}

//***************************************
// 終了処理
//***************************************
void CGame::Uninit(void)
{
	// ノイズ終了
	CSound2D::Instance()->Stop(SoundDevice::LABEL_GAMEBGM);
	CDistortion::Instance()->EndNoise();
	CSystemManager::SetPause(false);
	CManager::GetCamera()->EndSystems();
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