//****************************************************************
//
// タイトルカメラの処理[TitleCamera.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "TitleCamera.h"

// 静的定数メンバ変数の初期化
const D3DXVECTOR3 CTitleCamera::Config::PosV = { -430.75f,79.0f,1025.75f };
const D3DXVECTOR3 CTitleCamera::Config::PosR = { 1800.0f,700.0f,-1000.0f };

//***************************************
// 初期化処理
//***************************************
void CTitleCamera::Init(void)
{
	m_pOwner->SetPosV(CTitleCamera::Config::PosV);
	m_pOwner->SetPosR(CTitleCamera::Config::PosR);
	m_pOwner->SetPosVDest(CTitleCamera::Config::PosV);
	m_pOwner->SetPosRDest(CTitleCamera::Config::PosR);
}

//***************************************
// 更新処理
//***************************************
void CTitleCamera::Update(void)
{
}

//***************************************
// 終了処理
//***************************************
void CTitleCamera::Uninit(void)
{
}