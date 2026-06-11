//================================================================
//
// スカイボックスの描画システムの処理[RenderingSkyBoxSystem.cpp]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// インクルード
//****************************************************************
#include "System/Rendering/Shader/RenderingSkyBoxSystem.h"
#include "TagComp.hpp"
#include "Bace/manager.h"
#include "Shader/defaultcubemap.h"
#include "Component/Polygone/VertexRenderingComponent.hpp"
#include "Component/Polygone/IndexBufferComponent.hpp"
#include "Component/Shader/SkyBoxComponent.hpp"
#include "Shader/toon.h"

//****************************************************************
// 名前空間
//****************************************************************
using namespace Tag;

//****************************************************************
// 描画
//****************************************************************
void RenderingSkyBoxSystem::Rendering(entt::registry& Reg)
{
	// エンテティのリストを取得
	auto view = Reg.view<SkyBoxComponent>();

	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// キューブマップ用のシェーダーを設定
	CDefaultCubemap::Instance().Begin();
	CDefaultCubemap::Instance().BeginPass(0);

	// アクセス
	for (auto Entity : view)
	{
		// 情報を取得
		auto& VertexCmp = Reg.get<VertexComponent>(Entity);
		auto& IdxBuffCmp = Reg.get<IndexBufferComponent>(Entity);
		auto& SkyBoxCmp = Reg.get<SkyBoxComponent>(Entity);

		// 回転
		SkyBoxCmp.Angle += D3DXToRadian(1.0f / 60.0f);

		// マトリックス
		D3DXMATRIX mtxWorld, mtxTrans,mtxRot;
		D3DXMatrixIdentity(&mtxWorld);

		// カメラの位置を参照する
		D3DXVECTOR3 CamPos = CManager::GetCamera()->GetPosV();

		// マトリックスを計算
		D3DXMatrixRotationY(&mtxRot, SkyBoxCmp.Angle);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
		D3DXMatrixTranslation(&mtxTrans, CamPos.x, CamPos.y, CamPos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// ライトの影響を受けない
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, VertexCmp.pVertex, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(IdxBuffCmp.pIdx);
		// テクスチャの設定
		pDevice->SetTexture(0, nullptr);
		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		CDefaultCubemap::Instance().SetParameters(mtxWorld, SkyBoxCmp.pCubeTex);

		//メッシュキューブを描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		// ライトの影響を受けない
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
	// シェーダを終了
	CDefaultCubemap::Instance().EndPass();
	CDefaultCubemap::Instance().End();
}