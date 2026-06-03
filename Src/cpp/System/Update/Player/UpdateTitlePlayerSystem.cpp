//****************************************************************
//
// プレイヤー更新システムのクラスの処理[playerUpdateSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "System/Update/Player/UpdateTitlePlayerSystem.h"
#include "Component/TransformComponent.hpp"
#include "Component/Components.hpp"
#include "Component/Shader/OutLineComponent.hpp"
#include "Component/VelocityComponent.hpp"
#include "TagComp.hpp"
#include "Component/Player/PlayerAnimetionComponent.hpp"
#include "Component/LockOnUIAnimComponent.hpp"
#include "Component/ParentComponent.hpp"
#include "Component/SizeComponent.hpp"
#include "Component/Polygone/ColorComponent.hpp"
#include "Manager/SystemManager.h"
#include "Scene/game.h"
#include "Component/ItemComponent.h"
#include "Component/Player/PlayerStateComponent.hpp"
#include "Component/RenderFragComponent.hpp"
#include "Component/Shader/UICircleComponent.hpp"
#include "Component/ItemManagerComponent.hpp"
#include "Manager/mapmanager.h"
#include "Math/MyMath_T.h"
#include "Math/MyMath.h"

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