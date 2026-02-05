//****************************************************************
//
// 物陰への書き込みシステムの処理[RenderingToShapeShadowSystem.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingToShapeShadow.h"
#include "TagComp.hpp"
#include "TransformComponent.hpp"
#include "XRenderingComponent.hpp"
#include "shapeshadow.h"
#include "manager.h"

// 名前空間
using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderingToShapeShadowSystem::Rendering(entt::registry& reg)
{
	// エンテティのリストを取得
	auto view = reg.view<CastShapeShadow>();

	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 物陰マップへの書き込みを開始
	CShapeShadow::Instance()->Begin();
	CShapeShadow::Instance()->BeginObject();
	CShapeShadow::Instance()->BeginPass();

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
			CShapeShadow::Instance()->SetParameters(TransformComp.mtxWorld);

			CShapeShadow::Instance()->BeginPass(0);

			// モデル(パーツ)の描画
			modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

			CShapeShadow::Instance()->EndPass();
		}

		pDevice->SetMaterial(&matDef);
	}
	CShapeShadow::Instance()->EndTexs();
	CShapeShadow::Instance()->EndPass();
	CShapeShadow::Instance()->End();
}