//****************************************************************
//
// シンプルなブルームの処理[SimpleBloom.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "SimpleBloom.h"
#include "ShaderResource.h"
#include "manager.h"

//***************************************
// デストラクタ
//***************************************
CSimpleBloom::~CSimpleBloom()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CSimpleBloom::Init(void)
{
    // 初期化
    HRESULT hr = CShader::Init(HLSLID_SIMPLEBLOOM);

    // 初期化失敗
    if (FAILED(hr)) return E_FAIL;

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    m_SceneTexHandle = GetHandle("g_SceneTexture") = pEffect->GetParameterByName(NULL, "g_SceneTexture");
    m_BrightMapHandle = GetHandle("g_BrightTexture") = pEffect->GetParameterByName(NULL, "g_BrightTexture");
    m_BloomMapHandle = GetHandle("g_BloomTexture") = pEffect->GetParameterByName(NULL, "g_BloomTexture");

    // 深度バッファの情報を取得
    IDirect3DSurface9* pSuf;
    D3DSURFACE_DESC SufDesc;

    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 既存の深度バッファを取得
    pDevice->GetDepthStencilSurface(&pSuf);

    // 情報取得
    pSuf->GetDesc(&SufDesc);

    // テクスチャを作成
    pDevice->CreateTexture(SufDesc.Width, SufDesc.Height, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_BrightMap, NULL);

    // サーフェイスを取得
    m_BrightMap->GetSurfaceLevel(0, &m_BrightMapSurface);

    // テクスチャを作成
    pDevice->CreateTexture(SufDesc.Width, SufDesc.Height, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &m_BloomMap, NULL);

    // サーフェイスを取得
    m_BloomMap->GetSurfaceLevel(0, &m_BloomMapSurface);

    // 破棄
    pSuf->Release();

    return S_OK;
}

//***************************************
// リセット
//***************************************
void CSimpleBloom::ReSet(void)
{
    CShader::Reset();
}

//***************************************
// リスタート
//***************************************
void CSimpleBloom::ReStart(void)
{
    CShader::Restart();
}

//***************************************
// パラメータ設定
//***************************************
void CSimpleBloom::SetBrightParameters(LPDIRECT3DTEXTURE9 Scene)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    if (m_SceneTexHandle != nullptr)
    {
        pEffect->SetTexture(m_SceneTexHandle, Scene);
    }

    // GPUに変更を適応
    pEffect->CommitChanges();
}

//***************************************
// パラメータ設定
//***************************************
void CSimpleBloom::SetBloomParameters(void)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    if (m_BrightMapHandle != nullptr)
    {
        pEffect->SetTexture(m_BrightMapHandle, m_BrightMap);
    }

    // GPUに変更を適応
    pEffect->CommitChanges();
}

//***************************************
// ブルームマップをGPUに渡す
//***************************************
void CSimpleBloom::SetBloomMap(void)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    if (m_BloomMapHandle != nullptr)
    {
        pEffect->SetTexture(m_BloomMapHandle, m_BloomMap);
    }

    // GPUに変更を適応
    pEffect->CommitChanges();
}

//***************************************
// 輝度抽出マップへの書き込み開始
//***************************************
void CSimpleBloom::BeginBrightMap(void)
{
    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 現在のレンダーターゲットを保存
    pDevice->GetRenderTarget(0, &m_OldRT);

    // シャドウマップ用RTに切り替え
    pDevice->SetRenderTarget(0, m_BrightMapSurface);
}

//***************************************
// 輝度抽出マップへの書き込み終了
//***************************************
void CSimpleBloom::EndBrightMap(void)
{
    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 元のRTに戻す
    pDevice->SetRenderTarget(0, m_OldRT);

    // 一時的に使った変数を破棄
    if (m_OldRT != nullptr)
    {
        m_OldRT->Release();
        m_OldRT = nullptr;
    }
}

//***************************************
// ブルームの書き込み開始
//***************************************
void CSimpleBloom::BeginBloomMap(void)
{
    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 現在のレンダーターゲットを保存
    pDevice->GetRenderTarget(0, &m_OldRT);

    // シャドウマップ用RTに切り替え
    pDevice->SetRenderTarget(0, m_BloomMapSurface);
}

//***************************************
// ブルームの書き込み終了
//***************************************
void CSimpleBloom::EndBloomMap(void)
{
    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 元のRTに戻す
    pDevice->SetRenderTarget(0, m_OldRT);

    // 一時的に使った変数を破棄
    if (m_OldRT != nullptr)
    {
        m_OldRT->Release();
        m_OldRT = nullptr;
    }
}

//***************************************
// クリア
//***************************************
void CSimpleBloom::ClearAll(void)
{
    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 現在のレンダーターゲットを保存
    pDevice->GetRenderTarget(0, &m_OldRT);

    // シャドウマップ用RTに切り替え
    pDevice->SetRenderTarget(0, m_BrightMapSurface);

    // クリア
    pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

    // シャドウマップ用RTに切り替え
    pDevice->SetRenderTarget(0, m_BloomMapSurface);

    // クリア
    pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

    // 元のRTに戻す
    pDevice->SetRenderTarget(0, m_OldRT);

    // 一時的に使った変数を破棄
    if (m_OldRT != nullptr)
    {
        m_OldRT->Release();
        m_OldRT = nullptr;
    }
}