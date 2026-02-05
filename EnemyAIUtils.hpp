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
#include "AIComponent.hpp"
#include "TransformComponent.hpp"
#include "Velocity.hpp"
#include "EnemyPatrolPointComponent.hpp"
#include "FanInfoComponent.hpp"
#include "ParentComponent.hpp"
#include "Factories.h"
#include "mapmanager.h"
#include "math.h"
#include "CharactorComponent.hpp"
#include "game.h"
#include "math_T.h"
#include "PlayerSoundVolumeComponent.hpp"
#include "EnemySoundListenerComponent.hpp"
#include "ChildComponent.hpp"
#include "VisibleSineCurveComponent.hpp"
#include "Components.hpp"

// 名前空間
using namespace Tag;
using namespace SequenceTag;