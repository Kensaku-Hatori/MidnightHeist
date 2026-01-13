//*************************************
//
//　レンダラー処理[renderer.cpp]
//　Author:Hatori Kensaku
//
//*************************************

// インクルード
#include "renderer.h"
#include "manager.h"
#include "line.h"
#include "texmanager.h"
#include "shadowmap.h"
#include "toon.h"
#include "shapeshadow.h"
#include "manager.h"
#include "scene.h"
#include "fade.h"
#include "SystemManager.h"
#include "distortion.h"

// 名前空間
using namespace std;

//************************************
// レンダラークラスのコンストラクタ
//************************************
CRenderer::CRenderer()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_d3dpp = {};
	m_BackBufferCol = { 0.1f,0.1f,0.1f,1.0f };
}

//************************************
// レンダラークラスのデストラクタ
//************************************
CRenderer::~CRenderer()
{

}

//************************************
// レンダラークラスの初期化処理
//************************************
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DDISPLAYMODE d3ddm;
	//Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	m_d3dpp.BackBufferFormat = d3ddm.Format;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_d3dpp.Windowed = bWindow;
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		&m_pD3DDevice)))
	{//Direct3Dデバイスの生成
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{//Direct3Dデバイスの生成
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// インスタンスを生成
	m_pLine = CLine::Create();

	// グリッドの間の大きさ
	float Spacing = 100.0f;

	// グリッドの分割数
	int Size = 500;

	// X方向に平行な（Z軸方向へ伸びる）線群
	for (int nCnt = -Size; nCnt <= Size; nCnt++)
	{
		// 線に必要な情報を代入する一時変数
		// 色
		D3DXCOLOR col = WHITE;

		// 始点
		D3DXVECTOR3 p1((float)Spacing * -Size, 0.0f, (float)Spacing * nCnt);

		// 終点
		D3DXVECTOR3 p2((float)Spacing * Size, 0.0f, (float)Spacing * nCnt);

		// 線を追加
		m_pLine->AddLine(p1, p2, col);
	}

	// Z方向に平行な（X軸方向へ伸びる）線群
	for (int nCnt = -Size; nCnt <= Size; nCnt++)
	{
		// 線に必要な情報を代入する一時変数
		// 色
		D3DXCOLOR col = WHITE;

		// 始点
		D3DXVECTOR3 p1((float)nCnt * Spacing, 0.0f, (float)-Size * Spacing);

		// 終点
		D3DXVECTOR3 p2((float)nCnt * Spacing, 0.0f, (float)Size * Spacing);

		// 線を追加
		m_pLine->AddLine(p1, p2, col);
	}

	// X用のマニュピレーター(下から見ても消えないように2個生成)
	m_pLine->AddLine({ 0.0f,0.1f,0.0f }, { 100.0f,0.1f,0.0f }, RED);
	m_pLine->AddLine({ 0.0f,-0.1f,0.0f }, { 100.0f,-0.1f,0.0f }, RED);

	// Y用のマニュピレーター
	m_pLine->AddLine({ 0.0f,0.0f,0.0f }, { 0.0f,100.0f,0.0f }, GREEN);

	// Z用のマニュピレーター(下から見ても消えないように2個生成)
	m_pLine->AddLine({ 0.0f,0.1f,0.0f }, { 0.0f,0.1f,100.0f }, BLUE);
	m_pLine->AddLine({ 0.0f,-0.1f,0.0f }, { 0.0f,-0.1f,100.0f }, BLUE);

	//頂点バッファの生成
	m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVertex,
		NULL);

	// 何もないテクスチャを作る
	m_pD3DDevice->CreateTexture(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_SceneTex,
		NULL
	);

	// テクスチャからサーフェイスレベルを取得
	m_SceneTex->GetSurfaceLevel(0, &m_SceneSurface);

	VERTEX_2D* pVtx = NULL;

	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

		pVtx[0].col = WHITE;
		pVtx[1].col = WHITE;
		pVtx[2].col = WHITE;
		pVtx[3].col = WHITE;

		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	m_pVertex->Unlock();

	return S_OK;
}

//************************************
// レンダラークラスの終了処理
//************************************
void CRenderer::Uninit()
{
	//----------------------------
	//各種オブジェクトの終了処理
	//----------------------------
	CSystemManager::EndSystem();

	// 頂点バッファの破棄
	if (m_pVertex != NULL)
	{
		m_pVertex->Release();
		m_pVertex = NULL;
	}
	// テクスチャの破棄
	if (m_SceneTex != NULL)
	{
		m_SceneTex->Release();
		m_SceneTex = NULL;
	}

	//D3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	//D3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//************************************
// レンダラークラスの更新処理
//************************************
void CRenderer::Update()
{
	if (CSystemManager::GetUpdateSystemSize != NULL)CSystemManager::UpdateAll(CManager::GetScene()->GetReg());

	static bool isFullScrean = false;
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F11) == true)
	{
		ToggleFullscreen(GetForegroundWindow(), &isFullScrean);
	}
	static bool isWire = false;
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F1) == true)
	{
		isWire = !isWire;
		if (isWire == true)
		{
			onWireFrame();
		}
		else
		{
			offWireFrame();
		}
	}
}

