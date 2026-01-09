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
#include "EnemyAIComponent.hpp"
#include "TransformComponent.hpp"
#include "RigitBodyComponent.hpp"
#include "Velocity.hpp"
#include "EnemyPatrolPointComp.hpp"
#include "FanInfoComponent.hpp"
#include "ParentComponent.hpp"
//#include "LaserCollisionFragComp.hpp"
#include "Factories.h"
#include "mapmanager.h"
#include "math.h"
#include "CharactorComp.hpp"
#include "game.h"
#include "math_T.h"
#include "PlayerSoundVolumeComp.hpp"
#include "EnemySoundListener.hpp"
#include "ChildComp.hpp"
#include "VisibleSineCurveComp.hpp"
#include "SingleCollisionShapeComponent.hpp"

// 名前空間
using namespace Tag;
using namespace SequenceTag;