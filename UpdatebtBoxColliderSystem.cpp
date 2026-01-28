//****************************************************************
//
// 物理エンジン用の矩形の更新システムの処理[UpdatebtBoxColliderSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdatebtBoxColliderSystem.h"
#include "TagComp.hpp"
#include "RigitBodyComponent.hpp"
#include "SizeComponent.hpp"
#include "TransformComponent.hpp"
#include "SingleCollisionShapeComponent.hpp"
#include "XRenderingComponent.hpp"

// 名前空間
using namespace Pysics;

//***************************************
// 更新処理
//***************************************
void UpdatebtBoxColliderSystem::Update(entt::registry& reg)
{
	auto BoxColliderView = reg.view<btBoxColliderComponent>();
	for (auto Entity : BoxColliderView)
	{
		if (reg.all_of<RigitBodyComp, Size3DComp, Transform3D, SingleCollisionShapeComp>(Entity) == false) continue;

		auto& RBCmp = reg.get<RigitBodyComp>(Entity);
		auto& SizeCmp = reg.get<Size3DComp>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		auto& ColliderCmp = reg.get<SingleCollisionShapeComp>(Entity);

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

		// 当たり判定を再生成
		ColliderCmp.CollisionShape.reset(new btBoxShape(btVector3(SizeCmp.Size.x * TransformCmp.Scale.x, SizeCmp.Size.y * TransformCmp.Scale.y, SizeCmp.Size.z * TransformCmp.Scale.z)));
		ColliderCmp.Offset.y = SizeCmp.Size.y * TransformCmp.Scale.y;

		// 質量を宣言
		btScalar Mass = 0;

		// 抗力を代入
		btVector3 Inertia = { 0.0f,0.0f,0.0f };

		// 抗力を設定
		ColliderCmp.CollisionShape->calculateLocalInertia(Mass, Inertia);

		// 物理世界の位置などを取得
		btTransform transform, offset;

		// 初期化
		transform.setIdentity();
		offset.setIdentity();

		// 回転
		btQuaternion rotation;

		rotation = CMath::SetQuad(TransformCmp.Quat);
		transform.setRotation(rotation);
		transform.setOrigin(btVector3(TransformCmp.mtxWorld._41, TransformCmp.mtxWorld._42, TransformCmp.mtxWorld._43));

		offset.setOrigin(btVector3(ColliderCmp.Offset.x, ColliderCmp.Offset.y, ColliderCmp.Offset.z));

		transform.mult(transform, offset);

		// インターフェイスを設定
		btDefaultMotionState* motionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo info(Mass, motionState, ColliderCmp.CollisionShape.get());

		// リジットボディーを再生成
		RBCmp.RigitBody.reset(new btRigidBody(info));

		// 移動方向を制限
		RBCmp.RigitBody->setLinearFactor(btVector3(1, 1, 1));

		// ユーザーポインタを設定
		RBCmp.RigitBody->setUserPointer((void*)Entity);

		// スリープ状態を設定
		RBCmp.RigitBody->setActivationState(DISABLE_DEACTIVATION);

		// 物理世界にリジットボディーを追加
		CManager::GetDynamicsWorld()->addRigidBody(RBCmp.RigitBody.get(), ColliderCmp.Group, ColliderCmp.Mask);

		RBCmp.IsNeedUpdate = false;
	}
}