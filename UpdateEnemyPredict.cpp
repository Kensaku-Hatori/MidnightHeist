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
	auto view = reg.view<Enemy, AIComponent,EnemyStateComponent>();

	// パトロールポイントとプレイヤーのビュー
	auto PatrolManagerview = reg.view<PatrolPointManager>();
	auto Playerview = reg.view<Player>();

	// プレイヤーかパトロールポイントマネージャーが存在しなかったら切り上げ
	if (PatrolManagerview.empty() || Playerview.empty()) return;

	// コンテナにアクセス
	for (auto [Entity, AICmp,StateCmp] : view.each())
	{
		// 捜索状態以外なら切り上げ
		if (StateCmp.State != EnemyState::ENEMYSTATE::PREDICT) continue;

		// 扇情報を取得
		auto& FanInfoCmp = reg.get<FanComponent>(Entity);
		// Entityを取得
		auto PatrolManagerEneity = *PatrolManagerview.begin();
		auto PlayerEneity = *Playerview.begin();
		// コンポーネントを取得
		auto& PatrolPointCmp = reg.get<PatrolPointComponent>(PatrolManagerEneity);
		auto& PlayerTransformCmp = reg.get<Transform3D>(PlayerEneity);

		// 自分自身のコンポーネントを取得
		auto& RBCmp = reg.get<RigidBodyComponent>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		auto& VelocityCmp = reg.get<VelocityComponent>(Entity);

		// 視界内にプレイヤーがいてかつプレイヤーとの間にオブジェクトがなかったら
		if (CMath::IsPointInFan(FanInfoCmp, PlayerTransformCmp.Pos) == true &&
			AICmp.IsBlockedToPlayer == false)
		{
			CGame::AddEnCount();
			// 追いかけモード
			StateCmp.State = EnemyState::ENEMYSTATE::CHASE;
			continue;
		}

		// 剛体が生成されていたら
		if (RBCmp.Body == nullptr) continue;

		// 経路が見つかっていなかったら
		if (AICmp.AStarRoute.empty() == true)
		{
			AICmp.DestPos = PatrolPointCmp.PatrolPoint[AICmp.HalfPatrolRoute[AICmp.NextIdx].Idx].Point;
			StateCmp.State = EnemyState::ENEMYSTATE::SEARCH;
		}

		// 移動量を計算するためのベクトル
		D3DXVECTOR3 ToDestPos = AICmp.DestPos - TransformCmp.Pos;
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
			AICmp.BackIdx++;
			if (AICmp.AStarRoute.size() == 0) return;

			// 目標の位置を設定
			AICmp.BackIdx = Clamp(AICmp.BackIdx, 0, static_cast<int>(AICmp.AStarRoute.size() - 1));
			// エースターのインデックスがサイズオーバーしていてかつ探索が終了していなかったら
			if (AICmp.BackIdx >= static_cast<int>(AICmp.AStarRoute.size() - 1) && AICmp.IsFinishedAStar == false)
			{
				// 探索が終了していたら
				AICmp.IsFinishedAStar = true;
				// 目標の位置を設定
				AICmp.DestPos = AICmp.LastLookPlayerPosition;
			}
			// 探索が終了していたら
			else if (AICmp.IsFinishedAStar == true)
			{
				// ステートを変更
				StateCmp.State = EnemyState::ENEMYSTATE::BACK;
				// リセット
				AICmp.CoolDownCnt = 0;
				AICmp.BackIdx = 0;
				// 一番近くの障害物をまたがないポイントへのIdx
				int BestPoint;
				// Idxを取得
				BestPoint = CMath::NearCanMovePoint(TransformCmp.Pos, PatrolPointCmp.PatrolPoint, CMapManager::Instance()->GetvMapObject());
				if (BestPoint < 0) return;

				// 帰るまでの道筋を取得
				AICmp.AStarRoute = CMath::AStar(PatrolPointCmp.PatrolPoint, BestPoint, AICmp.HalfPatrolRoute[AICmp.NowIdx].Idx);
				// 目標の位置を設定
				AICmp.DestPos = PatrolPointCmp.PatrolPoint[AICmp.AStarRoute[AICmp.BackIdx]].Point;
				AICmp.IsFinishedAStar = false;
				continue;
			}
			else
			{
				// 目標の位置を設定
				AICmp.DestPos = PatrolPointCmp.PatrolPoint[AICmp.AStarRoute[AICmp.BackIdx]].Point;
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
	auto& VelocityCmp = Reg.get<VelocityComponent>(Entity);
	auto& CharactorCmp = Reg.get<CharactorComponent>(Entity);

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