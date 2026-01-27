//****************************************************************
//
// ゲートマネージャーシステムの処理[UpdateGateManager.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateGateManager.h"
#include "TagComp.hpp"
#include "ChildComp.hpp"
#include "TransformComponent.hpp"
#include "GateManagerComponent.hpp"

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
		auto& ChildrenCmp = reg.get<ChildrenComp>(entity);

		// アクセス
		for (int nCnt = 0; nCnt < ChildrenCmp.Children.size(); nCnt++)
		{
			// コンポーネントを取得
			auto& TransformCmp = reg.get<Transform3D>(ChildrenCmp.Children[nCnt]);
			if (fabsf(TransformCmp.Pos.x) > 75.0f) break;
			// 設定用の移動地
			D3DXVECTOR3 Move;
			Move = D3DXVECTOR3(5.0f * powf(-1.0f, nCnt), 0.0f, 0.0f);
			// 足す
			TransformCmp.Pos += Move;
		}
	}
}