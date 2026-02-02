//****************************************************************
//
// マップオブジェクト更新システムの処理[UpdateMapobjectSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateMapobjectSystem.h"
#include "SizeComponent.hpp"
#include "XRenderingComponent.hpp"
#include "TagComp.hpp"
#include "fade.h"
#include "math.h"
#include "scene.h"
#include "title.h"
#include "ParentComponent.hpp"
#include "Result.h"

// 名前空間
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateMapobjectSystem::Update(entt::registry& reg)
{
	// ビューを生成
	auto view = reg.view<MapObjectComponent>();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネントを取得
		auto& TransformCmp = reg.get<Transform3D>(entity);
		auto& RBCmp = reg.get<RigitBodyComp>(entity);
		auto& ColliderCmp = reg.get <SingleCollisionShapeComp>(entity);
		auto& XRenderCmp = reg.get<XRenderingComp>(entity);

		// ゴール用オブジェクトだったら
		if (XRenderCmp.FilePath.find("EXITPlate.x") != std::string::npos)
		{
			// 剛体の更新前に実行
			CollisionExitGate(reg, entity);
		}

		// 親のトランスフォーム
		Transform3D ParentTrans;

		// トランスフォーム
		btTransform trans;
		trans.setIdentity();

		if (RBCmp.RigitBody == nullptr) continue;

		// 親子関係コンポーネントが存在したら
		if (reg.any_of<ParentComp>(entity) == true)
		{
			// ゲット
			RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

			// 親のトランスフォームを取得
			auto& ParentCmp = reg.get<ParentComp>(entity);
			auto& ParentTransCmp = reg.get<Transform3D>(ParentCmp.Parent);
			ParentTrans = ParentTransCmp;

			// 回転行列を取得オフセット分掛ける
			// 物理世界での位置からオフセット分ずらした現実世界での位置を計算する用の変数
			btVector3 worldoffet = trans.getBasis() * btVector3(ColliderCmp.Offset.x, ColliderCmp.Offset.y, ColliderCmp.Offset.z);

			// 物理世界の位置から回転行列をかけ合わせたオフセットを引く
			btVector3 pos = (trans.getOrigin() - worldoffet) - btVector3(ParentTransCmp.Pos.x, ParentTransCmp.Pos.y, ParentTransCmp.Pos.z);

			// 位置に代入
			TransformCmp.Pos = { pos.x(), pos.y(), pos.z() };
		}
		else
		{
			// ゲット
			RBCmp.RigitBody->getMotionState()->getWorldTransform(trans);

			// 回転行列を取得オフセット分掛ける
			// 物理世界での位置からオフセット分ずらした現実世界での位置を計算する用の変数
			btVector3 worldoffet = trans.getBasis() * btVector3(ColliderCmp.Offset.x, ColliderCmp.Offset.y, ColliderCmp.Offset.z);

			// 物理世界の位置から回転行列をかけ合わせたオフセットを引く
			btVector3 pos = trans.getOrigin() - worldoffet;

			// 位置に代入
			TransformCmp.Pos = { pos.x(), pos.y(), pos.z() };
		}
	}
}

//*********************************************
// ゴールとの当たり判定
//*********************************************
void UpdateMapobjectSystem::CollisionExitGate(entt::registry& Reg, entt::entity MapObject)
{
	// ビューを生成
	auto PlayerView = Reg.view<PlayerComponent, SequenceTag::InGameComp>();
	auto& PlayerEntity = *PlayerView.begin();
	auto& RBPlayerCmp = Reg.get<RigitBodyComp>(PlayerEntity);

	// 自分のコンポーネントを取得
	auto& RBCmp = Reg.get<RigitBodyComp>(MapObject);

	// 何組が衝突しているか
	int numManifolds = CManager::GetDynamicsWorld()->getDispatcher()->getNumManifolds();

	// アクセスするために繰り返す
	for (int nCnt = 0; nCnt < numManifolds; nCnt++)
	{
		// ペアを取得
		btPersistentManifold* manifold = CManager::GetDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(nCnt);

		// 衝突オブジェクト1,2を取得
		const btCollisionObject* objA = manifold->getBody0();
		const btCollisionObject* objB = manifold->getBody1();

		// ゴールとプレイヤーが衝突していたら
		const bool Condition = (objA == RBPlayerCmp.RigitBody.get() && objB == RBCmp.RigitBody.get()) || (objA == RBCmp.RigitBody.get() && objB == RBPlayerCmp.RigitBody.get());

		// 自分とプレイヤーが当たったか？
		if (Condition == false) continue;
		// フラグを立てる
		CManager::SetClear(true);
		// 遷移
		CManager::GetFade()->SetFade(new CResult);
	}
}

//*********************************************
// 剛体の更新
//*********************************************
void UpdateMapobjectSystem::UpdateRB(Transform3D& ParentTransCmp, Transform3D& TransformCmp, RigitBodyComp& RBCmp, SingleCollisionShapeComp& ColliderCmp, Size3DComp& SizeCmp,entt::entity Entity)
{
	if (RBCmp.RigitBody != nullptr) return;
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
	btTransform transform, origin, offset,Parent;

	// 初期化
	transform.setIdentity();
	Parent.setIdentity();
	origin.setIdentity();
	offset.setIdentity();

	// OBBの回転（例：Y軸まわりに45度回転）
	btQuaternion rotation;

	rotation = CMath::SetQuad(ParentTransCmp.Quat);
	Parent.setRotation(rotation);
	Parent.setOrigin(btVector3(ParentTransCmp.Pos.x, ParentTransCmp.Pos.y, ParentTransCmp.Pos.z));

	rotation = CMath::SetQuad(TransformCmp.Quat);
	origin.setRotation(rotation);
	origin.setOrigin(btVector3(TransformCmp.Pos.x, TransformCmp.Pos.y, TransformCmp.Pos.z));

	offset.setOrigin(btVector3(ColliderCmp.Offset.x, ColliderCmp.Offset.y, ColliderCmp.Offset.z));

	transform.mult(origin, Parent);
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
	CManager::GetDynamicsWorld()->addRigidBody(RBCmp.RigitBody.get(), CollisionGroupAndMasks::GROUP_MAPOBJECT, CollisionGroupAndMasks::MASK_MAPOBJECT);
}