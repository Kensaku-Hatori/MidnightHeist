//****************************************************************
//
// 円形UIの処理[UICircle.h]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UICircle.h"
#include "manager.h"
#include "ShaderResource.h"
#include "math_T.h"

//***************************************
// 初期化
//***************************************
HRESULT CUICircle::Init(void)
{
    // 初期化
    HRESULT hr = CShader::Init(HLSLID_UICIRCLE);

    // 初期化失敗
    if (FAILED(hr)) return E_FAIL;

    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    m_mtxWorldHandle = GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    m_ViewHandle = GetHandle("g_mtxView") = pEffect->GetParameterByName(NULL, "g_mtxView");
    m_ProjHandle = GetHandle("g_mtxProj") = pEffect->GetParameterByName(NULL, "g_mtxProj");
    m_FillAmoutHandle = GetHandle("g_FillAmount") = pEffect->GetParameterByName(NULL, "g_FillAmount");
    m_CenterHandle = GetHandle("g_Center") = pEffect->GetParameterByName(NULL, "g_Center");
    m_RadiusHandle = GetHandle("g_Radius") = pEffect->GetParameterByName(NULL, "g_Radius");
    m_MaxFillHandle = GetHandle("g_MaxFillAmount") = pEffect->GetParameterByName(NULL, "g_MaxFillAmount");

    return S_OK;
}

//***************************************
// パラメータ設定
//***************************************
void CUICircle::SetParameters(const D3DXMATRIX& World, const D3DXMATRIX& OriginWorld, const float FillAmount, const float Radius, const float MaxFillAngle)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // デバイス取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    D3DXMATRIX View, Proj;
    pDevice->GetTransform(D3DTS_VIEW, &View);
    pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

    // パラメータ(グローバル変数の設定)
    if (m_mtxWorldHandle != nullptr)
    {
        pEffect->SetMatrix(m_mtxWorldHandle, &World);
    }
    if (m_ViewHandle != nullptr)
    {
        pEffect->SetMatrix(m_ViewHandle, &View);
    }
    if (m_ProjHandle != nullptr)
    {
        pEffect->SetMatrix(m_ProjHandle, &Proj);
    }
    if (m_FillAmoutHandle != nullptr)
    {
        pEffect->SetFloat(m_FillAmoutHandle, FillAmount);
    }
    if (m_CenterHandle != nullptr)
    {
        D3DXVECTOR3 Convert = { OriginWorld._41,OriginWorld._42,OriginWorld._43 };
        D3DXVECTOR4 Set = { OriginWorld._41,OriginWorld._42,OriginWorld._43,1.0f };
        D3DXVec3TransformCoord(&Convert, &Convert, &View);
        Set.x = Convert.x;
        Set.y = Convert.y;
        Set.z = Convert.z;

        pEffect->SetVector(m_CenterHandle,&Set);
    }
    if (m_RadiusHandle != nullptr)
    {
        pEffect->SetFloat(m_RadiusHandle, Radius);
    }
    if (m_MaxFillHandle != nullptr)
    {
        pEffect->SetFloat(m_MaxFillHandle, MaxFillAngle);
    }

    // GPUに変更を適応
    pEffect->CommitChanges();
}