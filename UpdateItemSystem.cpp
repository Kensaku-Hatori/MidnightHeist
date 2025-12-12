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
	auto PlayerView = reg.view<PlayerComponent>();

	// プレイヤーが存在したら
	if (PlayerView.empty()) return;

	// エンテティ取得
	auto PlayerEntity = *PlayerView.begin();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネントを取得
		auto& OutLineCmp = reg.get<OutLineComp>(entity);

		// プレイヤーのコンポーネント取得
		auto& PlayerRBCmp = reg.get<RigitBodyComp>(PlayerEntity);
		auto& PlayerCapsuleCmp = reg.get<CapsuleComp>(PlayerEntity);

		// プレイヤーのカプセル
		btCapsuleShape myCapsule(btScalar(PlayerCapsuleCmp.Radius), btScalar(PlayerCapsuleCmp.AllHeight));

		if (PlayerRBCmp.IsJump == false && PlayerRBCmp.IsGround(myCapsule) == true)
		{
			// 最初のノイズが終わったら
			if (CGame::IsFinishedFirstNoise() == true)OutLineCmp.Height -= 1.0f;
		}
	}
}