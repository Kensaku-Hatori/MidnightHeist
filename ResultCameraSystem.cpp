//****************************************************************
//
// リザルトカメラの処理[ResultCameraSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "ResultCameraSystem.h"

// 静的定数メンバ変数の初期化
// クリア時
const D3DXVECTOR3 CClearCamera::Config::PosV = { 0.0f,1000.0f,-1000.0f };
const D3DXVECTOR3 CClearCamera::Config::PosR = { 0.0f,1500.0f,0.0f };
// 失敗時
const D3DXVECTOR3 CFailedCamera::Config::PosV = { 0.0f,200.0f,-500.0f };
const D3DXVECTOR3 CFailedCamera::Config::PosR = { 0.0f,100.0f,0.0f };

//***************************************
// 初期化処理
//***************************************
void CClearCamera::Init(void)
{
	m_pOwner->SetPosV(CClearCamera::Config::PosV);
	m_pOwner->SetPosR(CClearCamera::Config::PosR);
	m_pOwner->SetPosVDest(CClearCamera::Config::PosV);
	m_pOwner->SetPosRDest(CClearCamera::Config::PosR);
}

//***************************************
// 更新処理
//***************************************
void CClearCamera::Update(void)
{
}

//***************************************
// 終了処理
//***************************************
void CClearCamera::Uninit(void)
{
}


//***************************************
// 初期化処理
//***************************************
void CFailedCamera::Init(void)
{
	m_pOwner->SetPosV(CFailedCamera::Config::PosV);
	m_pOwner->SetPosR(CFailedCamera::Config::PosR);
	m_pOwner->SetPosVDest(CFailedCamera::Config::PosV);
	m_pOwner->SetPosRDest(CFailedCamera::Config::PosR);
}

//***************************************
// 更新処理
//***************************************
void CFailedCamera::Update(void)
{
}

//***************************************
// 終了処理
//***************************************
void CFailedCamera::Uninit(void)
{
}