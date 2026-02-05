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
#include "PlayerStateComponent.hpp"
#include "TagComp.hpp"

// 名前空間
using namespace Tag;

// 静的定数メンバ変数の初期化
const D3DXVECTOR3 CGameCamera::Config::Default::PosV = { 0.0f,1400.0f,800.0f };
const D3DXVECTOR3 CGameCamera::Config::Default::PosR = { 0.0f,0.0f,0.0f };

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
	auto Playerview = CManager::GetScene()->GetReg().view<Player>();
	auto PlayerEneity = *Playerview.begin();
	auto& PlayerAnimCmp = CManager::GetScene()->GetReg().get<PlayerAnimComponent>(PlayerEneity);
	auto& PlayerStateCmp = CManager::GetScene()->GetReg().get<PlayerStateComponent>(PlayerEneity);
	auto& PlayerTransform = CManager::GetScene()->GetReg().get<Transform3D>(PlayerEneity);

	// デフォルトの視点からプレイヤーまでのベクトル
	D3DXVECTOR3 VecPlayer = PlayerTransform.Pos - CGameCamera::Config::Default::PosV;
	// ベクトルを正規化
	D3DXVec3Normalize(&VecPlayer, &VecPlayer);
	// 視点の位置
	D3DXVECTOR3 SetPosV = m_pOwner->GetPosVDest();
	// 注視点の位置
	D3DXVECTOR3 SetPosR = m_pOwner->GetPosRDest();
	// カメラの追いつく速さ
	float Speed = CCamera::Config::Defoult::SpeedV;

	// 画面に入っていてアニメーションが終わっていなかったら
	if (PlayerAnimCmp.IsScreen == true && PlayerAnimCmp.IsFinishedAnim == false)
	{
		Speed = 0.1f;
		// 視点を近づける
		SetPosV = PlayerTransform.Pos;
		// デフォルトカメラの位置をベクトルとして扱う
		D3DXVECTOR3 DefaultPosV = CGameCamera::Config::Default::PosV;
		// 正規化
		D3DXVec3Normalize(&DefaultPosV, &DefaultPosV);
		SetPosV += DefaultPosV * CGameCamera::Config::FirstAnimation::Zoom;
		SetPosR = PlayerTransform.Pos;
	}
	if (PlayerAnimCmp.IsFinishedAnim == true)
	{
		if (PlayerStateCmp.NowState == PlayerState::State::RANGE_PICKING)
		{
			D3DXVECTOR3 VecItem = PlayerTransform.Pos - CGameCamera::Config::Default::PosV;
			VecItem *= 0.5f;
			// デフォルトカメラの位置をベクトルとして扱う
			D3DXVECTOR3 DefaultPosV = CGameCamera::Config::Default::PosV;
			// 正規化
			D3DXVec3Normalize(&DefaultPosV, &DefaultPosV);
			SetPosR = VecItem;
			// 視点を近づける
			SetPosV = SetPosR;
			SetPosV += DefaultPosV * (CGameCamera::Config::PickingZoom::Zoom * 1.5f);
		}
		else if (PlayerStateCmp.NowState == PlayerState::State::PICKING)
		{
			// 視点を近づける
			SetPosV = PlayerTransform.Pos;
			// デフォルトカメラの位置をベクトルとして扱う
			D3DXVECTOR3 DefaultPosV = CGameCamera::Config::Default::PosV;
			// 正規化
			D3DXVec3Normalize(&DefaultPosV, &DefaultPosV);
			SetPosV += DefaultPosV * CGameCamera::Config::PickingZoom::Zoom;
			SetPosR = PlayerTransform.Pos;
		}
		else
		{
			// 設定
			SetPosV = CGameCamera::Config::Default::PosV;
			SetPosR = VEC3_NULL;
		}
	}
	// 設定
	m_pOwner->SetSpeedV(Speed);
	m_pOwner->SetPosVDest(SetPosV);
	m_pOwner->SetPosRDest(SetPosR);
}

//***************************************
// 終了処理
//***************************************
void CGameCamera::Uninit(void)
{
}