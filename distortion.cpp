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
    GetHandle("g_NoiseSpeed") = pEffect->GetParameterByName(NULL, "g_NoiseSpeed");
    GetHandle("g_NoiseMinRange") = pEffect->GetParameterByName(NULL, "g_NoiseMinRange");
    GetHandle("g_NoiseMaxRange") = pEffect->GetParameterByName(NULL, "g_NoiseMaxRange");

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
    GetHandle("g_NoiseSpeed") = pEffect->GetParameterByName(NULL, "g_NoiseSpeed");
    GetHandle("g_NoiseMinRange") = pEffect->GetParameterByName(NULL, "g_NoiseMinRange");
    GetHandle("g_NoiseMaxRange") = pEffect->GetParameterByName(NULL, "g_NoiseMaxRange");
}

//***************************************
// ノイズスタート
//***************************************
void CDistortion::StartNoise(float MaxNoiseRange, float MinNoiseRange, float NoiseSpeed)
{
    m_NoiseFrag = true;
    m_NoiseCount = 0;
    m_NoiseSpeed = NoiseSpeed;
    m_NoiseMaxRange = MaxNoiseRange;
    m_NoiseMinRange = MinNoiseRange;
}

//***************************************
// ノイズ終了
//***************************************
void CDistortion::EndNoise(void)
{
    m_NoiseFrag = false;
    m_NoiseCount = 0;
}

//***************************************
// パラメータ設定
//***************************************
void CDistortion::SetParameters(LPDIRECT3DTEXTURE9 Scene)
{
    if (m_NoiseFrag == true)m_NoiseCount++;
    if (m_NoiseCount > m_NoiseSpeed)m_NoiseFrag = false;

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    pEffect->SetTexture(GetHandle("g_SceneTexture"), Scene);
    pEffect->SetInt(GetHandle("g_nCnt"), m_NoiseCount);
    pEffect->SetFloat(GetHandle("g_NoiseSpeed"), m_NoiseSpeed);
    pEffect->SetFloat(GetHandle("g_NoiseMinRange"), m_NoiseMinRange);
    pEffect->SetFloat(GetHandle("g_NoiseMaxRange"), m_NoiseMaxRange);
}