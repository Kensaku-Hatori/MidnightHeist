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
void UpdateEnemyPredictFrontSystem::Update(entt::registry& reg)
{
	auto view = reg.view<EnemyComponent, EnemtAIComp>();
	for (auto [Entity, State] : view.each())
	{
		if (State.State != EnemyState::ENEMYSTATE::PREDICTFRONT) continue;
		auto& RBCmp = reg.get<RigitBodyComp>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		auto& VelocityCmp = reg.get<VelocityComp>(Entity);

		if (RBCmp.RigitBody == nullptr) continue;

		btCapsuleShape myShape(15.0f, 20.0f);;
		const btCollisionObject* Ground = RBCmp.GetGroundCollisionObject(myShape);

		// 何組が衝突しているか
		int numManifolds = CManager::GetDynamicsWorld()->getDispatcher()->getNumManifolds();

		// 衝突回数分繰り返し
		for (int nCnt = 0; nCnt < numManifolds; nCnt++)
		{
			// 衝突しているペアを取得
			btPersistentManifold* manifold = CManager::GetDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(nCnt);

			// 衝突していたら
			if (manifold->getNumContacts() <= 0) continue;

			// 衝突オブジェクト１、２を取得
			const btCollisionObject* objA = manifold->getBody0();
			const btCollisionObject* objB = manifold->getBody1();

			bool IsEnemyVsOther =
				(objA == RBCmp.RigitBody.get() && objB != Ground);

			if (IsEnemyVsOther == false) continue;
			// 接触点ごとにループ
			for (int ContactPoint = 0; ContactPoint < manifold->getNumContacts(); ContactPoint++)
			{
				btManifoldPoint& pt = manifold->getContactPoint(ContactPoint);

				if (pt.getDistance() > 0.0f) continue;
				btVector3 normalOnB = pt.m_normalWorldOnB;
				D3DXVECTOR3 NorContact, VecUp, SetVec;
				NorContact = CMath::SetVec(normalOnB);
				VecUp = VEC_UP;
				D3DXVec3Cross(&SetVec, &NorContact, &VecUp);
				D3DXVec3Normalize(&SetVec, &SetVec);
				VelocityCmp.Velocity = SetVec;
				State.State = EnemyState::ENEMYSTATE::PREDICTFOLLOW;
				// 処理を切り上げる
				return;
			}
		}
		D3DXVECTOR3 Setter;
		Setter = VelocityCmp.Velocity * 5;
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

//*********************************************
// 更新
//*********************************************
void UpdateEnemyPredictFollowSystem::Update(entt::registry& reg)
{
	auto view = reg.view<EnemyComponent, EnemtAIComp>();
	for (auto [Entity, State] : view.each())
	{
		if (State.State != EnemyState::ENEMYSTATE::PREDICTFOLLOW) continue;
		auto& RBCmp = reg.get<RigitBodyComp>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		auto& VelocityCmp = reg.get<VelocityComp>(Entity);

		if (RBCmp.RigitBody != nullptr)
		{
			btCapsuleShape myShape(35.0f, 20.0f);;
			const btCollisionObject* Ground = RBCmp.GetGroundCollisionObject(myShape);

			D3DXVECTOR3 Setter;
			if (Ground == nullptr)
			{
				VelocityCmp.Velocity = -VelocityCmp.Velocity;
			}

			Setter = VelocityCmp.Velocity * 5;
			Setter.y = RBCmp.RigitBody->getLinearVelocity().y();
			// 設定
			RBCmp.RigitBody->setLinearVelocity(CMath::SetVec(Setter));

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
}