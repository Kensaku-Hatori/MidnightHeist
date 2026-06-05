//================================================================
//
// マップオブジェクト更新システムの処理[UpdateMapobjectSystem.cpp]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// インクルード
//****************************************************************
#include "System/Update/UpdateMapobjectSystem.h"
#include "Component/SizeComponent.hpp"
#include "Component/XRenderingComponent.hpp"
#include "TagComp.hpp"
#include "Bace/fade.h"
#include "Math/MyMath.h"
#include "Scene/scene.h"
#include "Scene/title.h"
#include "Component/ParentComponent.hpp"
#include "Scene/Result.h"
#include "Component/Components.hpp"

//****************************************************************
// 名前空間
//****************************************************************
using namespace Tag;

//****************************************************************
// 更新
//****************************************************************
void UpdateMapobjectSystem::Update(entt::registry& reg)
{
	// ビューを生成
	auto view = reg.view<MapObject>();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネントを取得
		auto& XRenderCmp = reg.get<XRenderingComponent>(entity);

		// ゴール用オブジェクトだったら
		if (XRenderCmp.FilePath.find("EXITPlate.x") != std::string::npos)
		{
			// 剛体の更新前に実行
			CollisionExitGate(reg, entity);
		}
	}
}

//****************************************************************
// ゴールとの当たり判定
//****************************************************************
void UpdateMapobjectSystem::CollisionExitGate(entt::registry& Reg, entt::entity MapObject)
{
	// ビューを生成
	auto PlayerView = Reg.view<Player, SequenceTag::InGame>();
	auto& PlayerEntity = *PlayerView.begin();
	auto& RBPlayerCmp = Reg.get<RigidBodyComponent>(PlayerEntity);

	// 自分のコンポーネントを取得
	auto& RBCmp = Reg.get<RigidBodyComponent>(MapObject);

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
		const bool Condition = (objA == RBPlayerCmp.Body && objB == RBCmp.Body) || (objA == RBCmp.Body && objB == RBPlayerCmp.Body);

		// 自分とプレイヤーが当たったか？
		if (Condition == false) continue;
		// フラグを立てる
		CManager::SetClear(true);
		// 遷移
		CFade::Instance().SetFade(new CResult);
	}
}