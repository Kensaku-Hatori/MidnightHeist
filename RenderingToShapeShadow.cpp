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

// 名前空間
using namespace Tag;

//*********************************************
// 描画
//*********************************************
void RenderingToShapeShadowSystem::Rendering(entt::registry& reg)
{
	// エンテティのリストを取得
	auto view = reg.view<CastShapeShadow>();

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	CShapeShadow::Instance()->Begin();
	CShapeShadow::Instance()->BeginObject();
	CShapeShadow::Instance()->BeginPass();

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

		for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.Tex.size(); nCntMat++)
		{
			CShapeShadow::Instance()->SetParameters(mtxWorld);

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