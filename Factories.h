//****************************************************************
//
// ファクトリ[Factories.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include "SizeComponent.hpp"
#include "MeshInfoComponent.hpp"
#include "IndexBufferComponent.hpp"
#include "VertexRenderingComponent.hpp"

namespace Factories {
	entt::entity makeObject2D(entt::registry& Reg, const int Layer = 3, const std::string& Path = {});
	entt::entity makeObject3D(entt::registry& Reg);
	entt::entity makeObjectX(entt::registry& Reg);
	entt::entity makePlayer(entt::registry& Reg);
	entt::entity makeMapobject(entt::registry& Reg, const std::string& Path, const D3DXVECTOR3& Pos = VEC3_NULL, const D3DXQUATERNION& Quat = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), const D3DXVECTOR3& Scale = { 1.0f,1.0f,1.0f });
}

namespace MeshFactories {
	entt::entity makeMeshField(entt::registry& Reg, const int DivH, const int DivV, const D3DXVECTOR2& Size);
	HRESULT InitMeshField(entt::registry& Reg, const entt::entity& Entity);
}