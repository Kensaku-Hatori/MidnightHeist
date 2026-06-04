//================================================================
//
//　Manager[manager.cpp]
//　Author:Hatori Kensaku
//
//================================================================

//****************************************************************
// インクルード
//****************************************************************
#include "Bace/manager.h"
#include "Bace/renderer.h"
#include "Manager/texmanager.h"
#include "Manager/modelmanager.h"
#include "Shader/toon.h"
#include "Shader/shadowmap.h"
#include "Shader/shapeshadow.h"
#include "Scene/scene.h"
#include "Bace/fade.h"
#include "Scene/title.h"
#include "Manager/SystemManager.h"
#include "Shader/defaultcubemap.h"
#include "Shader/distortion.h"
#include "Shader/UICircle.h"
#include "Bace/Sound2D.h"
#include "Shader/VisibleSineCurve.h"
#include "Shader/SimpleBloom.h"

//****************************************************************
// 更新システム
//****************************************************************
#include "System/Update/Player/UpdateGamePlayerSystem.h"
#include "System/Update/Player/UpdateTitlePlayerSystem.h"
#include "System/Update/Obj/Update2DSystem.h"
#include "System/Update/Obj/Update3DSystem.h"
#include "System/Update/Obj/UpdateXSystem.h"
#include "System/Update/UpdateMapobjectSystem.h"
#include "System/Update/Mesh/UpdateMeshFieldSystem.h"
#include "System/Update/Mesh/UpdateMeshLaserSystem.h"
#include "System/Update/Enemy/UpdateEnemySystem.h"
#include "System/Update/Enemy/UpdateEnemyChaseSystem.h"
#include "System/Update/Enemy/UpdateEnemyPredictSystem.h"
#include "System/Update/UpdateTitleManagerSystem.h"
#include "System/Update/UpdatePauseManagerSystem.h"
#include "System/Update/UpdatePauseMenuSystem.h"
#include "System/Update/Enemy/UpdateEnemySearchSystem.h"
#include "System/Update/UpdateItemSystem.h"
#include "System/Update/Shader/UpdateUICircleSystem.hpp"
#include "System/Update/Enemy/UpdateEnemyBackSystem.h"
#include "System/Update/Shader/UpdateVisibleSineCurveSystem.h"
#include "System/Update/UpdateMtxSystem.h"
#include "System/Update/UpdateTimerSystem.h"
#include "System/Update/UpdateStutsAnimSystem.h"
#include "System/Update/UpdateGateManagerSystem.h"
#include "System/Update/UpdateRigidBodySystem.h"
#include "System/Update/Enemy/UpdateEnemySightSystem.h"

//****************************************************************
// 描画システム
//****************************************************************
#include "System/Rendering/Obj/Rendering2DBaseSystem.h"
#include "System/Rendering/Obj/Rendering3DBaseSystem.h"
#include "System/Rendering/Obj/RenderingXSystem.h"
#include "System/Rendering/Player/RenderingGamePlayerSystem.h"
#include "System/Rendering/Player/RenderingTitlePlayerSystem.h"
#include "System/Rendering/Mesh/RenderingMeshFieldSystem.h"
#include "System/Rendering/Mesh/RenderingMeshLaserSystem.h"
#include "System/Rendering/RenderingMapobjectSystem.h"
#include "System/Rendering/Enemy/RenderingEnemySystem.h"
#include "System/Rendering/Shader/RenderingToShadowmapSystem.h"
#include "System/Rendering/Shader/RenderingToShapeShadowSystem.h"
#include "System/Rendering/Shader/RenderingOutLineSystem.h"
#include "System/Rendering/RenderingPauseMenuSystem.h"
#include "System/Rendering/Shader/RenderingSkyBoxSystem.h"
#include "System/Rendering/Shader/RenderingUICircleSystem.h"
#include "System/Rendering/Enemy/RenderingEnemySightSystem.h"
#include "System/Rendering/Shader/RenderingVisibleSineCurveSystem.h"

//****************************************************************
// 名前空間
//****************************************************************
using namespace std;

//****************************************************************
// 静的メンバ変数宣言
//****************************************************************
CRenderer* CManager::m_Renderer = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CInputJoypad* CManager::m_pInputJoypad = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CCamera* CManager::m_pCamera = NULL;
CLight* CManager::m_pLight = NULL;
CScene* CManager::m_pScene = NULL;
unique_ptr <CThreadPool> CManager::m_pThreadPool = NULL;
unique_ptr<btDiscreteDynamicsWorld> CManager::m_pDynamicsWorld = NULL;
bool CManager::m_isPause = false;
bool CManager::m_isClear = false;

//****************************************************************
// コンストラクタ
//****************************************************************
CManager::CManager()
{
}

//****************************************************************
// デストラクタ
//****************************************************************
CManager::~CManager()
{

}

