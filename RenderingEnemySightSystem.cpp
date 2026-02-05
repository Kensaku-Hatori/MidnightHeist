//****************************************************************
//
// 敵の視界描画システムの処理[RenderingEnemySightSystem.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingEnemySightSystem.h"
#include "TagComp.hpp"
#include "TransformComponent.hpp"
#include "VertexRenderingComponent.hpp"
#include "TextureRenderingComponent.h"
#include "manager.h"

// 名前空間
using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderingEnemySightSystem::Rendering(entt::registry& reg)
{
	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// エンテティのリストを取得
	auto view = reg.view<SightFan>();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネント取得
		auto& TransformComp = reg.get<Transform3D>(entity);
		auto& VtxComp = reg.get<VertexComponent>(entity);
		auto& TextureComp = reg.get<TexComponent>(entity);

		// テクスチャを設定
		pDevice->SetTexture(0, TextureComp.Tex);
		// バイアス設定
		float bias = 0.000005f + (0.0000001f * static_cast<int>(entity));
		pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&bias);
		// 頂点バッファをデバイスからデータストリームに設定
		pDevice->SetStreamSource(0, VtxComp.pVertex, 0, sizeof(VERTEX_3D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &TransformComp.mtxWorld);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 1);
	}
}