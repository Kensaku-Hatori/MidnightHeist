//****************************************************************
//
// アイテム更新システムの処理[UpdateItemSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateItemSystem.h"
#include "OutLineComp.hpp"
#include "TagComp.hpp"
#include "RigitBodyComponent.hpp"
#include "CapsuleComponent.hpp"
#include "PlayerAnimetionComponent.hpp"
#include "game.h"

// 名前空間
using namespace Tag;

//***************************************
// 更新
//***************************************
void UpdateItemSystem::Update(entt::registry& reg)
{
	// ビューを取得
	auto view = reg.view<ItemComponent>();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネントを取得
		auto& OutLineCmp = reg.get<OutLineComp>(entity);

		// 最初のノイズが終わったら
		if (CGame::IsFinishedFirstNoise() == true)OutLineCmp.Height -= 1.0f;
	}
}