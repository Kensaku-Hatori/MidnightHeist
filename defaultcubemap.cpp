//****************************************************************
//
// デフォルトキューブマップの処理[defaultcubemap.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "defaultcubemap.h"
#include "manager.h"
#include "ShaderResource.h"

//***************************************
// 初期化処理
//***************************************
HRESULT CDefaultCubemap::Init(void)
{
    // 初期化
    HRESULT hr = CShader::Init(HLSLID_SKYBOX);

    // 初期化失敗
    if (FAILED(hr)) return E_FAIL;

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    GetHandle("g_View") = pEffect->GetParameterByName(NULL, "g_View");
    GetHandle("g_Proj") = pEffect->GetParameterByName(NULL, "g_Proj");
    GetHandle("g_CubeMap") = pEffect->GetParameterByName(NULL, "g_CubeMap");

    return S_OK;
}

//***************************************
// リセット
//***************************************
void CDefaultCubemap::ReSet(void)
{
    Reset();
}

//***************************************
// リスタート
//***************************************
void CDefaultCubemap::ReStart(void)
{
    Restart();

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    GetHandle("g_View") = pEffect->GetParameterByName(NULL, "g_View");
    GetHandle("g_Proj") = pEffect->GetParameterByName(NULL, "g_Proj");
    GetHandle("g_CubeMap") = pEffect->GetParameterByName(NULL, "g_CubeMap");
}

//***************************************
// パラメータ設定
//***************************************
void CDefaultCubemap::SetParameters(const D3DXMATRIX& World, const LPDIRECT3DCUBETEXTURE9& Tex)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    D3DXMATRIX view, proj;
    CRenderer* pRenderer;
    pRenderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

    pDevice->GetTransform(D3DTS_VIEW, &view);
    pDevice->GetTransform(D3DTS_PROJECTION, &proj);

    // パラメータ(グローバル変数の設定)
    pEffect->SetMatrix(GetHandle("g_mtxWorld"), &World);
    pEffect->SetMatrix(GetHandle("g_View"), &view);
    pEffect->SetMatrix(GetHandle("g_Proj"), &proj);
    pEffect->SetTexture(GetHandle("g_CubeMap"), Tex);

    // GPUに変更を適応
    pEffect->CommitChanges();
}