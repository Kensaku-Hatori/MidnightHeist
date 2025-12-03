//****************************************************************
//
// ファクトリ[Factories.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 通常のファクトリー
namespace Factories {
	entt::entity makeObject2D(entt::registry& Reg, const int Layer = 3, const std::string& Path = {}, D3DXVECTOR2 Pos = VEC2_NULL, D3DXVECTOR2 Size = { 100.0f,100.0f });
	entt::entity makeObject3D(entt::registry& Reg);
	entt::entity makeObjectX(entt::registry& Reg, const std::string& Path);

	entt::entity makeTitleManager(entt::registry& Reg);
	void InitTitleManager(entt::registry& Reg);

	entt::entity makePlayer(entt::registry& Reg);
	entt::entity makeEnemy(entt::registry& Reg);
	entt::entity makeMapobject(entt::registry& Reg, const std::string& Path, const D3DXVECTOR3& Pos = VEC3_NULL, const D3DXQUATERNION& Quat = QUAT_NULL, const D3DXVECTOR3& Scale = { 1.0f,1.0f,1.0f });
}

// メッシュ系のファクトリー
namespace MeshFactories {
	entt::entity makeMeshField(entt::registry& Reg, const int DivH, const int DivV, const D3DXVECTOR2& Size);
	HRESULT InitMeshField(entt::registry& Reg, const entt::entity& Entity);

	entt::entity makeLaser(entt::registry& Reg, entt::entity Parent = entt::null);
	HRESULT InitLaserMesh(entt::registry& Reg, const entt::entity& Entity);

	entt::entity makePatrolPointFromFile(entt::registry& Reg, std::string Path);
	HRESULT InitPatrolPointFromFile(entt::registry& Reg, const entt::entity& Entity);
}