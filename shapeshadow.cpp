#include "shapeshadow.h"
#include "resource1.h"
#include "manager.h"

CShapeShadow::~CShapeShadow()
{
}

HRESULT CShapeShadow::Init(void)
{
    // 初期化処理
    HRESULT hr = CShader::Init(HLSLID_SHAPESHADOW);

    // 初期化失敗
    if (FAILED(hr)) return E_FAIL;

    // エフェクト取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    GetHandle("g_View") = pEffect->GetParameterByName(NULL, "g_View");
    GetHandle("g_Proj") = pEffect->GetParameterByName(NULL, "g_Proj");
    GetHandle("g_ObjectTexture") = pEffect->GetParameterByName(NULL, "g_ObjectTexture");
    GetHandle("g_TargetTexture") = pEffect->GetParameterByName(NULL, "g_TargetTexture");

    // 深度バッファの幅と高さを取得
    UINT uiWidth;
    UINT uiHeight;

    // 既存の震度バッファ取得用
    IDirect3DSurface9* pSuf;
    D3DSURFACE_DESC SufDesc;

    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 既存の深度バッファを取得
    pDevice->GetDepthStencilSurface(&pSuf);

    // 情報取得
    pSuf->GetDesc(&SufDesc);
    uiWidth = SufDesc.Width;
    uiHeight = SufDesc.Height;


    // テクスチャを作成
    pDevice->CreateTexture(uiWidth, uiHeight, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_R32F, D3DPOOL_DEFAULT, &m_ObjectTex, NULL);

    // サーフェイスを取得
    m_ObjectTex->GetSurfaceLevel(0, &m_ObjectSurface);

    // シャドウマップ用のZバッファ作成
    pDevice->CreateDepthStencilSurface(
        uiWidth, uiHeight,
        SufDesc.Format, SufDesc.MultiSampleType, SufDesc.MultiSampleQuality, TRUE,
        &m_ObjectDepthSurface, NULL);




    // テクスチャを作成
    pDevice->CreateTexture(uiWidth, uiHeight, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_SceneTex, NULL);

    // サーフェイスを取得
    m_SceneTex->GetSurfaceLevel(0, &m_SceneSurface);

    // シャドウマップ用のZバッファ作成
    pDevice->CreateDepthStencilSurface(
        uiWidth, uiHeight,
        SufDesc.Format, SufDesc.MultiSampleType, SufDesc.MultiSampleQuality, TRUE,
        &m_SceneDepthSurface, NULL);

    // 破棄
    pSuf->Release();

    //頂点バッファの生成
    hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_2D,
        D3DPOOL_MANAGED,
        &m_pVertexBuffMT,
        NULL);

    VERTEX_2D* pVtx = NULL;

    m_pVertexBuffMT->Lock(0, 0, (void**)&pVtx, 0);

    if (pVtx != NULL)
    {
        pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

        pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
        pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
        pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
        pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

        pVtx[0].rhw =
            pVtx[1].rhw =
            pVtx[2].rhw =
            pVtx[3].rhw = 1.0f;

        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
    }

    m_pVertexBuffMT->Unlock();

    return hr;
}

void CShapeShadow::Draw(void)
{
    Begin();
    BeginPass(1);

    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    //頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, m_pVertexBuffMT, 0, sizeof(VERTEX_2D));

    //プレイヤーの描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

    EndPass();
    End();
}

