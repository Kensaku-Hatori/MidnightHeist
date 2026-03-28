//****************************************************************
//
// ファクトリに使うインクルードをまとめたもの[FactorieUtils.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "TagComp.hpp"
#include "Component/TransformComponent.hpp"
#include "Component/VelocityComponent.hpp"
#include "Component/Polygone/VertexRenderingComponent.hpp"
#include "Component/Polygone/TextureRenderingComponent.h"
#include "Component/SizeComponent.hpp"
#include "Component/Polygone/ColorComponent.hpp"
#include "Component/Polygone/UVComponent.hpp"
#include "Component/Polygone/NormalComponent.hpp"
#include "Component/XRenderingComponent.hpp"
#include "Component/Polygone/LayerComponent.hpp"
#include "Factories.h"
#include "Bace/manager.h"
#include "Math/MyMath.h"
#include "Mesh.hpp"
#include "Component/ParentComponent.hpp"
#include "Component/AIComponent.hpp"
#include "Component/Polygone/Select2DComponent.hpp"
#include "Component/Polygone/Menu2DComponent.hpp"
#include "Component/Enemy/EnemyPatrolPointComponent.hpp"
#include "Component/FanInfoComponent.hpp"
#include "TitleUI.hpp"
#include "PauseUI.hpp"
#include "Component/Enemy/EnemyAIAStarComponent.hpp"
#include "Component/Shader/SkyBoxComponent.hpp"
#include "Component/Enemy/EnemyManagerComponent.hpp"
#include "Component/Shader/OutLineComponent.hpp"
#include "Component/CapsuleComponent.hpp"
#include "Component/Player/PlayerAnimetionComponent.hpp"
#include "Component/LockOnUIAnimComponent.hpp"
#include "Component/ItemComponent.h"
#include "Component/Player/PlayerStateComponent.hpp"
#include "Component/Shader/UICircleComponent.hpp"
#include "Component/RenderFragComponent.hpp"
#include "Component/ItemManagerComponent.hpp"
#include "Component/Player/PlayerSoundVolumeComponent.hpp"
#include "Bace/Sound3D.h"
#include "Component/Shader/VisibleSineCurveComponent.hpp"
#include "Component/Enemy/EnemySoundListenerComponent.hpp"
#include "Component/ChildComponent.hpp"
#include "Component/CharactorComponent.hpp"
#include "Component/StutsComponent.h"
#include "Component/TimerComponent.hpp"
#include "Component/StutsComponent.h"
#include "Component/GateManagerComponent.hpp"
#include "Component/Components.hpp"
#include "btCollisionConfig.hpp"