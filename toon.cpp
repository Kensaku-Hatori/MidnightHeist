//****************************************************************
//
// トゥーンシェーダーの処理[toon.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "toon.h"
#include "manager.h"
#include "ShaderResource.h"
#include "shadowmap.h"
#include "texmanager.h"

// 静的メンバ変数
std::unique_ptr<CToon> CToon::m_Instance = nullptr;

//***************************************
// デストラクタ
//***************************************
CToon::~CToon()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CToon::Init(void)
{
    // 初期化
    HRESULT hr = CShader::Init(HLSLID_TOON);

    // 初期化失敗
    if (FAILED(hr)) return E_FAIL;

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    m_mtxWorldHandle = GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    m_ViewHandle =GetHandle("g_View") = pEffect->GetParameterByName(NULL, "g_View");
    m_ProjHandle = GetHandle("g_Proj") = pEffect->GetParameterByName(NULL, "g_Proj");
    m_DeffuseHandle = GetHandle("g_Deffuse") = pEffect->GetParameterByName(NULL, "g_Deffuse");
    m_ShadowMapHandle = GetHandle("g_ShadowTexture") = pEffect->GetParameterByName(NULL, "g_ShadowTexture");
    m_LightViewHandle = GetHandle("g_LightView") = pEffect->GetParameterByName(NULL, "g_LightView");
    m_LightProjHandle = GetHandle("g_LightProj") = pEffect->GetParameterByName(NULL, "g_LightProj");
    m_VecLightHandle = GetHandle("g_vecLight") = pEffect->GetParameterByName(NULL, "g_vecLight");
    m_ModelTexHandle = GetHandle("g_ModelTexture") = pEffect->GetParameterByName(NULL, "g_ModelTexture");
    m_ToonMapHandle = GetHandle("g_ToonMap") = pEffect->GetParameterByName(NULL, "g_ToonMap");
    m_OutLineHeightHandle = GetHandle("g_Height") = pEffect->GetParameterByName(NULL, "g_Height");
    m_OutLineThicknessHandle = GetHandle("g_Thickness") = pEffect->GetParameterByName(NULL, "g_Thickness");
    m_OutLineColorHandle = GetHandle("g_OutLineColor") = pEffect->GetParameterByName(NULL, "g_OutLineColor");

    m_ToonMap = CLoadTexture::GetTex("data/TEXTURE/toon.jpg");

    return S_OK;
}

//***************************************
// シェーダをリセット
//***************************************
void CToon::ReSet(void)
{
    Reset();
}

//***************************************
// シェーダーをリスタート
//***************************************
void CToon::ReStart(void)
{
    Restart();

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    m_mtxWorldHandle = GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    m_ViewHandle = GetHandle("g_View") = pEffect->GetParameterByName(NULL, "g_View");
    m_ProjHandle = GetHandle("g_Proj") = pEffect->GetParameterByName(NULL, "g_Proj");
    m_DeffuseHandle = GetHandle("g_Deffuse") = pEffect->GetParameterByName(NULL, "g_Deffuse");
    m_ShadowMapHandle = GetHandle("g_ShadowTexture") = pEffect->GetParameterByName(NULL, "g_ShadowTexture");
    m_LightViewHandle = GetHandle("g_LightView") = pEffect->GetParameterByName(NULL, "g_LightView");
    m_LightProjHandle = GetHandle("g_LightProj") = pEffect->GetParameterByName(NULL, "g_LightProj");
    m_VecLightHandle = GetHandle("g_vecLight") = pEffect->GetParameterByName(NULL, "g_vecLight");
    m_ModelTexHandle = GetHandle("g_ModelTexture") = pEffect->GetParameterByName(NULL, "g_ModelTexture");
    m_ToonMapHandle = GetHandle("g_ToonMap") = pEffect->GetParameterByName(NULL, "g_ToonMap");
    m_OutLineHeightHandle = GetHandle("g_Height") = pEffect->GetParameterByName(NULL, "g_Height");
    m_OutLineThicknessHandle = GetHandle("g_Thickness") = pEffect->GetParameterByName(NULL, "g_Thickness");
    m_OutLineColorHandle = GetHandle("g_OutLineColor") = pEffect->GetParameterByName(NULL, "g_OutLineColor");
}

//***************************************
// シャドウマップを使う描画用のパラメーター設定
//***************************************
void CToon::SetUseShadowMapParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, D3DXVECTOR4 Col, LPDIRECT3DTEXTURE9 ShadowMap, LPDIRECT3DTEXTURE9 ModelTex, D3DXMATRIX LightView, D3DXMATRIX LightProj)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    pEffect->SetMatrix(m_mtxWorldHandle, &World);
    pEffect->SetMatrix(m_ViewHandle, &View);
    pEffect->SetMatrix(m_ProjHandle, &Proj);
    pEffect->SetVector(m_DeffuseHandle, &Col);
    pEffect->SetTexture(m_ShadowMapHandle, ShadowMap);
    pEffect->SetMatrix(m_LightViewHandle, &LightView);
    pEffect->SetMatrix(m_LightProjHandle, &LightProj);
    D3DXVECTOR4 LightVec = { 0.5f, 1.0f, 0.5f ,1.0f };
    pEffect->SetVector(m_VecLightHandle, &LightVec);
    pEffect->SetTexture(m_ModelTexHandle, ModelTex);
    pEffect->SetTexture(m_ToonMapHandle, m_ToonMap);

    // GPUに変更を適応
    pEffect->CommitChanges();
}

//***************************************
// アウトラインのパラメーター設定
//***************************************
void CToon::SetUseOutLineParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, float Thickness, D3DXVECTOR4 Color, float Height)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    pEffect->SetMatrix(m_mtxWorldHandle, &World);
    pEffect->SetMatrix(m_ViewHandle, &View);
    pEffect->SetMatrix(m_ProjHandle, &Proj);
    pEffect->SetFloat(m_OutLineHeightHandle, Height);
    pEffect->SetFloat(m_OutLineThicknessHandle, Thickness);
    pEffect->SetVector(m_OutLineColorHandle, &Color);

    // GPUに変更を適応
    pEffect->CommitChanges();
}

//***************************************
// インスタンスを取得
//***************************************
CToon* CToon::Instance(void)
{
    // 生成済みなら
	if (m_Instance != NULL) return m_Instance.get();

    // メモリ確保
	m_Instance.reset(new CToon);

    // 返す
	return m_Instance.get();
}