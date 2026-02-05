//****************************************************************
//
// 敵の追跡更新処理[UpdateEnemySearchSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateEnemyPredict.h"
#include "EnemyAIUtils.hpp"

//*********************************************
// 更新
//*********************************************
void UpdateEnemyPredictSystem::Update(entt::registry& reg)
{
	// 敵のビュー
	auto view = reg.view<EnemyComponent, EnemyAIComp>();

	// パトロールポイントとプレイヤーのビュー
	auto PatrolManagerview = reg.view<PatrolPointManager>();
	auto Playerview = reg.view<PlayerComponent>();

	// プレイヤーかパトロールポイントマネージャーが存在しなかったら切り上げ
	if (PatrolManagerview.empty() || Playerview.empty()) return;

	// コンテナにアクセス
	for (auto [Entity, State] : view.each())
	{
		// 捜索状態以外なら切り上げ
		if (State.State != EnemyState::ENEMYSTATE::PREDICT) continue;

		// 扇情報を取得
		auto& FanInfoCmp = reg.get<FanComp>(Entity);
		// Entityを取得
		auto PatrolManagerEneity = *PatrolManagerview.begin();
		auto PlayerEneity = *Playerview.begin();
		// コンポーネントを取得
		auto& PatrolPointCmp = reg.get<PatrolPointComp>(PatrolManagerEneity);
		auto& PlayerTransformCmp = reg.get<Transform3D>(PlayerEneity);

		// 自分自身のコンポーネントを取得
		auto& RBCmp = reg.get<RigidBodyComponent>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		auto& VelocityCmp = reg.get<VelocityComp>(Entity);

		// 視界内にプレイヤーがいてかつプレイヤーとの間にオブジェクトがなかったら
		if (CMath::IsPointInFan(FanInfoCmp, PlayerTransformCmp.Pos) == true &&
			State.IsBlockedToPlayer == false)
		{
			CGame::AddEnCount();
			// 追いかけモード
			State.State = EnemyState::ENEMYSTATE::CHASE;
			continue;
		}

		// 剛体が生成されていたら
		if (RBCmp.Body == nullptr) continue;

		// 経路が見つかっていなかったら
		if (State.AStarRoute.empty() == true)
		{
			State.DestPos = PatrolPointCmp.PatrolPoint[State.HalfPatrolRoute[State.NextIdx].Idx].Point;
			State.State = EnemyState::ENEMYSTATE::SEARCH;
		}

		// 移動量を計算するためのベクトル
		D3DXVECTOR3 ToDestPos = State.DestPos - TransformCmp.Pos;
		// Y成分を消す
		ToDestPos.y = 0.0f;
		// ベクトルを正規化する用の変数
		D3DXVECTOR3 Normalize;
		// 正規化
		D3DXVec3Normalize(&Normalize, &ToDestPos);

		// 目的地に着いたら
		if (D3DXVec3Length(&ToDestPos) < PatrolPointCmp.PointRadius)
		{
			// 今の位置を目標の位置にする
			State.BackIdx++;
			if (State.AStarRoute.size() == 0) return;

			// 目標の位置を設定
			State.BackIdx = Clamp(State.BackIdx, 0, static_cast<int>(State.AStarRoute.size() - 1));
			// エースターのインデックスがサイズオーバーしていてかつ探索が終了していなかったら
			if (State.BackIdx >= static_cast<int>(State.AStarRoute.size() - 1) && State.IsFinishedAStar == false)
			{
				// 探索が終了していたら
				State.IsFinishedAStar = true;
				// 目標の位置を設定
				State.DestPos = State.LastLookPlayerPosition;
			}
			// 探索が終了していたら
			else if (State.IsFinishedAStar == true)
			{
				// ステートを変更
				State.State = EnemyState::ENEMYSTATE::BACK;
				// リセット
				State.CoolDownCnt = 0;
				State.BackIdx = 0;
				// 一番近くの障害物をまたがないポイントへのIdx
				int BestPoint;
				// Idxを取得
				BestPoint = CMath::NearCanMovePoint(TransformCmp.Pos, PatrolPointCmp.PatrolPoint, CMapManager::Instance()->GetvMapObject());
				if (BestPoint < 0) return;

				// 帰るまでの道筋を取得
				State.AStarRoute = CMath::AStar(PatrolPointCmp.PatrolPoint, BestPoint, State.HalfPatrolRoute[State.NowIdx].Idx);
				// 目標の位置を設定
				State.DestPos = PatrolPointCmp.PatrolPoint[State.AStarRoute[State.BackIdx]].Point;
				State.IsFinishedAStar = false;
				continue;
			}
			else
			{
				// 目標の位置を設定
				State.DestPos = PatrolPointCmp.PatrolPoint[State.AStarRoute[State.BackIdx]].Point;
			}
		}
		else
		{
			// 移動方向を代入
			VelocityCmp.Velocity = Normalize;
			// 速さを掛ける
			VelocityCmp.Velocity *= 8;
			// Y成分を取り除く
			VelocityCmp.Velocity.y = RBCmp.Body->getLinearVelocity().y();
		}
		// 移動
		UpdateMove(reg, Entity);
	}
}

//*********************************************
// 移動の更新
//*********************************************
void UpdateEnemyPredictSystem::UpdateMove(entt::registry& Reg, entt::entity& Entity)
{
	// 自分自身のコンポーネントを取得
	auto& RBCmp = Reg.get<RigidBodyComponent>(Entity);
	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& VelocityCmp = Reg.get<VelocityComp>(Entity);
	auto& CharactorCmp = Reg.get<CharactorComp>(Entity);

	// 設定
	RBCmp.Body->setLinearVelocity(CMath::SetVec(VelocityCmp.Velocity));;

	// 移動量がなかったら
	if (VelocityCmp.Velocity == VEC3_NULL) return;

	// クォータニオンを求める
	// 真上ベクトル
	D3DXVECTOR3 VecUp = VEC_UP;
	// 設定用変数
	D3DXQUATERNION SetQuat;
	// 移動値を方向ベクトルに変換
	D3DXVec3Normalize(&VelocityCmp.Velocity, &VelocityCmp.Velocity);

	// 方向ベクトルのZX平面上での角度を求める
	float angle = atan2f(VelocityCmp.Velocity.x, VelocityCmp.Velocity.z);
	angle += D3DX_PI;

	// クォータニオンを計算
	D3DXQuaternionRotationAxis(&SetQuat, &VecUp, angle);
	// 代入
	CharactorCmp.QuatDest = SetQuat;
}