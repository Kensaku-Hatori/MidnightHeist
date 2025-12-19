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
#include "ParentComponent.hpp"
#include "FanInfoComponent.hpp"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateEnemySystem::Update(entt::registry& reg)
{
	// ビュー取得
	auto view = reg.view<EnemyComponent>();

	// アクセス
	for (auto entity : view)
	{
		// 情報を取得
		auto& Transform = reg.get<Transform3D>(entity);
		auto& FanInfoCmp = reg.get<FanComp>(entity);
		auto& ParentCmp = reg.get<MulParentComp>(entity);
		auto& EnemyAiCmp = reg.get<EnemyAIComp>(entity);

		// マトリックスを取得
		D3DXMATRIX myMatrix = Transform.GetWorldMatrix();
		// 扇の位置と向きを設定
		FanInfoCmp.Origin = { myMatrix._41,myMatrix._42,myMatrix._43 };
		FanInfoCmp.Dir = { -myMatrix._31,-myMatrix._32,-myMatrix._33 };

		EnemyAiCmp.Emitter->SetPos(Transform.Pos);
		EnemyAiCmp.Emitter->SetFront({ -myMatrix._31,-myMatrix._32,-myMatrix._33 });
		EnemyAiCmp.Emitter->Update();

		D3DXVECTOR3 VecUp = VEC_UP;
		D3DXVECTOR3 Dir;
		D3DXQUATERNION SetQuat;
		// 移動値を方向ベクトルに変換
		D3DXVec3Normalize(&Dir, &FanInfoCmp.Dir);

		// 方向ベクトルのZX平面上での角度を求める
		float angle = atan2f(Dir.x, Dir.z);
		angle += D3DX_PI;

		D3DXQuaternionRotationAxis(&SetQuat, &VecUp, angle);
		auto& SightInfo = reg.get<Transform3D>(ParentCmp.Parents[1]);
		SightInfo.Pos = FanInfoCmp.Origin;
		SightInfo.QuatDest = SetQuat;
		// 剛体の更新
		UpdateRB(reg, entity);
	}
}

//*********************************************
// 剛体更新
//*********************************************
void UpdateEnemySystem::UpdateRB(entt::registry& Reg, entt::entity& Entity)
{
	// 剛体情報を取得
	auto& RBCmp = Reg.get<RigitBodyComp>(Entity);
	// 生成されていなかったら
	if (RBCmp.RigitBody != nullptr) return;

	// 自分の情報を取得
	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& ColliderCmp = Reg.get <SingleCollisionShapeComp>(Entity);

	// 当たり判定を生成
	ColliderCmp.CollisionShape = std::make_unique<btCapsuleShape>(btScalar(15.0f), btScalar(20.0f));

	// 質量を設定する用の変数
	btScalar mass = 1.0f;
	// 回転抗力
	btVector3 inertia(0, 0, 0);
	// 当たり判定に情報を渡す
	ColliderCmp.CollisionShape->calculateLocalInertia(mass, inertia);

	// 剛体のトランスフォームを設定する用の変数
	btTransform transform;
	transform.setIdentity();
	// 原点を設定
	transform.setOrigin(btVector3(TransformCmp.Pos.x, TransformCmp.Pos.y + 20.0f, TransformCmp.Pos.z));

	// モーションステートを生成
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	// 剛体生成時に必要な情報
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, ColliderCmp.CollisionShape.get());

	// 生成
	RBCmp.RigitBody = std::make_unique<btRigidBody>(info);

	// 移動制限を設定
	RBCmp.RigitBody->setLinearFactor(btVector3(1, 1, 1));

	// ユーザーポインタを設定
	RBCmp.RigitBody->setUserPointer(this);
	// 剛体の状態を設定
	RBCmp.RigitBody->setActivationState(DISABLE_DEACTIVATION);

	// 物理世界に追加
	CManager::GetDynamicsWorld()->addRigidBody(RBCmp.RigitBody.get(), CollisionGroupAndMasks::GROUP_ENEMY, CollisionGroupAndMasks::MASK_ENEMY);
}