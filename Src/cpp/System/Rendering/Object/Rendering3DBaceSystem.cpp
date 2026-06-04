//****************************************************************
//
// 3D描画基本システムのクラスの処理[3Drenderingbace.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "System/Rendering/Obj/Rendering3DBaseSystem.h"
#include "Bace/manager.h"
#include "Component/TransformComponent.hpp"
#include "Component/Polygone/VertexRenderingComponent.hpp"
#include "Component/Polygone/TextureRenderingComponent.h"
#include "TagComp.hpp"
#include "Manager/texmanager.h"
#include "System/LayerManager.hpp"

using namespace Tag;

//*********************************************
// 描画
//*********************************************
void Render3DSystem::Rendering(entt::registry& Reg)
{
	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// エンテティのリストを取得
	auto view = Reg.view<Object3D>();

	// アクセス
	for (auto entity : view)
	{
		// コンポーネント取得
		auto& TransformComp = Reg.get<Transform3D>(entity);
		auto& VtxComp = Reg.get<VertexComponent>(entity);
		auto& TextureComp = Reg.get<TexComponent>(entity);

		// テクスチャを設定
		pDevice->SetTexture(0, TextureComp.Tex);
		// 頂点バッファをデバイスからデータストリームに設定
		pDevice->SetStreamSource(0, VtxComp.pVertex, 0, sizeof(VERTEX_3D));
		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &TransformComp.mtxWorld);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}