//************************************
// レンダラークラスの描画処理
//************************************
void CRenderer::Draw()
{
	//画面クリア
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		m_BackBufferCol, 1.0f, 0);

	// マップ系のテクスチャをクリア
	CShapeShadow::Instance()->Clear();
	CShadowMap::Instance()->Clear();

	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//描画開始成功時
		//-----------------------------
		//各種オブジェクトの描画処理
		//-----------------------------

		// 現在のバックバッファを保存する用の一時変数
		LPDIRECT3DSURFACE9 RenderDef;

		// 現在のレンダーターゲットを取得
		m_pD3DDevice->GetRenderTarget(0, &RenderDef);

		// レンダータゲットを変える
		m_pD3DDevice->SetRenderTarget(0, m_SceneSurface);

		//画面クリア
		m_pD3DDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			m_BackBufferCol, 1.0f, 0);

		// ラインの描画
		//m_pLine->Draw();

		// カメラの設定
		CCamera* pCamera = CManager::GetCamera();
		pCamera->SetCamera();

		// 描画システムの描画
		if (CSystemManager::GetRenderingSystemSize != NULL)CSystemManager::RenderingAll(CManager::GetScene()->GetReg());

		// シーンとフェードの描画
		CManager::GetScene()->Draw();
		CManager::GetFade()->Draw();

		// 物陰を描画
		CShapeShadow::Instance()->DrawTex();

		// レンダータゲットを元に戻す
		m_pD3DDevice->SetRenderTarget(0, RenderDef);

		// 歪みシェーダ起動
		CDistortion::Instance()->Begin();
		if (CDistortion::Instance()->GetNoiseCnt() <= 0)CDistortion::Instance()->BeginPass(1);
		else CDistortion::Instance()->BeginPass(0);

		// パラメータ設定
		CDistortion::Instance()->SetParameters(m_SceneTex);

		// シーン描画
		DrawScene();

		// 歪みシェーダ終了
		CDistortion::Instance()->EndPass();
		CDistortion::Instance()->End();

		//描画終了
		m_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//************************************
// シーンを描画
//************************************
void CRenderer::DrawScene(void)
{
	//頂点フォーマットの設定
	m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	//頂点バッファをデータストリームに設定
	m_pD3DDevice->SetStreamSource(0, m_pVertex, 0, sizeof(VERTEX_2D));

	//プレイヤーの描画
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//*************************************
// ワイヤーフレームをオンにする
//*************************************
void CRenderer::onWireFrame()
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

//*************************************
// ワイヤーフレームを切る
//*************************************
void CRenderer::offWireFrame()
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//*************************************
// フォグを有効にする
//*************************************
void CRenderer::onFog()
{
	SetupVertexFog(D3DCOLOR_XRGB(200, 200, 200), D3DFOG_LINEAR, TRUE, 0.01f);
}

//*************************************
// フォグを無効にする
//*************************************
void CRenderer::offFog()
{
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

//*************************************
// フルスクリーンを切り替える
//*************************************
void CRenderer::ToggleFullscreen(HWND hWnd, bool* isFullScrean)
{
	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (*isFullScrean)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, m_RectWindow.left, m_RectWindow.top,
			m_RectWindow.right - m_RectWindow.left, m_RectWindow.bottom - m_RectWindow.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(hWnd, &m_RectWindow);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	bool isScrean = *isFullScrean;
	*isFullScrean = !isScrean;
}

//*************************************
// フォグを設定
//*************************************
void CRenderer::SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density)
{
	float Start = 360.0f,    // Linear fog distances
		End = 5000.0f;

	// Enable fog blending.
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	// Set the fog color.
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, Color);
	// Set fog parameters.
	if (D3DFOG_LINEAR == Mode)
	{
		m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&Start));
		m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&End));
	}
	else
	{
		m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		m_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&Density));
	}

	// Enable range-based fog if desired (only supported for
	//   vertex fog). For this example, it is assumed that UseRange
	//   is set to a nonzero value only if the driver exposes the
	//   D3DPRASTERCAPS_FOGRANGE capability.
	// Note: This is slightly more performance intensive
	//   than non-range-based fog.

	if (UseRange) m_pD3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
}