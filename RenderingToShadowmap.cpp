//****************************************************************
//
// シャドウマップへの書き込みシステムの処理[RenderingToShadomapSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingToShadowmap.h"
#include "TagComp.hpp"
#include "TransformComponent.hpp"
#include "XRenderingComponent.hpp"
#include "shadowmap.h"
#include "manager.h"

// 名前空間
using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderingToShadowmapSystem::Rendering(entt::registry& reg)
{
	// エンテティのリストを取得
	auto view = reg.view<CastShadow>();

	// シャドウマップへの書き込みを開始
	CShadowMap::Instance()->Begin();
	CShadowMap::Instance()->BeginPass();
	CShadowMap::Instance()->WriteMaps();

	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ビューマトリックスとプロジェクションマトリックスを取得
	D3DXMATRIX View, Proj;
	pDevice->GetTransform(D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

	// アクセス
	for (auto Entity : view)
	{
		// コンポーネントを取得
		auto& TransformComp = reg.get<Transform3D>(Entity);
		auto& RenderingComp = reg.get<XRenderingComponent>(Entity);

		// モデルへのインデックスが-1だったら終わる
		if (RenderingComp.FilePath.empty() == true) return;

		D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
		D3DXMATERIAL* pMat;							// マテリアルへのポインタ

		// モデルmanagerからインデックスを指定して取得
		CModelManager::MapObject modelinfo = RenderingComp.Info;

		// 現在のマテリアルの取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.Tex.size(); nCntMat++)
		{
			CShadowMap::Instance()->SetParameters(TransformComp.mtxWorld);

			// モデル(パーツ)の描画
			modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);
		}

		pDevice->SetMaterial(&matDef);
	}
	CShadowMap::Instance()->EndMaps();
	CShadowMap::Instance()->EndPass();
	CShadowMap::Instance()->End();
}