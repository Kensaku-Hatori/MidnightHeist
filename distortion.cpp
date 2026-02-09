//****************************************************************
//
// 画面の歪みの処理[distortion.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "distortion.h"
#include "ShaderResource.h"
#include "texmanager.h"

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
    m_SceneTexHandle = GetHandle("g_SceneTexture") = pEffect->GetParameterByName(NULL, "g_SceneTexture");
    m_NoizeTexHandle = GetHandle("g_NoizeTexture") = pEffect->GetParameterByName(NULL, "g_NoizeTexture");
    m_CounterHandle = GetHandle("g_nCnt") = pEffect->GetParameterByName(NULL, "g_nCnt");
    m_SpeedHandle = GetHandle("g_NoiseSpeed") = pEffect->GetParameterByName(NULL, "g_NoiseSpeed");
    m_MinRangeHandle = GetHandle("g_NoiseMinRange") = pEffect->GetParameterByName(NULL, "g_NoiseMinRange");
    m_MaxRangeHandle = GetHandle("g_NoiseMaxRange") = pEffect->GetParameterByName(NULL, "g_NoiseMaxRange");
    // テクスチャ取得
    m_NoizeTex = CLoadTexture::GetTex("data/TEXTURE/ParlinNoize.png");

    return S_OK;
}

//***************************************
// リセット
//***************************************
void CDistortion::ReSet(void)
{
    if (m_NoizeTex != nullptr)
    {
        m_NoizeTex->Release();
        m_NoizeTex = nullptr;
    }
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
    m_SceneTexHandle = GetHandle("g_SceneTexture") = pEffect->GetParameterByName(NULL, "g_SceneTexture");
    m_NoizeTexHandle = GetHandle("g_NoizeTexture") = pEffect->GetParameterByName(NULL, "g_NoizeTexture");
    m_CounterHandle = GetHandle("g_nCnt") = pEffect->GetParameterByName(NULL, "g_nCnt");
    m_SpeedHandle = GetHandle("g_NoiseSpeed") = pEffect->GetParameterByName(NULL, "g_NoiseSpeed");
    m_MinRangeHandle = GetHandle("g_NoiseMinRange") = pEffect->GetParameterByName(NULL, "g_NoiseMinRange");
    m_MaxRangeHandle = GetHandle("g_NoiseMaxRange") = pEffect->GetParameterByName(NULL, "g_NoiseMaxRange");
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
    if (m_NoiseCount > m_NoiseSpeed)
    {
        EndNoise();
    }

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    pEffect->SetTexture(m_SceneTexHandle, Scene);
    pEffect->SetTexture(m_NoizeTexHandle, m_NoizeTex);

    pEffect->SetInt(m_CounterHandle, m_NoiseCount);
    pEffect->SetFloat(m_SpeedHandle, m_NoiseSpeed);
    pEffect->SetFloat(m_MinRangeHandle, m_NoiseMinRange);
    pEffect->SetFloat(m_MaxRangeHandle, m_NoiseMaxRange);
}