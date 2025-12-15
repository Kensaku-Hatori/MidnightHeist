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
    GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    GetHandle("g_mtxView") = pEffect->GetParameterByName(NULL, "g_mtxView");
    GetHandle("g_mtxProj") = pEffect->GetParameterByName(NULL, "g_mtxProj");
    GetHandle("g_FillAmount") = pEffect->GetParameterByName(NULL, "g_FillAmount");
    GetHandle("g_Center") = pEffect->GetParameterByName(NULL, "g_Center");

    return S_OK;
}

//***************************************
// パラメータ設定
//***************************************
void CUICircle::SetParameters(const D3DXMATRIX& World, const D3DXMATRIX& OriginWorld, const float FillAmount)
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
    if (GetHandle("g_mtxView") != nullptr)
    {
        pEffect->SetMatrix(GetHandle("g_mtxView"), &View);
    }
    if (GetHandle("g_mtxProj") != nullptr)
    {
        pEffect->SetMatrix(GetHandle("g_mtxProj"), &Proj);
    }
    if (GetHandle("g_FillAmount") != nullptr)
    {
        pEffect->SetFloat(GetHandle("g_FillAmount"), FillAmount);
    }
    if (GetHandle("g_Center") != nullptr)
    {
        D3DXVECTOR3 Convert = { OriginWorld._41,OriginWorld._42,OriginWorld._43 };
        D3DXVECTOR4 Set = { OriginWorld._41,OriginWorld._42,OriginWorld._43,1.0f };
        D3DXVec3TransformCoord(&Convert, &Convert, &View);
        Set.x = Convert.x;
        Set.y = Convert.y;
        Set.z = Convert.z;

        pEffect->SetVector(GetHandle("g_Center"),&Set);
    }

    // GPUに変更を適応
    pEffect->CommitChanges();
}