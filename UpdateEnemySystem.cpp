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
		// 移動処理
		UpdateMovement(reg, entity);
		// 剛体の更新
		UpdateRB(reg, entity);
	}
}

//*********************************************
// 移動
//*********************************************
void UpdateEnemySystem::UpdateMovement(entt::registry& Reg, entt::entity& Entity)
{
	auto PlayerView = Reg.view<PlayerComponent>();
	// プレイヤーが存在したら
	if (!PlayerView.empty())
	{
		// Entityを取得
		auto playerEntity = *PlayerView.begin();
		// コンポーネントを取得
		auto& PlayerTransCmp = Reg.get<Transform3D>(playerEntity);
		// 移動量を設定するための変数
		btVector3 moveDir(0, 0, 0);

		auto& RBCmp = Reg.get<RigitBodyComp>(Entity);
		auto& TransformCmp = Reg.get<Transform3D>(Entity);

		if (RBCmp.RigitBody != nullptr)
		{
			D3DXVECTOR3 ToPlayer = PlayerTransCmp.Pos - TransformCmp.Pos;
			D3DXVec3Normalize(&ToPlayer, &ToPlayer);
			moveDir.setValue(ToPlayer.x, ToPlayer.y, ToPlayer.z);
			moveDir *= 5.0f;
			moveDir.setY(RBCmp.RigitBody->getLinearVelocity().y());
			// 設定
			RBCmp.RigitBody->setLinearVelocity(moveDir);

			// トランスフォームを取得
			btTransform trans;
			RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

			// 描画モデルの位置
			btVector3 newPos;

			// 位置を取得
			newPos = trans.getOrigin();

			D3DXVECTOR3 Axis = CMath::SetVec(moveDir);
			D3DXVECTOR3 VecFront = { 0.0f,0.0f,-1.0f };
			D3DXVECTOR3 VecUp = VEC_UP;
			// 移動値を方向ベクトルに変換
			moveDir.normalize();

			// 方向ベクトルのZX平面上での角度を求める
			float angle = atan2f(moveDir.x(), moveDir.z());
			angle += D3DX_PI;

			D3DXQuaternionRotationAxis(&TransformCmp.Quat, &VecUp, angle);

			// 位置を計算、設定
			TransformCmp.Pos = (D3DXVECTOR3(newPos.x(), newPos.y() - 20.0f, newPos.z()));
		}
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

	ColliderCmp.CollisionShape = std::make_unique<btCapsuleShape>(btScalar(7.0f), btScalar(20.0f));

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