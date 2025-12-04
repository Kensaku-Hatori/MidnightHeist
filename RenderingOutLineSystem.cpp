//****************************************************************
//
// シャドウマップへの書き込みシステムの処理[RenderingToShadomapSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingOutLineSystem.h"
#include "TagComp.hpp"
#include "TransformComponent.hpp"
#include "XRenderingComponent.hpp"
#include "toon.h"

// 名前空間
using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderingOutLineSystem::Rendering(entt::registry& reg)
{
	// エンテティのリストを取得
	auto view = reg.view<RenderingOutLine>();

	CToon::Instance()->Begin();
	CToon::Instance()->BeginPass(2);

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX View, Proj;
	pDevice->GetTransform(D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

	for (auto Entity : view)
	{
		auto& TransformComp = reg.get<Transform3D>(Entity);
		auto& RenderingComp = reg.get<XRenderingComp>(Entity);

		// モデルへのインデックスが-1だったら終わる
		if (RenderingComp.FilePath.empty() == true) return;

		D3DXMATRIX mtxWorld;						// 計算用マトリックス
		D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
		D3DXMATERIAL* pMat;							// マテリアルへのポインタ

		// モデルmanagerからインデックスを指定して取得
		CModelManager::MapObject modelinfo = RenderingComp.Info;

		mtxWorld = TransformComp.GetWorldMatrix();

		// 現在のマテリアルの取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.TexPath.size(); nCntMat++)
		{
			CToon::Instance()->SetUseOutLineParameters(mtxWorld, View, Proj);

			// モデル(パーツ)の描画
			modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);
		}

		pDevice->SetMaterial(&matDef);
	}
	CToon::Instance()->EndPass();
	CToon::Instance()->End();
}