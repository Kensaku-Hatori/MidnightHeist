//================================================================
//
// 2D描画基本システムのクラスの処理[2Drenderingbace.cpp]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// インクルード
//****************************************************************
#include "System/Rendering/Obj/Rendering2DBaseSystem.h"
#include "TagComp.hpp"
#include "Bace/manager.h"
#include "Manager/texmanager.h"
#include "Component/Polygone/VertexRenderingComponent.hpp"
#include "Component/Polygone/TextureRenderingComponent.h"
#include "System/LayerManager.hpp"
#include "Component/RenderFragComponent.hpp"

//****************************************************************
// 名前空間
//****************************************************************
using namespace Tag;

//****************************************************************
// 描画
//****************************************************************
void Render2DSystem::Rendering(entt::registry& Reg)
{
	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 描画が整理されたリストを取得
	auto Entitys = LayerManager::GetSortEntityList<Object2D>(Reg);

	// アクセス
	for (auto List : Entitys)
	{
		// コンポーネントが存在したら
		if (Reg.any_of<RenderFragComponent>(List) == true)
		{
			// 描画フラグを確認
			if (Reg.get<RenderFragComponent>(List).IsRendering == false) continue;
		}

		// コンポーネント取得
		auto& VtxComp = Reg.get<VertexComponent>(List);
		auto& TextureComp = Reg.get<TexComponent>(List);

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