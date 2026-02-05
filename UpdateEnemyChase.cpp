//****************************************************************
//
// 敵の追跡更新処理[UpdateEnemyChaseSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateEnemyChase.h"
#include "EnemyAIUtils.hpp"

//*********************************************
// 更新
//*********************************************
void UpdateEnemyChaseSystem::Update(entt::registry& reg)
{
	// 敵のビュー
	auto view = reg.view<Enemy, AIComponent,EnemyStateComponent>();

	// プレイヤーのビューを取得
	auto PlayerView = reg.view<Player>();
	auto PatrolManagerview = reg.view<PatrolPointManager>();

	// プレイヤーが存在しないまたはパトロールポイントマネージャーが存在しなかったらスキップ
	if (PlayerView.empty() || PatrolManagerview.empty()) return;

	// Entityを取得
	auto PatrolManagerEneity = *PatrolManagerview.begin();
	auto playerEntity = *PlayerView.begin();

	// コンテナにアクセス
	for (auto [Entity,AICmp,StateCmp] : view.each())
	{
		// 敵がCHASE以外の状態ならスキップ
		if (StateCmp.State != EnemyState::ENEMYSTATE::CHASE) continue;

		// プレイヤーとパトロールポイントマネージャーのコンポーネントを取得
		auto& PatrolPointCmp = reg.get<PatrolPointComponent>(PatrolManagerEneity);
		auto& PlayerTransCmp = reg.get<Transform3D>(playerEntity);

		// 自分のコンポーネントを取得
		auto& TransformCmp = reg.get<Transform3D>(Entity);

		// プレイヤーとの間にオブジェクトがあったら
		if (AICmp.IsOldBlockedToPlayer == false && AICmp.IsBlockedToPlayer == true)
		{
			// 捜索ステートにする
			StateCmp.State = EnemyState::ENEMYSTATE::PREDICT;
			// 最後に見たプレイヤーの位置を保存
			AICmp.LastLookPlayerPosition = PlayerTransCmp.Pos;
			// 一番近くの障害物をまたがないポイントへのIdx
			int BestPoint;
			// Idxを取得
			BestPoint = CMath::NearCanMovePoint(TransformCmp.Pos, PatrolPointCmp.PatrolPoint, CMapManager::Instance()->GetvMapObject());
			// 見失った位置のIdx
			int MissPoint;
			// Idxを取得
			MissPoint = CMath::NearCanMovePoint(AICmp.LastLookPlayerPosition, PatrolPointCmp.PatrolPoint, CMapManager::Instance()->GetvMapObject());
			if (MissPoint < 0) return;

			// 帰るまでの道筋を取得
			AICmp.AStarRoute = CMath::AStar(PatrolPointCmp.PatrolPoint, BestPoint, MissPoint);
			// 経路探索が出来なかったら
			if (AICmp.AStarRoute.empty() == true)
			{
				// 捜索ステートにする
				StateCmp.State = EnemyState::ENEMYSTATE::SEARCH;
				continue;
			}
			// 目標の位置を設定
			AICmp.BackIdx = Clamp(AICmp.BackIdx, 0, static_cast<int>(AICmp.AStarRoute.size() - 1));
			AICmp.DestPos = PatrolPointCmp.PatrolPoint[AICmp.AStarRoute[AICmp.BackIdx]].Point;
			AICmp.IsFinishedAStar = false;
			// ステートを切り替えたので切り上げ
			continue;
		}
		UpdateMove(reg, Entity, playerEntity);
	}
}

//*********************************************
// 移動の更新
//*********************************************
void UpdateEnemyChaseSystem::UpdateMove(entt::registry& Reg, entt::entity& Entity, entt::entity& PlayerEntity)
{
	// 自分のコンポーネントを取得
	auto& RBCmp = Reg.get<RigidBodyComponent>(Entity);
	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& VelocityCmp = Reg.get<VelocityComponent>(Entity);
	auto& CharactorCmp = Reg.get<CharactorComponent>(Entity);
	// プレイヤーのコンポーネントを取得
	auto& PlayerTransCmp = Reg.get<Transform3D>(PlayerEntity);

	if (RBCmp.Body != nullptr)
	{
		D3DXVECTOR3 ToPlayer = PlayerTransCmp.Pos - TransformCmp.Pos;
		D3DXVec3Normalize(&ToPlayer, &ToPlayer);
		VelocityCmp.Velocity = { ToPlayer.x, ToPlayer.y, ToPlayer.z };
		VelocityCmp.Velocity.y = RBCmp.Body->getLinearVelocity().y();
		// 設定
		RBCmp.Body->setLinearVelocity(CMath::SetVec(VelocityCmp.Velocity * 9));

		D3DXVECTOR3 VecUp = VEC_UP;
		// 移動値を方向ベクトルに変換
		D3DXVec3Normalize(&VelocityCmp.Velocity, &VelocityCmp.Velocity);

		// 方向ベクトルのZX平面上での角度を求める
		float angle = atan2f(VelocityCmp.Velocity.x, VelocityCmp.Velocity.z);
		angle += D3DX_PI;

		// 目標のクォータニオンとして設定
		D3DXQuaternionRotationAxis(&CharactorCmp.QuatDest, &VecUp, angle);
	}
}