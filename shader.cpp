//****************************************************************
//
// シェーダーの処理[shader.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "shader.h"
#include "manager.h"

//***************************************
// デストラクタ
//***************************************
CShader::~CShader()
{
    // エフェクトの破棄
    if (m_pEffect != nullptr)
    {
        m_pEffect->Release();
        m_pEffect = nullptr;
    }

    // ハンドルをクリア
    m_vHandle.clear();
}

//***************************************
// 初期化処理
//***************************************
HRESULT CShader::Init(std::string Path)
{
    // パスを保存
    m_EffectPath = Path;

    // デバイスを取得
    CRenderer* pRenderer;
    pRenderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

    // シェーダ読み込み時のエラーバッファ
    ID3DXBuffer* pError = nullptr;

    // エフェクト生成
    HRESULT hr = D3DXCreateEffectFromFile(pDevice,
        Path.c_str(),
        nullptr,
        nullptr,
        D3DXSHADER_DEBUG,
        nullptr,
        &m_pEffect,
        &pError);

    // 失敗したら
    if (FAILED(hr))
    {
        // エラーバッファの内容が存在したら
        if (pError)
        {
            // メッセージボックスに出力
            MessageBox(nullptr, (char*)pError->GetBufferPointer(), "Shader Error", MB_OK);

            // メモリの開放
            pError->Release();
        }
        // 失敗
        return E_FAIL;
    }

    // テクニックのハンドル取得
    m_Tech = m_pEffect->GetTechniqueByName("StandardDraw");

    // 正常終了
    return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CShader::Uninit(void)
{
}

//***************************************
// シェーダー適応
//***************************************
void CShader::Begin(void)
{
    m_pEffect->Begin(0, 0);
}

//***************************************
// パスを適応
//***************************************
void CShader::BeginPass(const UINT Pass)
{
    m_pEffect->BeginPass(Pass);
}

//***************************************
// シェーダー終了
//***************************************
void CShader::End(void)
{
    m_pEffect->End();
}

//***************************************
// パスの終了
//***************************************
void CShader::EndPass(void)
{
    m_pEffect->EndPass();
}

//***************************************
// リセット
//***************************************
void CShader::Reset(void)
{
    if (m_pEffect != nullptr)
    {
        m_pEffect->Release();
        m_pEffect = nullptr;
    }
}

//***************************************
// リスタート
//***************************************
void CShader::Restart(void)
{
    // デバイスを取得
    CRenderer* pRenderer;
    pRenderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

    // シェーダ読み込み時のエラーバッファ
    ID3DXBuffer* pError = nullptr;

    // エフェクト生成
    HRESULT hr = D3DXCreateEffectFromFile(pDevice,
        m_EffectPath.c_str(),
        nullptr,
        nullptr,
        D3DXSHADER_DEBUG,
        nullptr,
        &m_pEffect,
        &pError);

    // 失敗したら
    if (FAILED(hr))
    {
        // エラーバッファの内容が存在したら
        if (pError)
        {
            // メッセージボックスに出力
            MessageBox(nullptr, (char*)pError->GetBufferPointer(), "Shader Error", MB_OK);

            // メモリの開放
            pError->Release();
        }
    }

    // テクニックのハンドル取得
    m_Tech = m_pEffect->GetTechniqueByName("StandardDraw");
}

//***************************************
// ハンドルを取得
//***************************************
D3DXHANDLE& CShader::GetHandle(std::string Key)
{
    // ハンドルを取得
    return m_vHandle[Key];
}