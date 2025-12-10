//****************************************************************
//
// 画面の歪みの処理[distortion.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "distortion.h"
#include "ShaderResource.h"

//***************************************
// デストラクタ
//***************************************
CDistortion::~CDistortion()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CDistortion::Init(void)
{
    // 初期化
    HRESULT hr = CShader::Init(HLSLID_DISTORTION);

    // 初期化失敗
    if (FAILED(hr)) return E_FAIL;

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    GetHandle("g_SceneTexture") = pEffect->GetParameterByName(NULL, "g_SceneTexture");
    GetHandle("g_nCnt") = pEffect->GetParameterByName(NULL, "g_nCnt");

    return S_OK;
}

//***************************************
// リセット
//***************************************
void CDistortion::ReSet(void)
{
    CShader::Reset();
}

//***************************************
// リスタート
//***************************************
void CDistortion::ReStart(void)
{
    CShader::Restart();
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    GetHandle("g_SceneTexture") = pEffect->GetParameterByName(NULL, "g_SceneTexture");
    GetHandle("g_nCnt") = pEffect->GetParameterByName(NULL, "g_nCnt");
}

//***************************************
// パラメータ設定
//***************************************
void CDistortion::SetParameters(LPDIRECT3DTEXTURE9 Scene)
{
    static int test = 0;
    test++;

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    pEffect->SetTexture(GetHandle("g_SceneTexture"), Scene);
    pEffect->SetInt(GetHandle("g_nCnt"), test);
}