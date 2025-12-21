//****************************************************************
//
// サインカーブシェーダーの処理[VisibleSineCurve.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "VisibleSineCurve.h"
#include "manager.h"
#include "ShaderResource.h"

//***************************************
// 初期化
//***************************************
HRESULT CVisibleSineCurve::Init(void)
{
    // 初期化
    HRESULT hr = CShader::Init(HLSLID_VISIBLESINECURVE);

    // 初期化失敗
    if (FAILED(hr)) return E_FAIL;

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    m_SineCount = GetHandle("g_SineCount") = pEffect->GetParameterByName(NULL, "g_SineCount");
    m_SineMicro = GetHandle("g_SineMicro") = pEffect->GetParameterByName(NULL, "g_SineMicro");
    m_SineRipple = GetHandle("g_SineRipple") = pEffect->GetParameterByName(NULL, "g_SineRipple");
    m_SineSpeed = GetHandle("g_SineSpeed") = pEffect->GetParameterByName(NULL, "g_SineSpeed");
    m_CenterPos = GetHandle("g_Center") = pEffect->GetParameterByName(NULL, "g_Center");
    m_mtxWorldHandle = GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    m_ProjHandle = GetHandle("g_mtxProj") = pEffect->GetParameterByName(NULL, "g_mtxProj");
    m_ViewHandle = GetHandle("g_mtxView") = pEffect->GetParameterByName(NULL, "g_mtxView");

    return S_OK;
}

//***************************************
// パラメータ設定
//***************************************
void CVisibleSineCurve::SetParameters(D3DXMATRIX mtxWorld,const int Counter, const float Speed, const float Ripple, const float Micro)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    pEffect->SetInt(m_SineCount, Counter);
    pEffect->SetFloat(m_SineMicro, Micro);
    pEffect->SetFloat(m_SineRipple, Ripple);
    pEffect->SetFloat(m_SineSpeed, Speed);

    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXMATRIX View, Proj;
    pDevice->GetTransform(D3DTS_VIEW, &View);
    pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

    D3DXVECTOR3 Convert = { mtxWorld._41,mtxWorld._42,mtxWorld._43 };
    D3DXVECTOR4 Set = { mtxWorld._41,mtxWorld._42,mtxWorld._43,1.0f };
    D3DXVec3TransformCoord(&Convert, &Convert, &View);
    Set.x = Convert.x;
    Set.y = Convert.y;
    Set.z = Convert.z;

    pEffect->SetVector(m_CenterPos, &Set);
    pEffect->SetMatrix(m_mtxWorldHandle, &mtxWorld);
    pEffect->SetMatrix(m_ViewHandle, &View);
    pEffect->SetMatrix(m_ProjHandle, &Proj);
}