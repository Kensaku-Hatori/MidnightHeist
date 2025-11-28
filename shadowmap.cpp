//****************************************************************
//
// シャドウマップの処理[shadowmap.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "shadowmap.h"
#include "manager.h"

// 静的メンバ変数
std::unique_ptr<CShadowMap> CShadowMap::m_Instance = nullptr;

//***************************************
// デストラクタ
//***************************************
CShadowMap::~CShadowMap()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CShadowMap::Init(std::string Path)
{
    // 初期化処理
    HRESULT hr = CShader::Init(Path);

    // 初期化失敗
    if (FAILED(hr)) return E_FAIL;

    // エフェクト取得
    LPD3DXEFFECT pEffect = GetEffect();

    // 深度バッファの情報を取得
    IDirect3DSurface9* pSuf;
    D3DSURFACE_DESC SufDesc;

    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 既存の深度バッファを取得
    pDevice->GetDepthStencilSurface(&pSuf);

    // 情報取得
    pSuf->GetDesc(&SufDesc);

    // グローバル変数ハンドル取得
    GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    GetHandle("g_LightView") = pEffect->GetParameterByName(NULL, "g_LightView");
    GetHandle("g_LightProj") = pEffect->GetParameterByName(NULL, "g_LightProj");

    // シャドウマップのパラメータを設定
    m_LightVec = { 1.0f, 1.0f, 1.0f };
    m_MapRange = 2048;
    m_ZRange = 1500.0f;
    m_LightDistance = 400.0f;
    m_MapResolveW = 2048;
    m_MapResolveH = 2048;

    // テクスチャを作成
    pDevice->CreateTexture(m_MapResolveW, m_MapResolveH, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_R32F, D3DPOOL_DEFAULT, &m_ShadowMap, NULL);

    // サーフェイスを取得
    m_ShadowMap->GetSurfaceLevel(0, &m_ShadowMapSurface);

    // シャドウマップ用のZバッファ作成
    pDevice->CreateDepthStencilSurface(
        m_MapResolveW, m_MapResolveH,
        SufDesc.Format, SufDesc.MultiSampleType, SufDesc.MultiSampleQuality, TRUE,
        &m_ShadowDepthSurface, NULL);

    // 破棄
    pSuf->Release();

    return hr;
}

//***************************************
// レンダー周りのクリア
//***************************************
void CShadowMap::Clear(void)
{
    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 現在のレンダーターゲットを保存
    pDevice->GetRenderTarget(0, &m_OldRT);
    // 現在のレンダーターゲットを保存
    pDevice->GetDepthStencilSurface(&m_OldDepthSurface);

    // シャドウマップ用RTに切り替え
    pDevice->SetRenderTarget(0, m_ShadowMapSurface);
    // シャドウマップ用RTに切り替え
    pDevice->SetDepthStencilSurface(m_ShadowDepthSurface);

    // クリア
    pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, 0);

    // 元のRTに戻す
    pDevice->SetRenderTarget(0, m_OldRT);
    // 元のRTに戻す
    pDevice->SetDepthStencilSurface(m_OldDepthSurface);

    // 一時的に使った変数を破棄
    if (m_OldRT != nullptr)
    {
        m_OldRT->Release();
        m_OldRT = nullptr;
    }
    if (m_OldDepthSurface != nullptr)
    {
        m_OldDepthSurface->Release();
        m_OldDepthSurface = nullptr;
    }
}

//***************************************
// 書き込み開始
//***************************************
void CShadowMap::WriteMaps(void)
{
    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 現在のレンダーターゲットを保存
    pDevice->GetRenderTarget(0, &m_OldRT);
    // 現在のレンダーターゲットを保存
    pDevice->GetDepthStencilSurface(&m_OldDepthSurface);

    // シャドウマップ用RTに切り替え
    pDevice->SetRenderTarget(0, m_ShadowMapSurface);
    // シャドウマップ用RTに切り替え
    pDevice->SetDepthStencilSurface(m_ShadowDepthSurface);
}

