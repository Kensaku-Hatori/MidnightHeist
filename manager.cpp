//*************************************
//
//　Manager[manager.cpp]
//　Author:Hatori Kensaku
//
//*************************************

// インクルード
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "texmanager.h"
#include "modelmanager.h"
#include "default.h"
#include "toon.h"
#include "shadowmap.h"
#include "shapeshadow.h"
#include "scene.h"
#include "fade.h"
#include "title.h"
#include "SystemManager.h"
#include "defaultcubemap.h"
#include "distortion.h"
#include "UICircle.h"
#include "Sound2D.h"

#include "UpdateGamePlayerSystem.h"
#include "UpdateTitlePlayerSystem.h"
#include "Update2DSystem.h"
#include "Update3DSystem.h"
#include "UpdateXSystem.h"
#include "UpdateMapobjectSystem.h"
#include "UpdateMeshFieldSystem.h"
#include "UpdateMeshLaserSystem.h"
#include "UpdateEnemySystem.h"
#include "UpdateEnemyChase.h"
#include "UpdateEnemyPredict.h"
#include "UpdateTitleManager.h"
#include "UpdatePauseManager.h"
#include "UpdatePauseMenu.h"
#include "UpdateEnemySearch.h"
#include "UpdateItemSystem.h"
#include "UpdateUICircleSystem.hpp"
#include "UpdateEnemyBack.h"

#include "Rendering2Dbace.h"
#include "Rendering3DBace.h"
#include "RenderingXSystem.h"
#include "RenderingGamePlayerSystem.h"
#include "RenderingTitlePlayerSystem.h"
#include "RenderingMeshFieldSystem.h"
#include "RenderingMapobjectSystem.h"
#include "RenderingMeshLaserSystem.h"
#include "RenderingEnemySystem.h"
#include "RenderingToShadowmap.h"
#include "RenderingToShapeShadow.h"
#include "RenderingOutLineSystem.h"
#include "RenderingPauseMenu.h"
#include "RenderingSkyBoxSystem.h"
#include "RenderingUICircle.h"
#include "RenderingEnemySightSystem.h"

// 名前空間
using namespace std;

// 静的メンバ変数宣言
CRenderer* CManager::m_Renderer = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CInputJoypad* CManager::m_pInputJoypad = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CCamera* CManager::m_pCamera = NULL;
CLight* CManager::m_pLight = NULL;
CPlayer* CManager::m_pPlayer = NULL;
CScene* CManager::m_pScene = NULL;
CFade* CManager::m_pFade = NULL;
unique_ptr<btDiscreteDynamicsWorld> CManager::m_pDynamicsWorld = NULL;
bool CManager::m_isPause = false;
bool CManager::m_isClear = false;

//***************************************
// コンストラクタ
//***************************************
CManager::CManager()
{
}

//***************************************
// デストラクタ
//***************************************
CManager::~CManager()
{

}

