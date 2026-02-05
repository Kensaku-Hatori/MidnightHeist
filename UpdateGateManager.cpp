//****************************************************************
//
// ゲートマネージャーシステムの処理[UpdateGateManager.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateGateManager.h"
#include "TagComp.hpp"
#include "ChildComponent.hpp"
#include "TransformComponent.hpp"
#include "GateManagerComponent.hpp"
#include "Components.hpp"

// ネームスペース
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void UpdateGateManagerSystem::Update(entt::registry& reg)
{
	// ビュー取得
	auto view = reg.view<GateManager>();

	// アクセス
	for (auto entity : view)
	{
		// フラグコンポーネントを取得
		auto& OpenTriggerCmp = reg.get<GateManagerComponent>(entity);
		// フラグが立っていなかったら
		if (OpenTriggerCmp.m_IsOpenTrigger == false) return;

		// 子供のリストを取得
		auto& ChildrenCmp = reg.get<ChildrenComponent>(entity);

		// アクセス
		for (int nCnt = 0; nCnt < static_cast<int>(ChildrenCmp.Children.size()); nCnt++)
		{
			// コンポーネントを取得
			auto& RBCmp = reg.get<RigidBodyComponent>(ChildrenCmp.Children[nCnt]);
			auto& TransformCmp = reg.get<Transform3D>(ChildrenCmp.Children[nCnt]);
			// ゲートの移動量が制限に達していたら
			if (fabsf(TransformCmp.Pos.x) > 75.0f) break;
			// 設定用の移動量、位置
			btVector3 Pos, Move;
			// トランスフォーム取得用変数
			btTransform Trans;
			// 移動量計算
			Move = btVector3(10.0f * pow(-1, nCnt), 0.0f, 0.0f);
			// トランスフォームを取得
			RBCmp.Body->getMotionState()->getWorldTransform(Trans);
			// 移動した後の位置を計算
			Pos = Trans.getOrigin();
			Pos.setValue(Pos.x() + Move.x(), Pos.y() + Move.y(), Pos.z() + Move.z());
			Trans.setOrigin(Pos);
			// 設定
			RBCmp.Body->getMotionState()->setWorldTransform(Trans);
			RBCmp.Body->setWorldTransform(Trans);
		}
	}
}