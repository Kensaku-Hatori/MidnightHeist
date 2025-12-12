//****************************************************************
//
// タイトルカメラの処理[GameCamera.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "GameCamera.h"
#include "manager.h"
#include "scene.h"
#include "TransformComponent.hpp"
#include "PlayerAnimetionComponent.hpp"
#include "RigitBodyComponent.hpp"
#include "TagComp.hpp"

// 名前空間
using namespace Tag;

// 静的定数メンバ変数の初期化
const D3DXVECTOR3 CGameCamera::Config::PosV = { 0.0f,900.0f,500.0f };
const D3DXVECTOR3 CGameCamera::Config::PosR = { 0.0f,0.0f,0.0f };

//***************************************
// 初期化処理
//***************************************
void CGameCamera::Init(void)
{
	m_pOwner->SetPosV(CGameCamera::Config::PosV);
	m_pOwner->SetPosR(CGameCamera::Config::PosR);
	m_pOwner->SetPosVDest(CGameCamera::Config::PosV);
	m_pOwner->SetPosRDest(CGameCamera::Config::PosR);
}

//***************************************
// 更新処理
//***************************************
void CGameCamera::Update(void)
{
	// プレイヤーのコンポーネントを取得
	auto Playerview = CManager::GetScene()->GetReg().view<PlayerComponent>();
	auto PlayerEneity = *Playerview.begin();
	auto& PlayerAnimCmp = CManager::GetScene()->GetReg().get<PlayerAnimComp>(PlayerEneity);

	// 画面に入っていてアニメーションが終わっていなかったら
	if (PlayerAnimCmp.IsScreen == true && PlayerAnimCmp.IsFinishedAnim == false)
	{
		auto& PlayerTransform = CManager::GetScene()->GetReg().get<Transform3D>(PlayerEneity);
		D3DXVECTOR3 PosV = { PlayerTransform.Pos.x,600.0f,600.0f };
		m_pOwner->SetPosVDest(PosV);
		m_pOwner->SetPosRDest(PlayerTransform.Pos);
	}
	// アニメーションが終わっていたら
	if (PlayerAnimCmp.IsFinishedAnim == true)
	{
		D3DXVECTOR3 PosV = { 0.0f,1400.0f,800.0f };
		CManager::GetCamera()->SetPosVDest(PosV);
		CManager::GetCamera()->SetPosRDest(VEC3_NULL);
	}
}

//***************************************
// 終了処理
//***************************************
void CGameCamera::Uninit(void)
{
}