//****************************************************************
//
// プレイヤー更新システムのクラスの処理[playerUpdateSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "UpdateTitlePlayerSystem.h"
#include "TransformComponent.hpp"
#include "Components.hpp"
#include "OutLineComponent.hpp"
#include "Velocity.hpp"
#include "TagComp.hpp"
#include "CapsuleComponent.hpp"
#include "PlayerAnimetionComponent.hpp"
#include "LockOnUIAnimComponent.hpp"
#include "ParentComponent.hpp"
#include "SizeComponent.hpp"
#include "ColorComponent.hpp"
#include "SystemManager.h"
#include "game.h"
#include "ItemComponent.h"
#include "PlayerStateComponent.hpp"
#include "RenderFragComponent.hpp"
#include "UICircleComponent.hpp"
#include "ItemManagerComponent.hpp"
#include "mapmanager.h"
#include "math_T.h"
#include "math.h"

// 名前空間
using namespace Tag;
using namespace SequenceTag;

//*********************************************
// 更新
//*********************************************
void UpdateTitlePlayerSystem::Update(entt::registry& reg)
{
	// ビュー生成
	auto view = reg.view<Player, InTitle>();

	// アクセス
	for (auto [entity] : view.each())
	{
	}
}