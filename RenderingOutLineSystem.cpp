//****************************************************************
//
// シャドウマップへの書き込みシステムの処理[RenderingToShadomapSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingOutLineSystem.h"
#include "OutLineComp.hpp"
#include "TagComp.hpp"
#include "TransformComponent.hpp"
#include "XRenderingComponent.hpp"
#include "toon.h"
#include "manager.h"

// 名前空間
using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderingOutLineSystem::Rendering(entt::registry& reg)
{
	// エンテティのリストを取得
	auto view = reg.view<RenderingOutLine>();

	// シェーダー起動
	CToon::Instance()->Begin();
	CToon::Instance()->BeginPass(2);

	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ビューマトリックスとプロジェクションマトリックスを取得
	D3DXMATRIX View, Proj;
	pDevice->GetTransform(D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

	for (auto Entity : view)
	{
		auto& TransformCmp = reg.get<Transform3D>(Entity);
		auto& RenderingCmp = reg.get<XRenderingComp>(Entity);
		auto& OutLineCmp = reg.get<OutLineComp>(Entity);

		// モデルへのインデックスが-1だったら終わる
		if (RenderingCmp.FilePath.empty() == true) return;

		D3DMATERIAL9 matDef;								// 現在のマテリアルの保存用
		D3DXMATERIAL* pMat;									// マテリアルへのポインタ

		// モデルmanagerからインデックスを指定して取得
		CModelManager::MapObject modelinfo = RenderingCmp.Info;

		// 現在のマテリアルの取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.Tex.size(); nCntMat++)
		{
			// アウトライン用のパラメータ設定
			CToon::Instance()->SetUseOutLineParameters(TransformCmp.mtxWorld, View, Proj, OutLineCmp.Thickness, OutLineCmp.Color, OutLineCmp.Height);

			// モデル(パーツ)の描画
			modelinfo.modelinfo.pSmoothMesh->DrawSubset(nCntMat);
		}

		// マテリアルを元に戻す
		pDevice->SetMaterial(&matDef);
	}
	// シェーダー終了
	CToon::Instance()->EndPass();
	CToon::Instance()->End();
}