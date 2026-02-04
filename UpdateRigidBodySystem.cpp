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
	// 関数をコネクト
	entt::registry& Reg = CManager::GetScene()->GetReg();
	Reg.on_construct<RigidBodyComponent>().connect<&UpdateRigidBodySystem::OnRigidBodyComponentConstruct>(this);
	Reg.on_destroy<RigidBodyComponent>().connect<&UpdateRigidBodySystem::OnRigidBodyComponentDestruct>(this);
}

//*********************************************
// 更新
//*********************************************
void UpdateRigidBodySystem::Update(entt::registry& reg)
{
	// ビュー
	auto RigidBodyView = reg.view<RigitBody>();

	// アクセス
	for (auto Entity : RigidBodyView)
	{
		// トランスフォームと剛体をどちらか持っていない場合
		if (reg.all_of<RigidBodyComponent, Transform3D>(Entity) == false) continue;
		// コンポーネント取得
		auto& RigidBodyCmp = reg.get<RigidBodyComponent>(Entity);
		auto& TransformCmp = reg.get<Transform3D>(Entity);

		// 物理世界のトランスフォームを取得用
		btTransform TransBody;

		// 取得
		RigidBodyCmp.Body->getMotionState()->getWorldTransform(TransBody);

		// 親がいたら
		if (reg.any_of<ParentComp>(Entity))
		{
			// 親のコンポーネントを取得
			auto& ParentCmp = reg.get<ParentComp>(Entity);
			auto& ParentTransCmp = reg.get<Transform3D>(ParentCmp.Parent);

			// 親の逆トランスフォーム
			btTransform ParentInvTrans;
			ParentInvTrans.setRotation(CMath::SetQuad(ParentTransCmp.Quat));
			ParentInvTrans.setOrigin(CMath::SetVec(ParentTransCmp.Pos));
			ParentInvTrans = ParentInvTrans.inverse();
			// かけ合わせる
			TransBody.mult(TransBody, ParentInvTrans);
		}
		// 描画モデルの位置を設定
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

	// 当たり判定設定用一時変数
	btCompoundShape* Compound = new btCompoundShape;
	// 矩形のコライダー情報を持っていたら
	if (Reg.all_of<BoxColliderComponent>(Entity))
	{
		// ローカルのトランスフォーム設定用変数
		btTransform LocalTrans;
		LocalTrans.setIdentity();
		auto& BoxColliderComp = Reg.get<BoxColliderComponent>(Entity);
		LocalTrans.setRotation(CMath::SetQuad(BoxColliderComp.LocalQuat));
		LocalTrans.setOrigin(CMath::SetVec(BoxColliderComp.LocalPos));
		// 追加
		Compound->addChildShape(LocalTrans, new btBoxShape(CMath::SetVec(BoxColliderComp.HalfSize)));
	}
	// カプセルのコライダー情報を持っていたら
	if (Reg.all_of<CapsuleColliderComponent>(Entity))
	{
		// ローカルのトランスフォーム設定用変数
		btTransform LocalTrans;
		LocalTrans.setIdentity();
		auto& CapsuleColliderComp = Reg.get<CapsuleColliderComponent>(Entity);
		LocalTrans.setRotation(CMath::SetQuad(CapsuleColliderComp.LocalQuat));
		LocalTrans.setOrigin(CMath::SetVec(CapsuleColliderComp.LocalPos));
		// 追加
		Compound->addChildShape(LocalTrans, new btCapsuleShape(CapsuleColliderComp.Radius, CapsuleColliderComp.Height));
	}

	// コンポーネントを取得
	auto& TransformCmp = Reg.get<Transform3D>(Entity);
	auto& RBCmp = Reg.get<RigidBodyComponent>(Entity);
	// 当たり判定を代入
	RBCmp.Shape = Compound;

	// 剛体のトランスフォームを設定
	btTransform Transform;
	Transform.setIdentity();
	Transform.setRotation(CMath::SetQuad(TransformCmp.Quat));
	Transform.setOrigin(btVector3(TransformCmp.mtxWorld._41, TransformCmp.mtxWorld._42, TransformCmp.mtxWorld._43));

	// モーションステートを設定
	// NOTE:モーションステート...計算用の位置と描画用の位置のずれや違和感を無くすため
	btDefaultMotionState* MotionState = new btDefaultMotionState(Transform);
	btRigidBody::btRigidBodyConstructionInfo info(RBCmp.Mass, MotionState, RBCmp.Shape);

	// 剛体を生成
	RBCmp.Body = new btRigidBody(info);
	// 移動可能範囲を設定
	RBCmp.Body->setLinearFactor(btVector3(1, 1, 1));
	// ステートを設定
	RBCmp.Body->setActivationState(DISABLE_DEACTIVATION);
	// 物理世界に追加
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
		// 物理世界から削除
		CManager::GetDynamicsWorld()->removeRigidBody(RBCmp.Body);
		// モーションステートを削除
		if (RBCmp.Body->getMotionState())
		{
			delete RBCmp.Body->getMotionState();
		}
		// ポインタの削除
		delete RBCmp.Body;
	}

	// 当たり判定の削除
	if (RBCmp.Shape)
	{
		// コンパウンドなら
		if (RBCmp.Shape->isCompound())
		{
			// 子供を削除
			auto* compound = static_cast<btCompoundShape*>(RBCmp.Shape);
			for (int ChildNum = 0; ChildNum < compound->getNumChildShapes(); ChildNum++) delete compound->getChildShape(ChildNum);
		}
		// 削除
		delete RBCmp.Shape;
	}
}