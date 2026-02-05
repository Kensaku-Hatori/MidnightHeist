//****************************************************************
//
// ポーズメニュー描画システム処理[RenderingPauseMenu.h]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingPauseMenu.h"
#include "TagComp.hpp"
#include "manager.h"
#include "texmanager.h"
#include "VertexRenderingComponent.hpp"
#include "TextureRenderingComponent.h"
#include "LayerManager.hpp"
#include "ParentComponent.hpp"
#include "SystemManager.h"

// 名前空間
using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderingPauseMenuSystem::Rendering(entt::registry& reg)
{
	// ポーズフラグが無効なら
	if (CSystemManager::IsPause() == false) return;

	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// エンテティのリストを取得
	auto Entitys = LayerManager::GetSortEntityList<LayerComponent,PauseMenu>(reg);

	// アクセス
	for (auto List : Entitys)
	{
		// コンポーネント取得
		auto& VtxComp = reg.get<VertexComponent>(List);
		auto& TextureComp = reg.get<TexComponent>(List);

		// 頂点バッファが生成されていなかったら
		if (VtxComp.pVertex == nullptr) continue;

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, VtxComp.pVertex, 0, sizeof(VERTEX_2D));
		// テクスチャの設定
		pDevice->SetTexture(0, CLoadTexture::GetTex(TextureComp.FilePath));
		// 描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}