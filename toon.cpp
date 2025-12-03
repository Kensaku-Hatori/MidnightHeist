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
    GetHandle("g_vecLight") = pEffect->GetParameterByName(NULL, "g_vecLight");
    GetHandle("g_Deffuse") = pEffect->GetParameterByName(NULL, "g_Deffuse");
    GetHandle("g_mtxLightMtx") = pEffect->GetParameterByName(NULL, "g_mtxLightMtx");
    GetHandle("g_ShadowTexture") = pEffect->GetParameterByName(NULL, "g_ShadowTexture");
    GetHandle("g_LightView") = pEffect->GetParameterByName(NULL, "g_LightView");
    GetHandle("g_LightProj") = pEffect->GetParameterByName(NULL, "g_LightProj");
    GetHandle("g_ModelTexture") = pEffect->GetParameterByName(NULL, "g_ModelTexture");

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
    GetHandle("g_vecLight") = pEffect->GetParameterByName(NULL, "g_vecLight");
    GetHandle("g_Deffuse") = pEffect->GetParameterByName(NULL, "g_Deffuse");
    GetHandle("g_mtxLightMtx") = pEffect->GetParameterByName(NULL, "g_mtxLightMtx");
    GetHandle("g_ShadowTexture") = pEffect->GetParameterByName(NULL, "g_ShadowTexture");
    GetHandle("g_LightView") = pEffect->GetParameterByName(NULL, "g_LightView");
    GetHandle("g_LightProj") = pEffect->GetParameterByName(NULL, "g_LightProj");
    GetHandle("g_ModelTexture") = pEffect->GetParameterByName(NULL, "g_ModelTexture");
}

//***************************************
// パラメータを設定
//***************************************
void CToon::SetParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, D3DXVECTOR4 Light, D3DXVECTOR4 Col, LPDIRECT3DTEXTURE9 ShadowMap, LPDIRECT3DTEXTURE9 ModelTex,D3DXMATRIX LightView , D3DXMATRIX LightProj)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    pEffect->SetMatrix(GetHandle("g_mtxWorld"), &World);
    pEffect->SetMatrix(GetHandle("g_View"), &View);
    pEffect->SetMatrix(GetHandle("g_Proj"), &Proj);
    pEffect->SetVector(GetHandle("g_vecLight"), &Light);
    pEffect->SetVector(GetHandle("g_Deffuse"), &Col);
    pEffect->SetTexture(GetHandle("g_ShadowTexture"), ShadowMap);
    pEffect->SetMatrix(GetHandle("g_LightView"), &LightView);
    pEffect->SetMatrix(GetHandle("g_LightProj"), &LightProj);
    pEffect->SetTexture(GetHandle("g_ModelTexture"), ModelTex);

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