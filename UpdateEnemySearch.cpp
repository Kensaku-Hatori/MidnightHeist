//****************************************************************
//
// 敵の追跡更新処理[UpdateEnemySearchSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateEnemySearch.h"
#include "TagComp.hpp"
#include "EnemyAIComponent.hpp"
#include "TransformComponent.hpp"
#include "RigitBodyComponent.hpp"
#include "Velocity.hpp"
#include "EnemyPatrolPointComp.hpp"
#include "math.h"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateEnemySearchSystem::Update(entt::registry& reg)
{
	auto view = reg.view<EnemyComponent, EnemtAIComp>();
	for (auto [Entity, State] : view.each())
	{
		if (State.State != EnemyState::ENEMYSTATE::SEARCH) continue;
		auto PatrolManagerview = reg.view<PatrolPointManager>();
		// プレイヤーが存在したら
		if (!PatrolManagerview.empty())
		{
			// Entityを取得
			auto PatrolManagerEneity = *PatrolManagerview.begin();
			// コンポーネントを取得
			auto& PatrolPointCmp = reg.get<PatrolPointComp>(PatrolManagerEneity);
			auto& RBCmp = reg.get<RigitBodyComp>(Entity);
			auto& TransformCmp = reg.get<Transform3D>(Entity);
			auto& VelocityCmp = reg.get<VelocityComp>(Entity);

			if (RBCmp.RigitBody != nullptr)
			{
				D3DXVECTOR3 ToDestPos = PatrolPointCmp.PatrolPoint[State.DestPatrolPoint].Point - TransformCmp.Pos;
				D3DXVECTOR3 Normalize;
				D3DXVec3Normalize(&Normalize, &ToDestPos);
				VelocityCmp.Velocity = { Normalize.x, Normalize.y, Normalize.z };
				VelocityCmp.Velocity *= 10;
				VelocityCmp.Velocity.y = RBCmp.RigitBody->getLinearVelocity().y();

				if (D3DXVec3Length(&ToDestPos) < 10.0f)
				{
					State.IsFinish = true;
					int OldPoint = State.NowPatrolPoint;
					State.NowPatrolPoint = State.DestPatrolPoint;
					int NextPoint = rand() % PatrolPointCmp.PatrolPoint[State.NowPatrolPoint].CanMove.size() + 0;
					for (int nCnt = 0; nCnt < 100; nCnt++)
					{
						if (PatrolPointCmp.PatrolPoint[State.NowPatrolPoint].CanMove[NextPoint] == OldPoint)NextPoint = rand() % PatrolPointCmp.PatrolPoint[State.NowPatrolPoint].CanMove.size() + 0;
						if (PatrolPointCmp.PatrolPoint[State.NowPatrolPoint].CanMove[NextPoint] != OldPoint) break;
					}
					State.DestPatrolPoint = PatrolPointCmp.PatrolPoint[State.NowPatrolPoint].CanMove[NextPoint];
				}
				// 設定
				RBCmp.RigitBody->setLinearVelocity(CMath::SetVec(VelocityCmp.Velocity));;

				// トランスフォームを取得
				btTransform trans;
				RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

				// 描画モデルの位置
				btVector3 newPos;

				// 位置を取得
				newPos = trans.getOrigin();

				D3DXVECTOR3 Axis = VelocityCmp.Velocity;
				D3DXVECTOR3 VecFront = { 0.0f,0.0f,-1.0f };
				D3DXVECTOR3 VecUp = VEC_UP;
				// 移動値を方向ベクトルに変換
				D3DXVec3Normalize(&VelocityCmp.Velocity, &VelocityCmp.Velocity);

				// 方向ベクトルのZX平面上での角度を求める
				float angle = atan2f(VelocityCmp.Velocity.x, VelocityCmp.Velocity.z);
				angle += D3DX_PI;

				D3DXQuaternionRotationAxis(&TransformCmp.Quat, &VecUp, angle);

				// 位置を計算、設定
				TransformCmp.Pos = (D3DXVECTOR3(newPos.x(), newPos.y() - 20.0f, newPos.z()));
			}
		}
	}
}