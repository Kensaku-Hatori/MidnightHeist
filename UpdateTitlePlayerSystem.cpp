//****************************************************************
//
// プレイヤー更新システムのクラスの処理[playerUpdateSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateTitlePlayerSystem.h"
#include "TransformComponent.hpp"
#include "RigitBodyComponent.hpp"
#include "SingleCollisionShapeComponent.hpp"
#include "OutLineComp.hpp"
#include "Velocity.hpp"
#include "TagComp.hpp"
#include "CapsuleComponent.hpp"
#include "PlayerAnimetionComponent.hpp"
#include "LockOnUIAnim.hpp"
#include "ParentComponent.hpp"
#include "SizeComponent.hpp"
#include "ColorComponent.hpp"
#include "SystemManager.h"
#include "game.h"
#include "ItemComp.h"
#include "PlayerStateComp.hpp"
#include "RenderFragComp.hpp"
#include "UICircleComp.hpp"
#include "ItemManagerComp.hpp"
#include "mapmanager.h"
#include "math_T.h"
#include "math.h"

// 名前空間
using namespace Tag;
using namespace SequenceTag;

//*********************************************
// 更新
//*********************************************
void UpdateTitlePlayerSystem::Update(entt::registry& reg)
{
	auto view = reg.view<PlayerComponent, InTitleComp>();

	for (auto entity : view)
	{
		// コンポーネント取得
		auto& TransformCmp = reg.get<Transform3D>(entity);
		auto& RBCmp = reg.get<RigitBodyComp>(entity);
		auto& CapsuleCmp = reg.get<CapsuleComp>(entity);

		// トランスフォームを取得
		btTransform trans;
		trans = RBCmp.RigitBody->getWorldTransform();

		// 描画モデルの位置
		btVector3 newPos;

		// 位置を取得
		newPos = trans.getOrigin();

		// 位置を計算、設定
		TransformCmp.Pos = (D3DXVECTOR3(newPos.x(), newPos.y() - CapsuleCmp.ToCenterOffset, newPos.z()));

		UpdateRB(reg, entity);
	}
}

//*********************************************
// 剛体の更新
//*********************************************
void UpdateTitlePlayerSystem::UpdateRB(entt::registry& reg, entt::entity Player)
{
	// コンポーネント取得
	auto& RBCmp = reg.get<RigitBodyComp>(Player);
	auto& ColliderCmp = reg.get<SingleCollisionShapeComp>(Player);
	auto& CapsuleCmp = reg.get<CapsuleComp>(Player);
	auto& TransformCmp = reg.get<Transform3D>(Player);

	if (RBCmp.RigitBody != nullptr) return;

	ColliderCmp.CollisionShape = std::make_unique<btCapsuleShape>(btScalar(CapsuleCmp.Radius), btScalar(CapsuleCmp.AllHeight));

	btScalar mass = 1.0f; // 質量を1以上にすることで動的剛体になる
	btVector3 inertia(0, 0, 0);
	ColliderCmp.CollisionShape->calculateLocalInertia(mass, inertia);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(TransformCmp.Pos.x, TransformCmp.Pos.y + CapsuleCmp.ToCenterOffset, TransformCmp.Pos.z));

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, ColliderCmp.CollisionShape.get());

	RBCmp.RigitBody = std::make_unique<btRigidBody>(info);
	RBCmp.RigitBody->setLinearFactor(btVector3(1, 1, 1));

	RBCmp.RigitBody->setUserPointer(this);
	RBCmp.RigitBody->setActivationState(DISABLE_DEACTIVATION);

	CManager::GetDynamicsWorld()->addRigidBody(RBCmp.RigitBody.get(), CollisionGroupAndMasks::GROUP_PLAYER, CollisionGroupAndMasks::MASK_PLAYER);
}