//****************************************************************
//
// タイトルロゴの処理[titlelogo.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "titlelogo.h"
#include "titlemanager.h"
#include "math_T.h"
#include "texmanager.h"

//***************************************
// コンストラクタ
//***************************************
CTitleLogo::CTitleLogo() : CObject2D(5)
{
}

//***************************************
// デストラクタ
//***************************************
CTitleLogo::~CTitleLogo()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CTitleLogo::Init(void)
{
    CObject2D::Init();
    return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CTitleLogo::Uninit(void)
{
    CObject2D::Uninit();
}

//***************************************
// 更新処理
//***************************************
void CTitleLogo::Update(void)
{
    // カウンターを進める
    if (m_nFrameCounter < CTitleManager::Config::Logo::AnimFrame)
    {
        m_nFrameCounter++;
    }
    // 設定する位置の変数
    D3DXVECTOR3 SetPos = {};
    // 今のアニメーションの進行割合を計算
    float Ratio = EaseOutBounce((float)m_nFrameCounter / CTitleManager::Config::Logo::AnimFrame);
    // 最終地点から初期位置からの差分
    D3DXVECTOR3 Diff  = CTitleManager::Config::Logo::Dest - CTitleManager::Config::Logo::Apper;
    // 今の位置を計算
    SetPos = CTitleManager::Config::Logo::Apper + Diff * Ratio;
    // 設定
    CObject2D::SetPosition(SetPos);
    CObject2D::Update();
}

//***************************************
// 描画処理
//***************************************
void CTitleLogo::Draw(void)
{
    CObject2D::Draw();
}

//***************************************
// 一番下まで降りていなかったら下す
//***************************************
void CTitleLogo::Lower(void)
{
    if (m_nFrameCounter < CTitleManager::Config::Logo::AnimFrame)
    {
        m_nFrameCounter = CTitleManager::Config::Logo::AnimFrame;
    }
}

//***************************************
// 一番下まで降りているかどうか
//***************************************
bool CTitleLogo::GetLower(void)
{
    return m_nFrameCounter >= CTitleManager::Config::Logo::AnimFrame;
}

//***************************************
// 生成処理
//***************************************
CTitleLogo* CTitleLogo::Create(const D3DXVECTOR3 Pos, const D3DXVECTOR3 Rot, const D3DXVECTOR2 Size, const char* FilePath)
{
    // ポインタを宣言
    CTitleLogo* pTitleLogo = NULL;
    // メモリ確保
    pTitleLogo = new CTitleLogo;
    // 各種処理
    pTitleLogo->Init();
    pTitleLogo->SetPosition(Pos);
    //pTitleLogo->SetRotasion(Rot);
    pTitleLogo->SetSize(Size);
    pTitleLogo->SetCol(WHITE);
    // 返す
    return pTitleLogo;
}