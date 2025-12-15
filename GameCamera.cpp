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
#include "PlayerStateComp.hpp"
#include "TagComp.hpp"

// 名前空間
using namespace Tag;

// 静的定数メンバ変数の初期化
const D3DXVECTOR3 CGameCamera::Config::Default::PosV = { 0.0f,1400.0f,800.0f };
const D3DXVECTOR3 CGameCamera::Config::Default::PosR = { 0.0f,0.0f,0.0f };
const D3DXVECTOR3 CGameCamera::Config::Zoom::PosV = { 0.0f,1000.0f,1000.0f };
const D3DXVECTOR3 CGameCamera::Config::Zoom::PosR = { 0.0f,0.0f,0.0f };

//***************************************
// 初期化処理
//***************************************
void CGameCamera::Init(void)
{
	m_pOwner->SetPosV(CGameCamera::Config::Default::PosV);
	m_pOwner->SetPosR(CGameCamera::Config::Default::PosR);
	m_pOwner->SetPosVDest(CGameCamera::Config::Default::PosV);
	m_pOwner->SetPosRDest(CGameCamera::Config::Default::PosR);
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
	auto& PlayerStateCmp = CManager::GetScene()->GetReg().get<PlayerStateComp>(PlayerEneity);
	auto& PlayerTransform = CManager::GetScene()->GetReg().get<Transform3D>(PlayerEneity);

	// デフォルトの視点からプレイヤーまでのベクトル
	D3DXVECTOR3 VecPlayer = PlayerTransform.Pos - CGameCamera::Config::Default::PosV;
	// ベクトルを正規化
	D3DXVec3Normalize(&VecPlayer, &VecPlayer);
	// 視点の位置
	D3DXVECTOR3 SetPosV = m_pOwner->GetPosVDest();
	// 注視点の位置
	D3DXVECTOR3 SetPosR = m_pOwner->GetPosRDest();

	// 画面に入っていてアニメーションが終わっていなかったら
	if (PlayerAnimCmp.IsScreen == true && PlayerAnimCmp.IsFinishedAnim == false)
	{
		// 視点を近づける
		SetPosV = CGameCamera::Config::Default::PosV + VecPlayer * 700.0f;
		SetPosR = PlayerTransform.Pos;
	}
	// アニメーションが終わっていたら
	if (PlayerAnimCmp.IsFinishedAnim == true && PlayerStateCmp.NowState != PlayerState::State::PICKING)
	{
		// 設定
		SetPosV = CGameCamera::Config::Default::PosV;
		SetPosR = VEC3_NULL;
	}
	// 開錠されていたら
	else if(PlayerStateCmp.NowState == PlayerState::State::PICKING)
	{
		// 視点を近づける
		SetPosV = CGameCamera::Config::Default::PosV + VecPlayer * 1000.0f;
		SetPosR = PlayerTransform.Pos;
	}
	// 設定
	m_pOwner->SetPosVDest(SetPosV);
	m_pOwner->SetPosRDest(SetPosR);
}

//***************************************
// 終了処理
//***************************************
void CGameCamera::Uninit(void)
{
}