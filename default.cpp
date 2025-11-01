//****************************************************************
//
// 通常描画の処理[default.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "default.h"
#include "manager.h"

// 静的メンバ変数
std::unique_ptr<CDefoult> CDefoult::m_Instance = nullptr;

//***************************************
// デストラクタ
//***************************************
CDefoult::~CDefoult()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CDefoult::Init(std::string Path)
{
    CRenderer* pRenderer;
    pRenderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

    ID3DXBuffer* pError = nullptr;
    HRESULT hr = D3DXCreateEffectFromFile(pDevice,
        Path.c_str(),
        nullptr,
        nullptr,
        D3DXSHADER_DEBUG,
        nullptr,
        &m_pEffect,
        &pError);

    if (FAILED(hr))
    {
        if (pError)
        {
            MessageBox(nullptr, (char*)pError->GetBufferPointer(), "Shader Error", MB_OK);
            pError->Release();
        }
        return E_FAIL;
    }

    // テクニックのハンドル取得
    m_Tech = m_pEffect->GetTechniqueByName("StandardDraw");

    // グローバル変数ハンドル取得
    m_mtxWorld = m_pEffect->GetParameterByName(NULL, "g_mtxWorld");
    m_View = m_pEffect->GetParameterByName(NULL, "g_View");
    m_Proj = m_pEffect->GetParameterByName(NULL, "g_Proj");
    m_vecLight = m_pEffect->GetParameterByName(NULL, "g_vecLight");
    m_Diffuse = m_pEffect->GetParameterByName(NULL, "g_Deffuse");

    return S_OK;
}

//***************************************
// シェーダー適応
//***************************************
void CDefoult::Begin(void)
{
    m_pEffect->Begin(0, 0);
}

//***************************************
// パスを適応
//***************************************
void CDefoult::BeginPass(const UINT Pass)
{
    m_pEffect->BeginPass(Pass);
    // 変更をGPUに反映
    m_pEffect->CommitChanges();
}

//***************************************
// シェーダー終了
//***************************************
void CDefoult::End()
{
	m_pEffect->End();
}

//***************************************
// パスの終了
//***************************************
void CDefoult::EndPass(void)
{
    m_pEffect->EndPass();
}

//***************************************
// パラメータを設定
//***************************************
void CDefoult::SetParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj,D3DXVECTOR4 Light, D3DXVECTOR4 Col)
{
    // パラメータ(グローバル変数の設定)
    m_pEffect->SetMatrix(m_mtxWorld, &World);
    m_pEffect->SetMatrix(m_View, &View);
    m_pEffect->SetMatrix(m_Proj, &Proj);
    m_pEffect->SetVector(m_vecLight, &Light);
    m_pEffect->SetVector(m_Diffuse, &Col);
}

//***************************************
// インスタンスを取得
//***************************************
CDefoult* CDefoult::Instance(void)
{
	if (m_Instance != NULL) return m_Instance.get();
	m_Instance.reset(new CDefoult);
	return m_Instance.get();
}