//***************************************
// 書き込み終了
//***************************************
void CShadowMap::EndMaps(void)
{
    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 元のRTに戻す
    pDevice->SetRenderTarget(0, m_OldRT);
    // シャドウマップ用RTに切り替え
    pDevice->SetDepthStencilSurface(m_OldDepthSurface);

    // 一時的に使った変数を破棄
    if (m_OldRT != nullptr)
    {
        m_OldRT->Release();
        m_OldRT = nullptr;
    }
    if (m_OldDepthSurface != nullptr)
    {
        m_OldDepthSurface->Release();
        m_OldDepthSurface = nullptr;
    }
}

//***************************************
// デバイスリセット時
//***************************************
void CShadowMap::ReSet(void)
{
    // 破棄
    if (m_OldRT != nullptr)
    {
        m_OldRT->Release();
        m_OldRT = nullptr;
    }
    if (m_OldDepthSurface != nullptr)
    {
        m_OldDepthSurface->Release();
        m_OldDepthSurface = nullptr;
    }
    if (m_ShadowMap != nullptr)
    {
        m_ShadowMap->Release();
        m_ShadowMap = nullptr;
    }
    if (m_ShadowMapSurface != nullptr)
    {
        m_ShadowMapSurface->Release();
        m_ShadowMapSurface = nullptr;
    }
    if (m_ShadowDepthSurface != nullptr)
    {
        m_ShadowDepthSurface->Release();
        m_ShadowDepthSurface = nullptr;
    }
    Reset();
}

//***************************************
// リセット終了時
//***************************************
void CShadowMap::ReStart(void)
{
    // 深度バッファの情報を取得
    IDirect3DSurface9* pSuf;
    D3DSURFACE_DESC SufDesc;

    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 深度バッファを取得
    pDevice->GetDepthStencilSurface(&pSuf);

    // 情報取得
    pSuf->GetDesc(&SufDesc);

    // テクスチャを作成
    pDevice->CreateTexture(m_MapResolveW, m_MapResolveH, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_R32F, D3DPOOL_DEFAULT, &m_ShadowMap, NULL);
    m_ShadowMap->GetSurfaceLevel(0, &m_ShadowMapSurface);

    // シャドウマップ用のZバッファ作成
    pDevice->CreateDepthStencilSurface(
        m_MapResolveW, m_MapResolveH,
        SufDesc.Format, SufDesc.MultiSampleType, SufDesc.MultiSampleQuality, TRUE,
        &m_ShadowDepthSurface, NULL);

    // 破棄
    pSuf->Release();

    Restart();

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    GetHandle("g_LightView") = pEffect->GetParameterByName(NULL, "g_LightView");
    GetHandle("g_LightProj") = pEffect->GetParameterByName(NULL, "g_LightProj");
}

//***************************************
// パラメータを設定
//***************************************
void CShadowMap::SetParameters(D3DXMATRIX World)
{
    D3DXVECTOR3 LightPos;
    D3DXVECTOR3 LightTarget;
    D3DXVECTOR3 LightUp(0.0f, 1.0f, 0.0f);
    D3DXVECTOR3 CamPos = CManager::GetCamera()->GetPosR();
    LightPos = CamPos + m_LightVec * m_LightDistance;
    LightTarget = CamPos;

    D3DXMatrixLookAtLH(&m_LightView, &LightPos, &LightTarget, &LightUp);

    D3DXMatrixOrthoOffCenterLH(&m_LightProj, -m_MapRange, m_MapRange, -m_MapRange, m_MapRange, 0.1f, m_ZRange);

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    if (GetHandle("g_mtxWorld") != nullptr)
    {
        pEffect->SetMatrix(GetHandle("g_mtxWorld"), &World);
    }
    if (GetHandle("g_LightView") != nullptr)
    {
        pEffect->SetMatrix(GetHandle("g_LightView"), &m_LightView);
    }
    if (GetHandle("g_LightProj") != nullptr)
    {
        pEffect->SetMatrix(GetHandle("g_LightProj"), &m_LightProj);
    }

    // GPUに変更を適応
    pEffect->CommitChanges();
}

//***************************************
// インスタンスを取得
//***************************************
CShadowMap* CShadowMap::Instance(void)
{
	if (m_Instance != NULL) return m_Instance.get();
	m_Instance.reset(new CShadowMap);
	return m_Instance.get();
}