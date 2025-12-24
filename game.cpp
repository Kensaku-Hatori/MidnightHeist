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
int CGame::m_Time = 0;
int CGame::m_EnCount = 0;
int CGame::m_Steal = 0;

// ネームスペース
using ordered_json = nlohmann::ordered_json;
using namespace std;

// 規定値を設定
// プレイヤー
const D3DXVECTOR3 CGame::Config::Player::Pos = { 1000.0f, 50.0f, 375.0f };
// 2Dポリゴン
const D3DXVECTOR2 CGame::Config::XDay::Pos = { 175.0f,125.0f };
const D3DXVECTOR2 CGame::Config::XDay::Size = { 100.0f,50.0f };

const D3DXVECTOR2 CGame::Config::Rec::Pos = { 1150.0f,125.0f };
const D3DXVECTOR2 CGame::Config::Rec::Size = { 50.0f,50.0f };

const D3DXVECTOR2 CGame::Config::CameraWork::Pos = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f };
const D3DXVECTOR2 CGame::Config::CameraWork::Size = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f };

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
	// スタッツ情報を初期化
	m_Time = 0;
	m_EnCount = 0;
	m_Steal = 0;

	// フラグを失敗にする
	CManager::SetClear(false);
	// フラグを初期化
	m_IsFinishedFirstNoise = false;
	// ステージ読み込み
	CMapManager::Instance()->Load(Config::MapObjects::Path);
	// プレイヤー生成
	entt::entity Player = Factories::makeBacePlayer(GetReg(), Config::Player::Pos);
	Factories::InitGamePlayer(GetReg(), Player);
	// 巡回ポイント読み込み
	MeshFactories::makePatrolPointFromFile(GetReg(), Config::PatrolPoints::Path);
	// 敵管理エンティティを生成
	ManagerFactories::makeEnemyManager(GetReg());
	// ポーズマネージャー生成
	ManagerFactories::makePauseManager(GetReg());
	// アイテムマネージャー
	ManagerFactories::makeItemManager(GetReg());
	// ゲームBGMの再生
	CSound2D::Instance()->Play(SoundDevice::LABEL_GAMEBGM);

	// モニター用の2Dポリゴン生成
	Factories::makeObject2D(GetReg(), 3, Config::XDay::Path, Config::XDay::Pos, Config::XDay::Size);
	Factories::makeObject2D(GetReg(), 3, Config::Rec::Path, Config::Rec::Pos, Config::Rec::Size);
	Factories::makeObject2D(GetReg(), 3, Config::CameraWork::Path, Config::CameraWork::Pos, Config::CameraWork::Size);
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
	if (CManager::isPause() == false)
	{
		m_Frame++;
		if (m_Frame > 60)
		{
			m_Frame = 0;
			m_Time++;
		}
	}

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
	WriteStutsInfo();
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
// スタッツを書き込む
//***************************************
void CGame::WriteStutsInfo(void)
{
	ordered_json j;

	// OBJECT_SETの中身用の配列を準備
	ordered_json objectArray;

	// キーとバリューを設定
	objectArray["Time"] = m_Time;
	objectArray["EnCount"] = m_EnCount;
	objectArray["Steal"] = m_Steal;

	// "PlayData"タグの中にPlayDataの情報をだいにゅう
	j["PlayData"] = objectArray;

	ofstream writing_file("data/TEXT/Stuts.json");
	if (writing_file.is_open() == true)
	{
		writing_file << setw(4) << j << endl;
	}
	writing_file.close();
}

//***************************************
// プレイヤーをリスポーンさせる
//***************************************
void CGame::ResetPlayer(void)
{
}