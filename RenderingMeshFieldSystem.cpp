//****************************************************************
//
// メッシュフィールド描画システムの処理[RenderingMeshFieldsystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingMeshFieldSystem.h"
#include "Mesh.hpp"
#include "TagComp.hpp"
#include "VertexRenderingComponent.hpp"
#include "manager.h"

using namespace Tag;

//*********************************************
// 更新
//*********************************************
void RenderMehFieldSystem::Rendering(entt::registry& reg)
{
	auto view = reg.view<MeshFieldComponent>();

	for (auto entity : view)
	{
		auto& MeshInfo = reg.get<MeshInfoComp>(entity);

		auto& VtxCmp = reg.get<VertexComp>(entity);
		auto& IdxBuffCmp = reg.get<IndexBufferComp>(entity);

		CRenderer* pRenderer;
		pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		D3DXMATRIX mtxWorld = {};
		D3DXMatrixIdentity(&mtxWorld);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, VtxCmp.pVertex, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(IdxBuffCmp.pIdx);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//メッシュ床を描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MeshInfo.nNumVtx, 0, MeshInfo.nNumPoly);
	}
}