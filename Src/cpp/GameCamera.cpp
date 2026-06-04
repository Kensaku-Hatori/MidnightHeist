//****************************************************************
//
// タイトルカメラの処理[GameCamera.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "System/GameCamera.h"
#include "Bace/manager.h"
#include "Scene/scene.h"
#include "Component/TransformComponent.hpp"
#include "Component/Player/PlayerAnimetionComponent.hpp"
#include "Component/Player/PlayerStateComponent.hpp"
#include "TagComp.hpp"

// 名前空間
using namespace Tag;

// 静的定数メンバ変数の初期化
const D3DXVECTOR3 CGameCameraSystem::Config::Default::PosV = { 0.0f,1400.0f,800.0f };
const D3DXVECTOR3 CGameCameraSystem::Config::Default::PosR = { 0.0f,0.0f,0.0f };

//***************************************
// 初期化処理
//***************************************
void CGameCameraSystem::Init(void)
{
	m_pOwner->SetPosV(CGameCameraSystem::Config::Default::PosV);
	m_pOwner->SetPosR(CGameCameraSystem::Config::Default::PosR);
	m_pOwner->SetPosVDest(CGameCameraSystem::Config::Default::PosV);
	m_pOwner->SetPosRDest(CGameCameraSystem::Config::Default::PosR);
}

//***************************************
// 更新処理
//***************************************
void CGameCameraSystem::Update(void)
{
	// プレイヤーのコンポーネントを取得
	auto Playerview = CManager::GetScene()->GetReg().view<Player>();
	auto PlayerEneity = *Playerview.begin();
	auto& PlayerAnimCmp = CManager::GetScene()->GetReg().get<PlayerAnimComponent>(PlayerEneity);
	auto& PlayerStateCmp = CManager::GetScene()->GetReg().get<PlayerStateComponent>(PlayerEneity);
	auto& PlayerTransform = CManager::GetScene()->GetReg().get<Transform3D>(PlayerEneity);

	// デフォルトの視点からプレイヤーまでのベクトル
	D3DXVECTOR3 VecPlayer = PlayerTransform.Pos - CGameCameraSystem::Config::Default::PosV;
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
		D3DXVECTOR3 DefaultPosV = CGameCameraSystem::Config::Default::PosV;
		// 正規化
		D3DXVec3Normalize(&DefaultPosV, &DefaultPosV);
		SetPosV += DefaultPosV * CGameCameraSystem::Config::FirstAnimation::Zoom;
		SetPosR = PlayerTransform.Pos;
	}
	if (PlayerAnimCmp.IsFinishedAnim == true)
	{
		if (PlayerStateCmp.NowState == PlayerState::State::RANGE_PICKING)
		{
			D3DXVECTOR3 VecItem = PlayerTransform.Pos - CGameCameraSystem::Config::Default::PosV;
			VecItem *= 0.5f;
			// デフォルトカメラの位置をベクトルとして扱う
			D3DXVECTOR3 DefaultPosV = CGameCameraSystem::Config::Default::PosV;
			// 正規化
			D3DXVec3Normalize(&DefaultPosV, &DefaultPosV);
			SetPosR = VecItem;
			// 視点を近づける
			SetPosV = SetPosR;
			SetPosV += DefaultPosV * (CGameCameraSystem::Config::PickingZoom::Zoom * 1.5f);
		}
		else if (PlayerStateCmp.NowState == PlayerState::State::PICKING)
		{
			// 視点を近づける
			SetPosV = PlayerTransform.Pos;
			// デフォルトカメラの位置をベクトルとして扱う
			D3DXVECTOR3 DefaultPosV = CGameCameraSystem::Config::Default::PosV;
			// 正規化
			D3DXVec3Normalize(&DefaultPosV, &DefaultPosV);
			SetPosV += DefaultPosV * CGameCameraSystem::Config::PickingZoom::Zoom;
			SetPosR = PlayerTransform.Pos;
		}
		else
		{
			// 設定
			SetPosV = CGameCameraSystem::Config::Default::PosV;
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
void CGameCameraSystem::Uninit(void)
{
}