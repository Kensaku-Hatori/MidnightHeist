//****************************************************************
//
// リザルトカメラの処理[ResultCameraSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "ResultCameraSystem.h"

// 静的定数メンバ変数の初期化
const D3DXVECTOR3 CResultCamera::Config::PosV = { 0.0f,200.0f,-500.0f };
const D3DXVECTOR3 CResultCamera::Config::PosR = { 0.0f,100.0f,0.0f };

//***************************************
// 初期化処理
//***************************************
void CResultCamera::Init(void)
{
	m_pOwner->SetPosV(CResultCamera::Config::PosV);
	m_pOwner->SetPosR(CResultCamera::Config::PosR);
	m_pOwner->SetPosVDest(CResultCamera::Config::PosV);
	m_pOwner->SetPosRDest(CResultCamera::Config::PosR);
}

//***************************************
// 更新処理
//***************************************
void CResultCamera::Update(void)
{
}

//***************************************
// 終了処理
//***************************************
void CResultCamera::Uninit(void)
{
}