//***************************************
// 初期化処理
//***************************************
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWnd)
{
	HRESULT hr;

	// 静的メンバ変数の一部をメモリ確保
	m_Renderer = new CRenderer;
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputJoypad = new CInputJoypad;
	m_pInputMouse = new CInputMouse;
	m_pCamera = new CCamera;
	m_pLight = new CLight;

	CSystemManager::AddUpdateSystem(new UpdateEnemyChaseSystem);
	CSystemManager::AddUpdateSystem(new UpdateEnemyPredictSystem);
	CSystemManager::AddUpdateSystem(new UpdateEnemySearchSystem);
	CSystemManager::AddUpdateSystem(new UpdateEnemyBackSystem);
	CSystemManager::AddUpdateSystem(new UpdateUICircleSystem);
	CSystemManager::AddUpdateSystem(new Update2DSystem);
	CSystemManager::AddUpdateSystem(new Update3DSystem);
	CSystemManager::AddUpdateSystem(new UpdateTitlePlayerSystem);
	CSystemManager::AddUpdateSystem(new UpdateGamePlayerSystem);
	CSystemManager::AddUpdateSystem(new UpdateMapobjectSystem);
	CSystemManager::AddUpdateSystem(new UpdateMeshLaserSystem);
	CSystemManager::AddUpdateSystem(new UpdateEnemySystem);
	CSystemManager::AddUpdateSystem(new UpdateTitleManagerSystem);
	CSystemManager::AddUpdateSystem(new UpdatePauseManagerSystem);
	CSystemManager::AddUpdateSystem(new UpdatePauseMenuSystem);
	CSystemManager::AddUpdateSystem(new UpdateItemSystem);
	//CSystemManager::AddUpdateSystem(new UpdateMeshFieldSystem);
	//CBaceSystem::AddSystem(new CXUpdateSystem);

	CSystemManager::AddRenderingSystem(new RenderingSkyBoxSystem);
	CSystemManager::AddRenderingSystem(new RenderingToShadowmapSystem);
	CSystemManager::AddRenderingSystem(new RenderingMapobjectSystem);
	CSystemManager::AddRenderingSystem(new RenderingToShapeShadowSystem);
	CSystemManager::AddRenderingSystem(new RenderingOutLineSystem);
	CSystemManager::AddRenderingSystem(new Render3DSystem);
	CSystemManager::AddRenderingSystem(new RenderXSystem);
	CSystemManager::AddRenderingSystem(new RenderingEnemySystem);
	CSystemManager::AddRenderingSystem(new RenderingTitlePlayerSystem);
	CSystemManager::AddRenderingSystem(new RenderingGameSystem);
	CSystemManager::AddRenderingSystem(new RenderMehFieldSystem);
	CSystemManager::AddRenderingSystem(new RenderMehLaerSystem);
	CSystemManager::AddRenderingSystem(new RenderingEnemySightSystem);
	CSystemManager::AddRenderingSystem(new Render2DSystem);
	CSystemManager::AddRenderingSystem(new RenderingUICircleSystem);
	CSystemManager::AddRenderingSystem(new RenderingPauseMenuSystem);

	// 物理世界に必要なポインタを生成
	m_pBroadPhase = make_unique<btDbvtBroadphase>();
	m_pConfiguration = make_unique<btDefaultCollisionConfiguration>();
	m_pDispatcher = make_unique<btCollisionDispatcher>(m_pConfiguration.get());
	m_pSolver = make_unique<btSequentialImpulseConstraintSolver>();

	// 物理世界生成
	m_pDynamicsWorld = make_unique<btDiscreteDynamicsWorld>(m_pDispatcher.get(), m_pBroadPhase.get(), m_pSolver.get(), m_pConfiguration.get());

	// 重力を設定
	m_pDynamicsWorld->setGravity({ 0.0f,-9.81f,0.0f });

	// メモリ確保できたら
	if (m_Renderer != NULL)
	{
		// 初期化処理
		hr = m_Renderer->Init(hWnd, bWnd);
	}

	// メモリ確保できたら
	if (m_pInputKeyboard != NULL)
	{
		// 初期化処理
		hr = m_pInputKeyboard->Init(hInstance, hWnd);
	}
	// メモリ確保できたら
	if (m_pInputJoypad != NULL)
	{
		// 初期化処理
		hr = m_pInputJoypad->Init(hInstance, hWnd);
	}
	// メモリ確保できたら
	if (m_pInputMouse != NULL)
	{
		// 初期化処理
		hr = m_pInputMouse->Init(hInstance, hWnd);
	}

	// ３Dに必要なものを初期化
	m_pCamera->Init();
	m_pLight->Init();

	CSound2D::Instance()->Init();

	// 暗転係を生成
	m_pFade = CFade::CreateSingle();

	// タイトル画面に設定
	SetScene(new CTitle);

	CShapeShadow::Instance()->Init();
	CShadowMap::Instance()->Init();
	CToon::Instance()->Init();
	CDefaultCubemap::Instance()->Init();
	CDistortion::Instance()->Init();
	CDistortion::Instance()->Init();
	CUICircle::Instance()->Init();

	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CManager::Uninit()
{
	// シーンの終了処理
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		m_pScene = nullptr;
	}

	// カメラが使われていたら
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// レンダラークラスが使われていたら
	if (m_Renderer != NULL)
	{
		m_Renderer->Uninit();
		delete m_Renderer;
		m_Renderer = NULL;
	}

	// キーボードが使われていたら
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
	// ゲームパッドが使われていたら
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}
	// Mouseが使われていたら
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// ライトが使われていたら
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// フェードの破棄
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		m_pFade = NULL;
	}

	CShadowMap::Instance()->ReSet();
	CShapeShadow::Instance()->ReSet();
	CLoadTexture::UnRegistTex();
	CModelManager::UnRegistModel();
	CSound2D::Instance()->Uninit();
}

//***************************************
// 更新処理
//***************************************
void CManager::Update()
{
	// カメラのアップデート
	m_pCamera->Update();

	// 物理世界でシュミレーションを実行
	m_pDynamicsWorld->stepSimulation(btScalar(GetFPS()), 10, GetDeltaTime());

	// ライトのアップデート
	m_pLight->Update();

	if (m_isPause == false)
	{
		if (m_Renderer != NULL)
		{
			m_Renderer->Update();
		}
	}

	// シーンの更新
	if (m_pScene != NULL)
	{
		m_pScene->Update();
	}

	// フェードの更新
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

	// キーボードが使われていたら更新
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}
	// ゲームパッドが使われていたら更新
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();
	}
	// Mouseが使われていたら更新
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Update();
	}
}

//***************************************
// 描画処理
//***************************************
void CManager::Draw()
{
	if (m_Renderer != NULL)
	{
		m_Renderer->Draw();
	}
}

//***************************************
// シーンを設定
//***************************************
void CManager::SetScene(CScene* Scene)
{
	if (m_pScene == Scene)
	{
		delete Scene;
		return;
	}
	else if (m_pScene != NULL)
	{
		m_pScene->Uninit();
		m_pScene = NULL;
		CObject::ReleaseAll();
	}
	m_pScene = Scene;
	m_pScene->Init();
}

//***************************************
// プレイヤーのリスポーン
//***************************************
void CManager::RespawPlayer(void)
{
}