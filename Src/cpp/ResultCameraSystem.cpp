//================================================================
//
// リザルトカメラの処理[ResultCameraSystem.cpp]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// インクルード
//****************************************************************
#include "System/ResultCameraSystem.h"

//****************************************************************
// 静的定数メンバ変数の初期化
//****************************************************************
// クリア時
const D3DXVECTOR3 CClearCameraSystem::Config::PosV = { 0.0f,1000.0f,-1000.0f };
const D3DXVECTOR3 CClearCameraSystem::Config::PosR = { 0.0f,1500.0f,0.0f };
// 失敗時
const D3DXVECTOR3 CFailedCameraSystem::Config::PosV = { 0.0f,200.0f,-500.0f };
const D3DXVECTOR3 CFailedCameraSystem::Config::PosR = { 0.0f,100.0f,0.0f };

//***************************************
// 初期化処理
//***************************************
void CClearCameraSystem::Init(void)
{
	m_pOwner->SetPosV(CClearCameraSystem::Config::PosV);
	m_pOwner->SetPosR(CClearCameraSystem::Config::PosR);
	m_pOwner->SetPosVDest(CClearCameraSystem::Config::PosV);
	m_pOwner->SetPosRDest(CClearCameraSystem::Config::PosR);
}

//***************************************
// 更新処理
//***************************************
void CClearCameraSystem::Update(void)
{
}

//***************************************
// 終了処理
//***************************************
void CClearCameraSystem::Uninit(void)
{
}


//***************************************
// 初期化処理
//***************************************
void CFailedCameraSystem::Init(void)
{
	m_pOwner->SetPosV(CFailedCameraSystem::Config::PosV);
	m_pOwner->SetPosR(CFailedCameraSystem::Config::PosR);
	m_pOwner->SetPosVDest(CFailedCameraSystem::Config::PosV);
	m_pOwner->SetPosRDest(CFailedCameraSystem::Config::PosR);
}

//***************************************
// 更新処理
//***************************************
void CFailedCameraSystem::Update(void)
{
}

//***************************************
// 終了処理
//***************************************
void CFailedCameraSystem::Uninit(void)
{
}