void CShapeShadow::DrawTex(void)
{
    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    //頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, m_pVertexBuffMT, 0, sizeof(VERTEX_2D));

    //テクスチャの設定
    pDevice->SetTexture(0, m_SceneTex);
    //プレイヤーの描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void CShapeShadow::Clear(void)
{
    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 現在のレンダーターゲットを保存
    pDevice->GetRenderTarget(0, &m_OldRT);
    // 現在のレンダーターゲットを保存
    pDevice->GetDepthStencilSurface(&m_OldDepthSurface);

    // シャドウマップ用RTに切り替え
    pDevice->SetRenderTarget(0, m_ObjectSurface);
    // シャドウマップ用RTに切り替え
    pDevice->SetDepthStencilSurface(m_ObjectDepthSurface);

    // クリア
    pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

    // シャドウマップ用RTに切り替え
    pDevice->SetRenderTarget(0, m_SceneSurface);
    // シャドウマップ用RTに切り替え
    pDevice->SetDepthStencilSurface(m_SceneDepthSurface);

    // クリア
    pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

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

void CShapeShadow::BeginObject(void)
{
    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 現在のレンダーターゲットを保存
    pDevice->GetRenderTarget(0, &m_OldRT);
    // 現在のレンダーターゲットを保存
    pDevice->GetDepthStencilSurface(&m_OldDepthSurface);

    // シャドウマップ用RTに切り替え
    pDevice->SetRenderTarget(0, m_ObjectSurface);
    // シャドウマップ用RTに切り替え
    pDevice->SetDepthStencilSurface(m_ObjectDepthSurface);
}

void CShapeShadow::BeginScene(void)
{
    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 現在のレンダーターゲットを保存
    pDevice->GetRenderTarget(0, &m_OldRT);
    // 現在のレンダーターゲットを保存
    pDevice->GetDepthStencilSurface(&m_OldDepthSurface);

    // シャドウマップ用RTに切り替え
    pDevice->SetRenderTarget(0, m_SceneSurface);
    // シャドウマップ用RTに切り替え
    pDevice->SetDepthStencilSurface(m_SceneDepthSurface);
}

void CShapeShadow::EndTexs(void)
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

void CShapeShadow::ReSet(void)
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
    if (m_SceneTex != nullptr)
    {
        m_SceneTex->Release();
        m_SceneTex = nullptr;
    }
    if (m_SceneSurface != nullptr)
    {
        m_SceneSurface->Release();
        m_SceneSurface = nullptr;
    }
    if (m_SceneDepthSurface != nullptr)
    {
        m_SceneDepthSurface->Release();
        m_SceneDepthSurface = nullptr;
    }
    if (m_ObjectTex != nullptr)
    {
        m_ObjectTex->Release();
        m_ObjectTex = nullptr;
    }
    if (m_ObjectSurface != nullptr)
    {
        m_ObjectSurface->Release();
        m_ObjectSurface = nullptr;
    }
    if (m_ObjectDepthSurface != nullptr)
    {
        m_ObjectDepthSurface->Release();
        m_ObjectDepthSurface = nullptr;
    }
    Reset();
}

void CShapeShadow::ReStart(void)
{
    // 深度バッファの幅と高さを取得
    UINT uiWidth;
    UINT uiHeight;

    // 既存の震度バッファ取得用
    IDirect3DSurface9* pSuf;
    D3DSURFACE_DESC SufDesc;

    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // 既存の深度バッファを取得
    pDevice->GetDepthStencilSurface(&pSuf);

    // 情報取得
    pSuf->GetDesc(&SufDesc);
    uiWidth = SufDesc.Width;
    uiHeight = SufDesc.Height;


    // テクスチャを作成
    pDevice->CreateTexture(uiWidth, uiHeight, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_R32F, D3DPOOL_DEFAULT, &m_ObjectTex, NULL);

    // サーフェイスを取得
    m_ObjectTex->GetSurfaceLevel(0, &m_ObjectSurface);

    // シャドウマップ用のZバッファ作成
    pDevice->CreateDepthStencilSurface(
        uiWidth, uiHeight,
        SufDesc.Format, SufDesc.MultiSampleType, SufDesc.MultiSampleQuality, TRUE,
        &m_ObjectDepthSurface, NULL);




    // テクスチャを作成
    pDevice->CreateTexture(uiWidth, uiHeight, 1, D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_SceneTex, NULL);

    // サーフェイスを取得
    m_SceneTex->GetSurfaceLevel(0, &m_SceneSurface);

    // シャドウマップ用のZバッファ作成
    pDevice->CreateDepthStencilSurface(
        uiWidth, uiHeight,
        SufDesc.Format, SufDesc.MultiSampleType, SufDesc.MultiSampleQuality, TRUE,
        &m_SceneDepthSurface, NULL);

    // 破棄
    pSuf->Release();

    Restart();

    // エフェクト取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    GetHandle("g_View") = pEffect->GetParameterByName(NULL, "g_View");
    GetHandle("g_Proj") = pEffect->GetParameterByName(NULL, "g_Proj");
    GetHandle("g_ObjectTexture") = pEffect->GetParameterByName(NULL, "g_ObjectTexture");
}

void CShapeShadow::SetParameters(D3DXMATRIX World)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXMATRIX View, Proj;
    pDevice->GetTransform(D3DTS_VIEW, &View);
    pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

    // パラメータ(グローバル変数の設定)
    if (GetHandle("g_mtxWorld") != nullptr)
    {
        pEffect->SetMatrix(GetHandle("g_mtxWorld"), &World);
    }
    if (GetHandle("g_View") != nullptr)
    {
        pEffect->SetMatrix(GetHandle("g_View"), &View);
    }
    if (GetHandle("g_Proj") != nullptr)
    {
        pEffect->SetMatrix(GetHandle("g_Proj"), &Proj);
    }
    if (GetHandle("g_ObjectTexture") != nullptr)
    {
        pEffect->SetTexture(GetHandle("g_ObjectTexture"), m_ObjectTex);
    }

    // GPUに変更を適応
    pEffect->CommitChanges();
}