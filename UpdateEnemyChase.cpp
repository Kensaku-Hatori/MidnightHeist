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
	auto view = reg.view<EnemyComponent, EnemyAIComp>();

	// プレイヤーのビューを取得
	auto PlayerView = reg.view<PlayerComponent>();
	auto PatrolManagerview = reg.view<PatrolPointManager>();

	// プレイヤーが存在しないまたはパトロールポイントマネージャーが存在しなかったらスキップ
	if (PlayerView.empty() || PatrolManagerview.empty()) return;

	// Entityを取得
	auto PatrolManagerEneity = *PatrolManagerview.begin();
	auto playerEntity = *PlayerView.begin();

	// コンテナにアクセス
	for (auto [Entity,State] : view.each())
	{
		// 敵がCHASE以外の状態ならスキップ
		if (State.State != EnemyState::ENEMYSTATE::CHASE) continue;

		// プレイヤーとパトロールポイントマネージャーのコンポーネントを取得
		auto& PatrolPointCmp = reg.get<PatrolPointComp>(PatrolManagerEneity);
		auto& PlayerTransCmp = reg.get<Transform3D>(playerEntity);

		// 自分のコンポーネントを取得
		auto& TransformCmp = reg.get<Transform3D>(Entity);

		// プレイヤーとの間にオブジェクトがあったら
		if (State.IsBlockedToPlayer == true)
		{
			// 捜索ステートにする
			State.State = EnemyState::ENEMYSTATE::PREDICT;
			// 最後に見たプレイヤーの位置を保存
			State.LastLookPlayerPosition = PlayerTransCmp.Pos;
			// 一番近くの障害物をまたがないポイントへのIdx
			int BestPoint;
			// Idxを取得
			BestPoint = CMath::NearCanMovePoint(TransformCmp.Pos, PatrolPointCmp.PatrolPoint, CMapManager::Instance()->GetvMapObject());
			// 見失った位置のIdx
			int MissPoint;
			// Idxを取得
			MissPoint = CMath::NearCanMovePoint(State.LastLookPlayerPosition, PatrolPointCmp.PatrolPoint, CMapManager::Instance()->GetvMapObject());
			// 帰るまでの道筋を取得
			State.AStarRoute = CMath::AStar(PatrolPointCmp.PatrolPoint, BestPoint, MissPoint);
			if (State.AStarRoute.empty() == true)
			{
				// 捜索ステートにする
				State.State = EnemyState::ENEMYSTATE::SEARCH;
				continue;
			}
			// 目標の位置を設定
			State.BackIdx = Clamp(State.BackIdx, 0, static_cast<int>(State.AStarRoute.size() - 1));
			State.DestPos = PatrolPointCmp.PatrolPoint[State.AStarRoute[State.BackIdx]].Point;
			State.IsFinishedAStar = false;
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
	auto& RBCmp = Reg.get<RigitBodyComp>(Entity);
	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& VelocityCmp = Reg.get<VelocityComp>(Entity);
	auto& CharactorCmp = Reg.get<CharactorComp>(Entity);
	// プレイヤーのコンポーネントを取得
	auto& PlayerTransCmp = Reg.get<Transform3D>(PlayerEntity);

	if (RBCmp.RigitBody != nullptr)
	{
		D3DXVECTOR3 ToPlayer = PlayerTransCmp.Pos - TransformCmp.Pos;
		D3DXVec3Normalize(&ToPlayer, &ToPlayer);
		VelocityCmp.Velocity = { ToPlayer.x, ToPlayer.y, ToPlayer.z };
		VelocityCmp.Velocity.y = RBCmp.RigitBody->getLinearVelocity().y();
		// 設定
		RBCmp.RigitBody->setLinearVelocity(CMath::SetVec(VelocityCmp.Velocity * 9));

		// トランスフォームを取得
		btTransform trans;
		RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

		// 描画モデルの位置
		btVector3 newPos;

		// 位置を取得
		newPos = trans.getOrigin();

		D3DXVECTOR3 VecUp = VEC_UP;
		// 移動値を方向ベクトルに変換
		D3DXVec3Normalize(&VelocityCmp.Velocity, &VelocityCmp.Velocity);

		// 方向ベクトルのZX平面上での角度を求める
		float angle = atan2f(VelocityCmp.Velocity.x, VelocityCmp.Velocity.z);
		angle += D3DX_PI;

		// 目標のクォータニオンとして設定
		D3DXQuaternionRotationAxis(&CharactorCmp.QuatDest, &VecUp, angle);

		// 位置を計算、設定
		TransformCmp.Pos = (D3DXVECTOR3(newPos.x(), newPos.y() - 20.0f, newPos.z()));
	}
}