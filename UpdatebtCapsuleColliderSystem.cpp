//****************************************************************
//
// 物理エンジン用のカプセルの更新システムの処理[UpdatebtCapsuleColliderSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdatebtCapsuleColliderSystem.h"
#include "TagComp.hpp"
#include "manager.h"
#include "RigitBodyComponent.hpp"
#include "TransformComponent.hpp"
#include "CapsuleComponent.hpp"
#include "SingleCollisionShapeComponent.hpp"

// 名前空間
using namespace Pysics;

//***************************************
// 更新処理
//***************************************
void UpdatebtCapsuleColliderSystem::Update(entt::registry& reg)
{
	auto CapsuleColliderView = reg.view< Pysics::CapsuleCollider>();
	for (auto Entity : CapsuleColliderView)
	{
		if (reg.all_of<RigitBodyComp, CapsuleComp, Transform3D, SingleCollisionShapeComp>(Entity) == false) continue;

		// コンポーネント取得
		auto& RBCmp = reg.get<RigitBodyComp>(Entity);
		auto& ColliderCmp = reg.get<SingleCollisionShapeComp>(Entity);
		auto& CapsuleCmp = reg.get<CapsuleComp>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);

		if (RBCmp.IsNeedUpdate == false) continue;

		// 剛体の削除
		if (RBCmp.RigitBody)
		{
			// 物理世界から削除
			CManager::GetDynamicsWorld()->removeRigidBody(RBCmp.RigitBody.get());

			// モーションステートを取得nullチェック
			if (RBCmp.RigitBody->getMotionState())
			{
				// モーションステートを削除
				delete RBCmp.RigitBody->getMotionState();
			}
			// リジットボディをクリア
			RBCmp.RigitBody.reset();
		}

		// ポインタ生成
		ColliderCmp.CollisionShape = std::make_unique<btCapsuleShape>(btScalar(CapsuleCmp.Radius), btScalar(CapsuleCmp.AllHeight));

		// 質量
		btScalar mass = 1.0f;
		// 回転抗力
		btVector3 inertia(0, 0, 0);
		// 生成
		ColliderCmp.CollisionShape->calculateLocalInertia(mass, inertia);

		// トランスフォーム
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(TransformCmp.mtxWorld._41, TransformCmp.mtxWorld._42 + CapsuleCmp.ToCenterOffset, TransformCmp.mtxWorld._43));

		// モーションステート生成
		btDefaultMotionState* motionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, ColliderCmp.CollisionShape.get());

		// リジットボディー生成
		RBCmp.RigitBody = std::make_unique<btRigidBody>(info);
		// 移動制限
		RBCmp.RigitBody->setLinearFactor(btVector3(1, 1, 1));

		// ユーザーポインタ・ステート設定
		RBCmp.RigitBody->setUserPointer((void*)Entity);
		RBCmp.RigitBody->setActivationState(DISABLE_DEACTIVATION);

		// 物理世界に登録
		CManager::GetDynamicsWorld()->addRigidBody(RBCmp.RigitBody.get(), ColliderCmp.Group, ColliderCmp.Mask);

		RBCmp.IsNeedUpdate = false;
	}
}