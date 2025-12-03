//****************************************************************
//
// 敵の追跡更新処理[UpdateEnemyChaseSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateEnemyChase.h"
#include "TagComp.hpp"
#include "EnemyAIComponent.hpp"
#include "TransformComponent.hpp"
#include "RigitBodyComponent.hpp"
#include "Velocity.hpp"
#include "LaserCollisionFragComp.hpp"
#include "ParentComponent.hpp"
#include "math.h"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateEnemyChaseSystem::Update(entt::registry& reg)
{
	auto view = reg.view<EnemyComponent, EnemtAIComp>();
	for (auto [Entity,State] : view.each())
	{
		if (State.State != EnemyState::ENEMYSTATE::CHASE) continue;
		auto PlayerView = reg.view<PlayerComponent>();
		// プレイヤーが存在したら
		if (!PlayerView.empty())
		{
			// Entityを取得
			auto playerEntity = *PlayerView.begin();
			// コンポーネントを取得
			auto& PlayerTransCmp = reg.get<Transform3D>(playerEntity);

			auto& RBCmp = reg.get<RigitBodyComp>(Entity);
			auto& TransformCmp = reg.get<Transform3D>(Entity);
			auto& VelocityCmp = reg.get<VelocityComp>(Entity);
			auto& Laser = reg.get<SingleParentComp>(Entity);
			auto& CollisionFrag = reg.get<LaserCollisionFragComp>(Laser.Parent);

			if (CollisionFrag.IsRayCollision == true)
			{
				State.State = EnemyState::ENEMYSTATE::PREDICTFRONT;
				State.LastLookPlayerPosition = PlayerTransCmp.Pos;
				continue;
			}

			if (RBCmp.RigitBody != nullptr)
			{
				D3DXVECTOR3 ToPlayer = PlayerTransCmp.Pos - TransformCmp.Pos;
				D3DXVec3Normalize(&ToPlayer, &ToPlayer);
				VelocityCmp.Velocity = { ToPlayer.x, ToPlayer.y, ToPlayer.z };
				VelocityCmp.Velocity.y = RBCmp.RigitBody->getLinearVelocity().y();
				// 設定
				RBCmp.RigitBody->setLinearVelocity(CMath::SetVec(VelocityCmp.Velocity * 5));

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