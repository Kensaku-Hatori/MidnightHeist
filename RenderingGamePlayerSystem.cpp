//****************************************************************
//
// プレイヤー描画システムの処理[RenderingPlayerSystem.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "RenderingGamePlayerSystem.h"
#include "XRenderingComponent.hpp"
#include "TagComp.hpp"
#include "shapeshadow.h"
#include "toon.h"
#include "shadowmap.h"
#include "manager.h"
#include "TransformComponent.hpp"

// 名前空間
using namespace Tag;
using namespace SequenceTag;

//*********************************************
// 描画
//*********************************************
void RenderingGamePlayerSystem::Rendering(entt::registry& reg)
{
	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DMATERIAL9 matDef;	// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;		// マテリアルへのポインタ

	// エンテティのリストを取得
	auto view = reg.view<Player,InGame>();

	//*********************************************
	// 物陰マップに書き込みを開始
	//*********************************************
	CShapeShadow::Instance()->Begin();
	CShapeShadow::Instance()->BeginScene();

	// アクセス
	for (auto [Entity] : view.each())
	{
		// 描画
		RenderingShape(reg, Entity);
	}

	// 書き込みを終了する
	CShapeShadow::Instance()->EndTexs();
	CShapeShadow::Instance()->End();

	//*********************************************
	// 通常描画
	//*********************************************
	// アクセス
	for (auto [entity] : view.each())
	{
		// 情報を取得
		auto& TransformComp = reg.get<Transform3D>(entity);
		auto& RenderingComp = reg.get<XRenderingComponent>(entity);

		// 現在のマテリアルの取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)RenderingComp.Info.modelinfo.pBuffMat->GetBufferPointer();

		// シェーダーに渡す用のマトリックス
		D3DXMATRIX View, Proj;
		pDevice->GetTransform(D3DTS_VIEW, &View);
		pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

		// シェーダー起動
		CToon::Instance()->Begin();

		// マテリアル分回す
		for (int nCntMat = 0; nCntMat < (int)RenderingComp.Info.modelinfo.dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// シェーダーに渡す用の色
			D3DXVECTOR4 SettCol = { pMat[nCntMat].MatD3D.Diffuse.r,pMat[nCntMat].MatD3D.Diffuse.g,pMat[nCntMat].MatD3D.Diffuse.b,pMat[nCntMat].MatD3D.Diffuse.a };
			// パラメータ設定
			CToon::Instance()->SetUseShadowMapParameters(TransformComp.mtxWorld, View, Proj, SettCol, CShadowMap::Instance()->GetTex(), RenderingComp.Info.modelinfo.Tex[nCntMat], CShadowMap::Instance()->GetLightView(), CShadowMap::Instance()->GetLightProj());

			// テクスチャパスがあるかどうか
			if (pMat[nCntMat].pTextureFilename == NULL)
			{
				CToon::Instance()->BeginPass(0);
			}
			else
			{
				CToon::Instance()->BeginPass(1);
			}
			// モデル(パーツ)の描画
			RenderingComp.Info.modelinfo.pMesh->DrawSubset(nCntMat);
			// シェーダー終了
			CToon::Instance()->EndPass();
		}
		CToon::Instance()->End();
		// 既存のマテリアルに戻す
		pDevice->SetMaterial(&matDef);
	}
}

//*********************************************
// プレイヤーのシルエット描画
//*********************************************
void RenderingGamePlayerSystem::RenderingShape(entt::registry& Reg, entt::entity Entity)
{
	auto& TransformComp = Reg.get<Transform3D>(Entity);
	auto& RenderingComp = Reg.get<XRenderingComponent>(Entity);

	// モデルへのインデックスが-1だったら終わる
	if (RenderingComp.FilePath.empty() == true) return;

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

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

		CShapeShadow::Instance()->BeginPass(1);

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CShapeShadow::Instance()->EndPass();
	}

	pDevice->SetMaterial(&matDef);
}