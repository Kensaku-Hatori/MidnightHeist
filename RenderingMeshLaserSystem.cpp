//****************************************************************
//
// メッシュフィールド描画システムの処理[RenderingMeshFieldsystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingMeshLaserSystem.h"
#include "Mesh.hpp"
#include "TagComp.hpp"
#include "VertexRenderingComponent.hpp"
#include "manager.h"
#include "ParentComponent.hpp"

using namespace Tag;

//*********************************************
// 更新
//*********************************************
void RenderMehLaerSystem::Rendering(entt::registry& reg)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	auto view = reg.view<LaserComponent>();

	// ライトの影響を受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (auto entity : view)
	{
		auto& TransformCmp = reg.get<Transform3D>(entity);
		auto& ParentCmp = reg.get<SingleParentComp>(entity);

		auto& MeshInfo = reg.get<MeshInfoComp>(entity);

		auto& VtxCmp = reg.get<VertexComp>(entity);
		auto& IdxBuffCmp = reg.get<IndexBufferComp>(entity);

		D3DXMATRIX mtxWorld = TransformCmp.GetMultiplyWorldMatrix(reg.get<Transform3D>(ParentCmp.Parent).GetWorldMatrix());

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
	// 設定を元に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}