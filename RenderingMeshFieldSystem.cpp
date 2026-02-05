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
#include "TextureRenderingComponent.h"
#include "TransformComponent.hpp"
#include "manager.h"

// ネームスペース
using namespace Tag;

//*********************************************
// 更新
//*********************************************
void RenderMehFieldSystem::Rendering(entt::registry& reg)
{
	// ビュー生成
	auto view = reg.view<MeshField>();

	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネント取得
		auto& TransformCmp = reg.get<Transform3D>(entity);
		auto& MeshInfo = reg.get<MeshInfoComponent>(entity);
		auto& VtxCmp = reg.get<VertexComponent>(entity);
		auto& IdxBuffCmp = reg.get<IndexBufferComponent>(entity);
		auto& TexCmp = reg.get<TexComponent>(entity);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &TransformCmp.mtxWorld);
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, VtxCmp.pVertex, 0, sizeof(VERTEX_3D));
		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(IdxBuffCmp.pIdx);
		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		//テクスチャの設定
		pDevice->SetTexture(0, TexCmp.Tex);
		//メッシュ床を描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MeshInfo.nNumVtx, 0, MeshInfo.nNumPoly);
	}
}