//****************************************************************
// 初期化処理
//****************************************************************
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

	// 物理世界に必要なポインタを生成
	m_pBroadPhase = make_unique<btDbvtBroadphase>();
	m_pConfiguration = make_unique<btDefaultCollisionConfiguration>();
	m_pDispatcher = make_unique<btCollisionDispatcher>(m_pConfiguration.get());
	m_pSolver = make_unique<btSequentialImpulseConstraintSolver>();

	// スレッドプール作成
	m_pThreadPool = make_unique<CThreadPool>();

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

	CFade::Instance().Init();

	// 2Dサウンドを初期化
	CSound2D::Instance().Init();

	// タイトル画面に設定
	SetScene(new CTitle);

	// シェーダー初期化
	CShapeShadow::Instance().Init();
	CShadowMap::Instance().Init();
	CToon::Instance().Init();
	CDefaultCubemap::Instance().Init();
	CDistortion::Instance().Init();
	CUICircle::Instance().Init();
	CVisibleSineCurve::Instance().Init();
	CSimpleBloom::Instance().Init();

	return S_OK;
}

//****************************************************************
// システム初期化処理
//****************************************************************
void CManager::InitSystems(void)
{
	// 更新システムを追加
	CSystemManager::AddUpdateSystem(new UpdateMtxSystem);
	CSystemManager::AddUpdateSystem(new UpdateRigidBodySystem);
	CSystemManager::AddUpdateSystem(new UpdateEnemyChaseSystem);
	CSystemManager::AddUpdateSystem(new UpdateEnemyPredictSystem);
	CSystemManager::AddUpdateSystem(new UpdateEnemySearchSystem);
	CSystemManager::AddUpdateSystem(new UpdateEnemyBackSystem);
	CSystemManager::AddUpdateSystem(new UpdateUICircleSystem);
	CSystemManager::AddUpdateSystem(new Update2DSystem);
	CSystemManager::AddUpdateSystem(new UpdateTimerSystem);
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
	CSystemManager::AddUpdateSystem(new UpdateEnemySightSystem);
	CSystemManager::AddUpdateSystem(new UpdateVisibleSineCurveSystem);
	CSystemManager::AddUpdateSystem(new UpdateStutsAnimSystem);
	CSystemManager::AddUpdateSystem(new UpdateGateManagerSystem);

	// 描画システムを追加
	CSystemManager::AddRenderingSystem(new RenderingSkyBoxSystem);
	CSystemManager::AddRenderingSystem(new RenderingToShadowmapSystem);
	CSystemManager::AddRenderingSystem(new RenderingMapobjectSystem);
	CSystemManager::AddRenderingSystem(new RenderingToShapeShadowSystem);
	CSystemManager::AddRenderingSystem(new RenderingOutLineSystem);
	CSystemManager::AddRenderingSystem(new Render3DSystem);
	CSystemManager::AddRenderingSystem(new RenderXSystem);
	CSystemManager::AddRenderingSystem(new RenderingEnemySystem);
	CSystemManager::AddRenderingSystem(new RenderingTitlePlayerSystem);
	CSystemManager::AddRenderingSystem(new RenderingGamePlayerSystem);
	CSystemManager::AddRenderingSystem(new RenderMehFieldSystem);
	CSystemManager::AddRenderingSystem(new RenderMehLaerSystem);
	CSystemManager::AddRenderingSystem(new RenderingEnemySightSystem);
	CSystemManager::AddRenderingSystem(new RenderingVisibleSineCurveSystem);
	CSystemManager::AddRenderingSystem(new Render2DSystem);
	CSystemManager::AddRenderingSystem(new RenderingUICircleSystem);
	CSystemManager::AddRenderingSystem(new RenderingPauseMenuSystem);
}

//****************************************************************
// 終了処理
//****************************************************************
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

	CFade::Instance().Uninit();

	// シェーダー終了
	CShapeShadow::Instance().ReSet();
	CShadowMap::Instance().ReSet();
	CToon::Instance().ReSet();
	CDefaultCubemap::Instance().ReSet();
	CDistortion::Instance().ReSet();
	CUICircle::Instance().Reset();
	CVisibleSineCurve::Instance().Reset();
	CSimpleBloom::Instance().ReSet();
	// リソースマネージャーの解放
	CLoadTexture::UnRegistTex();
	CModelManager::UnRegistModel();
	// ２D音源の破棄
	CSound2D::Instance().Uninit();
}

//****************************************************************
// 更新処理
//****************************************************************
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

	CFade::Instance().Update();

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

//****************************************************************
// 描画処理
//****************************************************************
void CManager::Draw()
{
	if (m_Renderer != NULL)
	{
		m_Renderer->Draw();
	}
}

//****************************************************************
// シーンを設定
//****************************************************************
void CManager::SetScene(CScene* Scene)
{
	if (m_pScene == Scene)
	{
		delete Scene;
		return;
	}
	else if (m_pScene != NULL)
	{
		CSound2D::Instance().StopAll();
		m_pScene->Uninit();
		CSystemManager::EndSystem();
		m_pScene = NULL;
	}
	m_pScene = Scene;
	InitSystems();
	m_pScene->Init();
}