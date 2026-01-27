//****************************************************************
//
// 2D描画基本システムのクラスの処理[2Drenderingbace.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "Rendering2Dbace.h"
#include "TagComp.hpp"
#include "manager.h"
#include "texmanager.h"
#include "VertexRenderingComponent.hpp"
#include "TextureRenderingComponent.h"
#include "LayerManager.hpp"
#include "RenderFragComp.hpp"

using namespace Tag;

//*********************************************
// 描画
//*********************************************
void Render2DSystem::Rendering(entt::registry& reg)
{
	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 描画が整理されたリストを取得
	auto Entitys = LayerManager::GetSortEntityList<Object2DComponent>(reg);

	// アクセス
	for (auto List : Entitys)
	{
		// コンポーネントが存在したら
		if (reg.any_of<RenderFragComp>(List) == true)
		{
			// 描画フラグを確認
			if (reg.get<RenderFragComp>(List).IsRendering == false) continue;
		}
		// コンポーネント取得
		auto& VtxComp = reg.get<VertexComp>(List);
		auto& TextureComp = reg.get<TexComp>(List);

		// 頂点バッファが生成されていなかったら
		if (VtxComp.pVertex == nullptr) continue;

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, VtxComp.pVertex, 0, sizeof(VERTEX_2D));
		// テクスチャの設定
		pDevice->SetTexture(0, TextureComp.Tex);
		// 描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}