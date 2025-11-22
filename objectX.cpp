//*************************************
//
//　モデル処理[ObjectX.cpp]
//　Author:Hatori Kensaku
//
//*************************************

// インクルード
#include "objectX.h"
#include "modelmanager.h"
#include "manager.h"
#include "toon.h"
#include "shadowmap.h"
#include "shapeshadow.h"

//***************************************
// コンストラクタ
//***************************************
CObjectX::CObjectX(const int Priority) : CObject(Priority)
{
	m_Pos = VEC3_NULL;
	m_Rot = VEC3_NULL;
	m_Scale = VEC3_NULL;
	m_pMtxParent = NULL;
	m_fAlpha = 1.0f;
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxRot);
}

//***************************************
// デストラクタ
//***************************************
CObjectX::~CObjectX()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CObjectX::Init(void)
{
	m_Scale = { 1.0f,1.0f,1.0f };
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CObjectX::Uninit(void)
{
	Release();
}

//***************************************
// 更新処理
//***************************************
void CObjectX::Update(void)
{
}

//***************************************
// 描画処理
//***************************************
void CObjectX::Draw(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans,mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;							// マテリアルへのポインタ

	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(m_FilePath);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_pMtxParent != NULL)
	{
		m_mtxWorld = *m_pMtxParent * m_mtxWorld;
	}

	if (m_mtxRot != NULL)
	{
		m_mtxWorld = m_mtxRot * m_mtxWorld;
	}

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

	D3DXMATRIX View, Proj;
	D3DXVECTOR4 Light = { -0.5f,0.5f,0.5f,0.0f };
	pDevice->GetTransform(D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

	CToon::Instance()->Begin();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.TexPath.size(); nCntMat++)
	{
		CToon::Instance()->SetParameters(m_mtxWorld, View, Proj, Light);

		CToon::Instance()->BeginPass(2);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pSmoothMesh->DrawSubset(nCntMat);

		CToon::Instance()->EndPass();
	}

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL col = pMat[nCntMat];

		D3DXVECTOR4 SettCol = { col.MatD3D.Diffuse.r,col.MatD3D.Diffuse.g,col.MatD3D.Diffuse.b,col.MatD3D.Diffuse.a };

		CToon::Instance()->SetParameters(m_mtxWorld, View, Proj, Light, SettCol,CShadowMap::Instance()->GetTex(), NULL,CShadowMap::Instance()->GetLightView(), CShadowMap::Instance()->GetLightProj());

		if (col.pTextureFilename == NULL)
		{
			CToon::Instance()->BeginPass(0);
		}
		else
		{
			CToon::Instance()->BeginPass(1);
		}

		// テクスチャの設定
		pDevice->SetTexture(0, CLoadTexture::GetTex(modelinfo.modelinfo.TexPath[nCntMat]));

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CToon::Instance()->EndPass();
	}
	CToon::Instance()->End();

	pDevice->SetMaterial(&matDef);
}

void CObjectX::DrawShape(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用

	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(m_FilePath);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_pMtxParent != NULL)
	{
		m_mtxWorld = *m_pMtxParent * m_mtxWorld;
	}

	if (m_mtxRot != NULL)
	{
		m_mtxWorld = m_mtxRot * m_mtxWorld;
	}

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.TexPath.size(); nCntMat++)
	{
		CShapeShadow::Instance()->SetParameters(m_mtxWorld);

		CShapeShadow::Instance()->BeginPass(0);

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CShapeShadow::Instance()->EndPass();
	}

	pDevice->SetMaterial(&matDef);
}

void CObjectX::CastShadow(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;							// マテリアルへのポインタ

	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(m_FilePath);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_pMtxParent != NULL)
	{
		m_mtxWorld = *m_pMtxParent * m_mtxWorld;
	}

	if (m_mtxRot != NULL)
	{
		m_mtxWorld = m_mtxRot * m_mtxWorld;
	}

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

	D3DXMATRIX View, Proj;
	D3DXVECTOR4 Light = { -0.5f,0.5f,0.5f,0.0f };
	pDevice->GetTransform(D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

	CToon::Instance()->Begin();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL col = pMat[nCntMat];

		D3DXVECTOR4 SettCol = { col.MatD3D.Diffuse.r,col.MatD3D.Diffuse.g,col.MatD3D.Diffuse.b,col.MatD3D.Diffuse.a };

		CToon::Instance()->SetParameters(m_mtxWorld, View, Proj, Light, SettCol, CShadowMap::Instance()->GetTex(), NULL,CShadowMap::Instance()->GetLightView(), CShadowMap::Instance()->GetLightProj());

		if (col.pTextureFilename == NULL)
		{
			CToon::Instance()->BeginPass(0);
		}
		else
		{
			CToon::Instance()->BeginPass(1);
		}

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CToon::Instance()->EndPass();
	}

	CToon::Instance()->End();

	pDevice->SetMaterial(&matDef);

}

void CObjectX::DrawShadow(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用

	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(m_FilePath);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_pMtxParent != NULL)
	{
		m_mtxWorld = *m_pMtxParent * m_mtxWorld;
	}

	if (m_mtxRot != NULL)
	{
		m_mtxWorld = m_mtxRot * m_mtxWorld;
	}

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.TexPath.size(); nCntMat++)
	{
		CShadowMap::Instance()->SetParameters(m_mtxWorld);

		CShadowMap::Instance()->BeginPass(0);

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CShadowMap::Instance()->EndPass();
	}

	pDevice->SetMaterial(&matDef);
}

void CObjectX::DrawOutline(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;							// マテリアルへのポインタ

	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(m_FilePath);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_pMtxParent != NULL)
	{
		m_mtxWorld = *m_pMtxParent * m_mtxWorld;
	}

	if (m_mtxRot != NULL)
	{
		m_mtxWorld = m_mtxRot * m_mtxWorld;
	}

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

	D3DXMATRIX View, Proj;
	D3DXVECTOR4 Light = { -0.5f,0.5f,0.5f,0.0f };
	pDevice->GetTransform(D3DTS_VIEW, &View);
	pDevice->GetTransform(D3DTS_PROJECTION, &Proj);

	CToon::Instance()->Begin();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL col = pMat[nCntMat];

		D3DXVECTOR4 SettCol = { col.MatD3D.Diffuse.r,col.MatD3D.Diffuse.g,col.MatD3D.Diffuse.b,col.MatD3D.Diffuse.a };

		CToon::Instance()->SetParameters(m_mtxWorld, View, Proj, Light, SettCol, CShadowMap::Instance()->GetTex(), NULL,CShadowMap::Instance()->GetLightView(), CShadowMap::Instance()->GetLightProj());

		CToon::Instance()->BeginPass(2);

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);

		CToon::Instance()->EndPass();
	}

	CToon::Instance()->End();

	pDevice->SetMaterial(&matDef);
}

//***************************************
// 生成
//***************************************
CObjectX* CObjectX::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, std::string Path)
{
	CObjectX* pObjectX = NULL;
	// メモリ確保
	pObjectX = new CObjectX;
	pObjectX->SetPosition(Pos);
	pObjectX->SetRotasion(Rot);
	pObjectX->m_FilePath = Path;
	pObjectX->Init();
	return pObjectX;
}