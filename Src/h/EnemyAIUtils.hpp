//****************************************************************
//
// 敵のAIに必要なヘッダーまとめ[EnemyAIUtils.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "TagComp.hpp"
#include "Component/AIComponent.hpp"
#include "Component/TransformComponent.hpp"
#include "Component/VelocityComponent.hpp"
#include "Component/Enemy/EnemyPatrolPointComponent.hpp"
#include "Component/FanInfoComponent.hpp"
#include "Component/ParentComponent.hpp"
#include "Factories.h"
#include "Manager/mapmanager.h"
#include "Math/MyMath.h"
#include "Component/CharactorComponent.hpp"
#include "Scene/game.h"
#include "Math/MyMath_T.h"
#include "Component/Player/PlayerSoundVolumeComponent.hpp"
#include "Component/Enemy/EnemySoundListenerComponent.hpp"
#include "Component/ChildComponent.hpp"
#include "Component/Shader/VisibleSineCurveComponent.hpp"
#include "Component/Components.hpp"

// 名前空間
using namespace Tag;
using namespace SequenceTag;