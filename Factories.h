//****************************************************************
//
// ファクトリ[Factories.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "EnemyAIComponent.hpp"

// 通常のファクトリー
namespace Factories {
	entt::entity makeObject2D(entt::registry& Reg, const int Layer = 3, const std::string& Path = {}, D3DXVECTOR2 Pos = VEC2_NULL, D3DXVECTOR2 Size = { 100.0f,100.0f }, entt::entity Parent = entt::null);
	entt::entity makeObject3D(entt::registry& Reg);
	entt::entity makeObjectX(entt::registry& Reg, const std::string& Path);

	entt::entity make3DSightFan(entt::registry& Reg, entt::entity Parent = entt::null);
	void InitSightFan(entt::registry& Reg, entt::entity& Entity);

	entt::entity makeEmitterVolume(entt::registry& Reg, const D3DXCOLOR Col, entt::entity Parent = entt::null);

	entt::entity makeUICircle(entt::registry& Reg, entt::entity Parent = entt::null);
	void InitUICircle(entt::registry& Reg, entt::entity& Entity);

	entt::entity makeBacePlayer(entt::registry& Reg, const D3DXVECTOR3& Pos);
	void InitBacePlayer(entt::registry& Reg, entt::entity& Entity);
	void InitGamePlayer(entt::registry& Reg, entt::entity& Entity);
	void InitTitlePlayer(entt::registry& Reg, entt::entity& Entity);

	entt::entity makeNumber(entt::registry& Reg, const D3DXVECTOR2 Origin, const int Value, const D3DXVECTOR2 DigitSize, const D3DXVECTOR2 DigitOffset);
	void InitNumber(entt::registry& Reg, entt::entity& Timer);

	entt::entity makeEnemy(entt::registry& Reg,D3DXVECTOR3 Pos,std::vector<EnemyState::PatrolMap>& PointList);
	entt::entity makeMapobject(entt::registry& Reg, const std::string& Path, const D3DXVECTOR3& Pos = VEC3_NULL, const D3DXQUATERNION& Quat = QUAT_NULL, const D3DXVECTOR3& Scale = { 1.0f,1.0f,1.0f });
	void MappingModelPathToComponent(entt::registry& Reg, entt::entity& Entity, std::string Path);
}

// マネージャー系のファクトリ
namespace ManagerFactories {
	entt::entity makeTitleManager(entt::registry& Reg);
	void InitTitleManager(entt::registry& Reg);

	entt::entity makePauseManager(entt::registry& Reg);
	void InitPauseManager(entt::registry& Reg, entt::entity Parent);

	entt::entity makeEnemyManager(entt::registry& Reg);

	entt::entity makeItemManager(entt::registry& Reg);

	entt::entity makeStutsManager(entt::registry& Reg);
}
// メッシュ系のファクトリー
namespace MeshFactories {
	entt::entity makeMeshField(entt::registry& Reg, const int DivH, const int DivV, const D3DXVECTOR2& Size);
	HRESULT InitMeshField(entt::registry& Reg, const entt::entity& Entity);

	entt::entity makeLaser(entt::registry& Reg, entt::entity Parent = entt::null);
	HRESULT InitLaserMesh(entt::registry& Reg, const entt::entity& Entity);

	entt::entity makePatrolPointFromFile(entt::registry& Reg, std::string Path);

	entt::entity makeSkyBox(entt::registry& Reg);
	HRESULT InitMeshCube(entt::registry& Reg, const entt::entity& Entity);
}