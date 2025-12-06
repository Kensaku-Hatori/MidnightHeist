//****************************************************************
//
// 敵の追跡更新処理[UpdateEnemyChaseSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateEnemyPredict.h"
#include "TagComp.hpp"
#include "EnemyAIComponent.hpp"
#include "TransformComponent.hpp"
#include "RigitBodyComponent.hpp"
#include "Velocity.hpp"
#include "LaserCollisionFragComp.hpp"
#include "ParentComponent.hpp"
#include "SingleCollisionShapeComponent.hpp"
#include "math.h"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateEnemyPredictSystem::Update(entt::registry& reg)
{
	auto view = reg.view<EnemyComponent, EnemtAIComp>();
	for (auto [Entity, State] : view.each())
	{
		if (State.State != EnemyState::ENEMYSTATE::PREDICT) continue;
		auto& RBCmp = reg.get<RigitBodyComp>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		auto& VelocityCmp = reg.get<VelocityComp>(Entity);

		if (RBCmp.RigitBody == nullptr) continue;

		D3DXVECTOR3 Setter;
		Setter = VelocityCmp.Velocity * 9;
		Setter.y = RBCmp.RigitBody->getLinearVelocity().y();
		// 設定
		RBCmp.RigitBody->setLinearVelocity(CMath::SetVec(Setter));;

		// トランスフォームを取得
		btTransform trans;
		RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

		// 描画モデルの位置
		btVector3 newPos;

		// 位置を取得
		newPos = trans.getOrigin();

		D3DXVECTOR3 VecUp = VEC_UP;
		D3DXVECTOR3 VecVelocity;
		// 移動値を方向ベクトルに変換
		D3DXVec3Normalize(&VecVelocity, &VelocityCmp.Velocity);

		// 方向ベクトルのZX平面上での角度を求める
		float angle = atan2f(VecVelocity.x, VecVelocity.z);
		angle += D3DX_PI;

		D3DXQuaternionRotationAxis(&TransformCmp.Quat, &VecUp, angle);

		// 位置を計算、設定
		TransformCmp.Pos = (D3DXVECTOR3(newPos.x(), newPos.y() - 20.0f, newPos.z()));
	}
}