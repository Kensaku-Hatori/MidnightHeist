//****************************************************************
//
// 剛体の更新システムの処理[UpdateRigitBodySystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateRigidBodySystem.h"
#include "Components.hpp"
#include "scene.h"
#include "manager.h"
#include "TransformComponent.hpp"
#include "Components.hpp"
#include "TagComp.hpp"
#include "math.h"
#include "ParentComponent.hpp"

using namespace Pysics;

//*********************************************
// コンストラクタ
//*********************************************
UpdateRigidBodySystem::UpdateRigidBodySystem()
{
	entt::registry& Reg = CManager::GetScene()->GetReg();
	Reg.on_construct<RigidBodyComponent>().connect<&UpdateRigidBodySystem::OnRigidBodyComponentConstruct>(this);
	Reg.on_destroy<RigidBodyComponent>().connect<&UpdateRigidBodySystem::OnRigidBodyComponentDestruct>(this);
}

//*********************************************
// 更新
//*********************************************
void UpdateRigidBodySystem::Update(entt::registry& reg)
{
	auto RigidBodyView = reg.view<RigitBody>();
	for (auto Entity : RigidBodyView)
	{
		if (reg.all_of<RigidBodyComponent, Transform3D>(Entity) == false) continue;
		auto& RigidBodyCmp = reg.get<RigidBodyComponent>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		btTransform TransBody;

		RigidBodyCmp.Body->getMotionState()->getWorldTransform(TransBody);

		if (reg.any_of<ParentComp>(Entity))
		{
			auto& ParentCmp = reg.get<ParentComp>(Entity);
			auto& ParentTransCmp = reg.get<Transform3D>(ParentCmp.Parent);
			btTransform ParentInvTrans;
			ParentInvTrans.setRotation(CMath::SetQuad(ParentTransCmp.Quat));
			ParentInvTrans.setOrigin(CMath::SetVec(ParentTransCmp.Pos));
			ParentInvTrans = ParentInvTrans.inverse();
			TransBody.mult(TransBody, ParentInvTrans);
		}
		TransformCmp.Pos = CMath::SetVec(TransBody.getOrigin());

		// レイの始点(少し上にずらす)
		btVector3 RayOrigin = TransBody.getOrigin();
		RayOrigin.setY(RayOrigin.getY() + 1.0f);

		// レイの終点(少し下にずらす)
		btVector3 RayEnd = RayOrigin + btVector3(0, btScalar(-1.0f), 0);

		// 着地判定
		btCollisionWorld::ClosestRayResultCallback callback(RayOrigin, RayEnd);
		CManager::GetDynamicsWorld()->rayTest(RayOrigin, RayEnd, callback);
		RigidBodyCmp.IsGround = callback.hasHit();
	}
}

//*********************************************
// 剛体が追加されたら
//*********************************************
void UpdateRigidBodySystem::OnRigidBodyComponentConstruct(entt::registry& Reg, entt::entity Entity)
{
	// 親子関係コンポーネントが存在したら
	if (Reg.all_of<Transform3D>(Entity) == false) return;

	std::vector<btCollisionShape*> shapes;
	btCompoundShape* compound = new btCompoundShape;
	if (Reg.all_of<BoxColliderComponent>(Entity))
	{
		btTransform LocalTrans;
		LocalTrans.setIdentity();
		auto& BoxColliderComp = Reg.get<BoxColliderComponent>(Entity);
		LocalTrans.setRotation(CMath::SetQuad(BoxColliderComp.LocalQuat));
		LocalTrans.setOrigin(CMath::SetVec(BoxColliderComp.LocalPos));
		compound->addChildShape(LocalTrans, new btBoxShape(CMath::SetVec(BoxColliderComp.HalfSize)));
	}

	if (Reg.all_of<CapsuleColliderComponent>(Entity))
	{
		btTransform LocalTrans;
		LocalTrans.setIdentity();
		auto& CapsuleColliderComp = Reg.get<CapsuleColliderComponent>(Entity);
		LocalTrans.setRotation(CMath::SetQuad(CapsuleColliderComp.LocalQuat));
		LocalTrans.setOrigin(CMath::SetVec(CapsuleColliderComp.LocalPos));
		compound->addChildShape(LocalTrans, new btCapsuleShape(CapsuleColliderComp.Radius, CapsuleColliderComp.Height));
	}

	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& RBCmp = Reg.get<RigidBodyComponent>(Entity);
	RBCmp.Shape = compound;	// コンポーネントを取得

	btVector3 inertia(0, 0, 0);

	btTransform transform;
	transform.setIdentity();
	transform.setRotation(CMath::SetQuad(TransformCmp.Quat));
	transform.setOrigin(btVector3(TransformCmp.mtxWorld._41, TransformCmp.mtxWorld._42, TransformCmp.mtxWorld._43));

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(RBCmp.Mass, motionState, RBCmp.Shape);

	RBCmp.Body = new btRigidBody(info);
	RBCmp.Body->setLinearFactor(btVector3(1, 1, 1));

	RBCmp.Body->setActivationState(DISABLE_DEACTIVATION);

	CManager::GetDynamicsWorld()->addRigidBody(RBCmp.Body, RBCmp.Group, RBCmp.Mask);
}

//*********************************************
// 剛体が削除されたら
//*********************************************
void UpdateRigidBodySystem::OnRigidBodyComponentDestruct(entt::registry& Reg, entt::entity Entity)
{
	// コンポーネントを取得
	auto& RBCmp = Reg.get<RigidBodyComponent>(Entity);

	// 剛体の削除
	if (RBCmp.Body)
	{
		CManager::GetDynamicsWorld()->removeRigidBody(RBCmp.Body);
		if (RBCmp.Body->getMotionState())
		{
			delete RBCmp.Body->getMotionState();
		}
		delete RBCmp.Body;
	}

	if (RBCmp.Shape)
	{
		if (RBCmp.Shape->isCompound())
		{
			auto* compound = static_cast<btCompoundShape*>(RBCmp.Shape);
			for (int ChildNum = 0; ChildNum < compound->getNumChildShapes(); ChildNum++) delete compound->getChildShape(ChildNum);
		}
		delete RBCmp.Shape;
	}
}