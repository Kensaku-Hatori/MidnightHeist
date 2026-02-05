//****************************************************************
//
// 敵の更新処理[UpdateEnemySystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateEnemySystem.h"
#include "EnemyAIUtils.hpp"
#include "ColorComponent.hpp"

//*********************************************
// 更新
//*********************************************
void UpdateEnemySystem::Update(entt::registry& reg)
{
	// ビュー取得
	auto view = reg.view<Enemy>();

	// アクセス
	for (auto entity : view)
	{
		// 情報を取得
		auto& Transform = reg.get<Transform3D>(entity);
		auto& FanInfoCmp = reg.get<FanComponent>(entity);
		auto& ChildrenCmp = reg.get<ChildrenComponent>(entity);
		auto& EnemyAiCmp = reg.get<AIComponent>(entity);
		auto& SoundCmp = reg.get<EnemyListenerComponent>(entity);
		auto& StateCmp = reg.get<EnemyStateComponent>(entity);
		auto& ChractorCmp = reg.get<CharactorComponent>(entity);
		auto& SightColorCmp = reg.get<ColorComponent>(ChildrenCmp.Children[1]);
		
		switch (StateCmp.State)
		{
		case EnemyState::ENEMYSTATE::SEARCH:
			SightColorCmp.Col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			break;
		default:
			SightColorCmp.Col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		}
		// プレイヤーが発する音
		SoundCmp.ListenerVolume = EnemyListenerConfig::Bace * EnemyListenerConfig::Scale[static_cast<int>(StateCmp.State)];

		// マトリックスを取得
		D3DXMATRIX myMatrix = Transform.mtxWorld;
		// 扇の位置と向きを設定
		FanInfoCmp.Origin = { myMatrix._41,myMatrix._42,myMatrix._43 };
		FanInfoCmp.Dir = { -myMatrix._31,-myMatrix._32,-myMatrix._33 };

		// エミッターの情報を設定
		EnemyAiCmp.Emitter->SetPos(Transform.Pos);
		EnemyAiCmp.Emitter->SetFront({ -myMatrix._31,-myMatrix._32,-myMatrix._33 });
		EnemyAiCmp.Emitter->Update();

		// 向いている方向を計算
		D3DXVECTOR3 VecUp = VEC_UP;
		D3DXVECTOR3 Dir;
		D3DXQUATERNION SetQuat;
		// 移動値を方向ベクトルに変換
		D3DXVec3Normalize(&Dir, &FanInfoCmp.Dir);

		// 方向ベクトルのZX平面上での角度を求める
		float angle = atan2f(Dir.x, Dir.z);
		angle += D3DX_PI;

		D3DXQuaternionRotationAxis(&SetQuat, &VecUp, angle);

		// 自身が立てている音の大きさを子往診
		auto& SineCurveCmp = reg.get<VisibleSineCurveComponent>(ChildrenCmp.Children[2]);

		SineCurveCmp.Radius = SoundCmp.ListenerVolume;

		// 目標の位置まで補完
		D3DXQuaternionSlerp(&Transform.Quat, &Transform.Quat, &ChractorCmp.QuatDest, ChractorCmp.QuatSpeed);

		// 光線たちの更新
		UpdateRays(reg, entity);
	}
}

//*********************************************
// 光線の更新
//*********************************************
void UpdateEnemySystem::UpdateRays(entt::registry& Reg, entt::entity& Entity)
{
	UpdateToPlayerRay(Reg, Entity);
}

//*********************************************
// プレイヤーまでの光線の更新
//*********************************************
void UpdateEnemySystem::UpdateToPlayerRay(entt::registry& Reg, entt::entity& Entity)
{
	// プレイヤーのビュー
	auto Playerview = Reg.view<Player, InGame>();
	// プレイヤーが存在しなかったら切り上げ
	if (Playerview.size_hint() == 0) return;
	// プレイヤーのエンティティ
	auto PlayerEntity = *Playerview.begin();
	// 自分自身のコンポーネントを取得
	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& State = Reg.get<AIComponent>(Entity);
	// プレイヤーのトランスフォームを取得
	auto& PlayerTransformCmp = Reg.get<Transform3D>(PlayerEntity);
	D3DXVECTOR3 MyPos, PlayerPos;
	MyPos = D3DXVECTOR3(TransformCmp.mtxWorld._41, 0.0f, TransformCmp.mtxWorld._43);
	PlayerPos = D3DXVECTOR3(PlayerTransformCmp.mtxWorld._41, 0.0f, PlayerTransformCmp.mtxWorld._43);

	D3DXVECTOR3 Vec = PlayerPos - MyPos;

	// 正規化されたプレイヤーまでのベクトル
	D3DXVECTOR3 VecNormal;
	D3DXVec3Normalize(&VecNormal, &Vec);
	// プレイヤーまでの光線の情報
	RayComponent ToPlayerRay;
	ToPlayerRay.Origin = MyPos;
	ToPlayerRay.Dir = VecNormal;
	// フラグを保存
	State.IsOldBlockedToPlayer = State.IsBlockedToPlayer;
	// フラグを更新
	State.IsBlockedToPlayer = CMath::IsBlockedRayToMeshes(CMapManager::Instance()->GetvMapObject(), ToPlayerRay, CMath::CalcDistance(MyPos, PlayerPos));
}