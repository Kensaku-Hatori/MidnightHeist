//****************************************************************
//
// 敵の更新処理[UpdateEnemySystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "TagComp.hpp"
#include "TransformComponent.hpp"
#include "RigitBodyComponent.hpp"
#include "UpdateEnemySystem.h"
#include "SingleCollisionShapeComponent.hpp"
#include "SizeComponent.hpp"
#include "Velocity.hpp"
#include "FanInfoComponent.hpp"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateEnemySystem::Update(entt::registry& reg)
{
	auto view = reg.view<EnemyComponent>();

	for (auto entity : view)
	{
		auto& Transform = reg.get<Transform3D>(entity);
		auto& FanInfoCmp = reg.get<FanComp>(entity);
		D3DXMATRIX myMatrix = Transform.GetWorldMatrix();

		FanInfoCmp.Origin = { myMatrix._41,myMatrix._42,myMatrix._43 };
		FanInfoCmp.Dir = { -myMatrix._31,-myMatrix._32,-myMatrix._33 };

		// 剛体の更新
		UpdateRB(reg, entity);
	}
}

//*********************************************
// 剛体更新
//*********************************************
void UpdateEnemySystem::UpdateRB(entt::registry& Reg, entt::entity& Entity)
{
	auto& RBCmp = Reg.get<RigitBodyComp>(Entity);
	if (RBCmp.RigitBody != nullptr) return;

	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& ColliderCmp = Reg.get <SingleCollisionShapeComp>(Entity);

	ColliderCmp.CollisionShape = std::make_unique<btCapsuleShape>(btScalar(15.0f), btScalar(20.0f));

	btScalar mass = 1.0f; // 質量を1以上にすることで動的剛体になる
	btVector3 inertia(0, 0, 0);
	ColliderCmp.CollisionShape->calculateLocalInertia(mass, inertia);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(TransformCmp.Pos.x, TransformCmp.Pos.y + 20.0f, TransformCmp.Pos.z));

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, ColliderCmp.CollisionShape.get());

	RBCmp.RigitBody = std::make_unique<btRigidBody>(info);
	RBCmp.RigitBody->setLinearFactor(btVector3(1, 1, 1));

	RBCmp.RigitBody->setUserPointer(this);
	RBCmp.RigitBody->setActivationState(DISABLE_DEACTIVATION);

	CManager::GetDynamicsWorld()->addRigidBody(RBCmp.RigitBody.get());
}