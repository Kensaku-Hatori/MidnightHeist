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
    GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    GetHandle("g_View") = pEffect->GetParameterByName(NULL, "g_View");
    GetHandle("g_Proj") = pEffect->GetParameterByName(NULL, "g_Proj");
    GetHandle("g_Deffuse") = pEffect->GetParameterByName(NULL, "g_Deffuse");
    GetHandle("g_mtxLightMtx") = pEffect->GetParameterByName(NULL, "g_mtxLightMtx");
    GetHandle("g_ShadowTexture") = pEffect->GetParameterByName(NULL, "g_ShadowTexture");
    GetHandle("g_LightView") = pEffect->GetParameterByName(NULL, "g_LightView");
    GetHandle("g_LightProj") = pEffect->GetParameterByName(NULL, "g_LightProj");
    GetHandle("g_vecLight") = pEffect->GetParameterByName(NULL, "g_vecLight");
    GetHandle("g_ModelTexture") = pEffect->GetParameterByName(NULL, "g_ModelTexture");
    GetHandle("g_ToonMap") = pEffect->GetParameterByName(NULL, "g_ToonMap");

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
    GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    GetHandle("g_View") = pEffect->GetParameterByName(NULL, "g_View");
    GetHandle("g_Proj") = pEffect->GetParameterByName(NULL, "g_Proj");
    GetHandle("g_Deffuse") = pEffect->GetParameterByName(NULL, "g_Deffuse");
    GetHandle("g_mtxLightMtx") = pEffect->GetParameterByName(NULL, "g_mtxLightMtx");
    GetHandle("g_ShadowTexture") = pEffect->GetParameterByName(NULL, "g_ShadowTexture");
    GetHandle("g_LightView") = pEffect->GetParameterByName(NULL, "g_LightView");
    GetHandle("g_LightProj") = pEffect->GetParameterByName(NULL, "g_LightProj");
    GetHandle("g_vecLight") = pEffect->GetParameterByName(NULL, "g_vecLight");
    GetHandle("g_ModelTexture") = pEffect->GetParameterByName(NULL, "g_ModelTexture");
    GetHandle("g_ToonMap") = pEffect->GetParameterByName(NULL, "g_ToonMap");
}

//***************************************
// シャドウマップを使う描画用のパラメーター設定
//***************************************
void CToon::SetUseShadowMapParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, D3DXVECTOR4 Col, LPDIRECT3DTEXTURE9 ShadowMap, LPDIRECT3DTEXTURE9 ModelTex, D3DXMATRIX LightView, D3DXMATRIX LightProj)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    pEffect->SetMatrix(GetHandle("g_mtxWorld"), &World);
    pEffect->SetMatrix(GetHandle("g_View"), &View);
    pEffect->SetMatrix(GetHandle("g_Proj"), &Proj);
    pEffect->SetVector(GetHandle("g_Deffuse"), &Col);
    pEffect->SetTexture(GetHandle("g_ShadowTexture"), ShadowMap);
    pEffect->SetMatrix(GetHandle("g_LightView"), &LightView);
    pEffect->SetMatrix(GetHandle("g_LightProj"), &LightProj);
    D3DXVECTOR4 LightVec = { 0.5f, 1.0f, 0.5f ,1.0f };
    pEffect->SetVector(GetHandle("g_vecLight"), &LightVec);
    pEffect->SetTexture(GetHandle("g_ModelTexture"), ModelTex);
    pEffect->SetTexture(GetHandle("g_ToonMap"), m_ToonMap);

    // GPUに変更を適応
    pEffect->CommitChanges();
}

//***************************************
// アウトラインのパラメーター設定
//***************************************
void CToon::SetUseOutLineParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    pEffect->SetMatrix(GetHandle("g_mtxWorld"), &World);
    pEffect->SetMatrix(GetHandle("g_View"), &View);
    pEffect->SetMatrix(GetHandle("g_Proj"), &Proj);

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