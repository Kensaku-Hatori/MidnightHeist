//*************************************
//
//　2Dポリゴン処理[Object2D.cpp]
//　Author:Hatori Kensaku
//
//*************************************

// インクルード
#include "Object2D.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "shadowmap.h"

//*********************************************
// コンストラクタ
//*********************************************
CObject2D::CObject2D(int Priority) : CObject(Priority)
{
	// メンバ変数をNULにする
}

//*********************************************
// デストラクタ
//*********************************************
CObject2D::~CObject2D()
{

}

//*********************************************
// 初期化処理
//*********************************************
HRESULT CObject2D::Init()
{
	//CManager::GetScene()->GetReg().emplace<Object2DComponent>(m_Entity);
	//CManager::GetScene()->GetReg().emplace<Render2DComponent>(m_Entity);

	//// デバイスを取得
	//CRenderer *pRenderer;
	//pRenderer = CManager::GetRenderer();
	//LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//Render2DComponent& myRender2DCom = CManager::GetScene()->GetReg().get<Render2DComponent>(m_Entity);

	////頂点バッファの生成
	//pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
	//	D3DUSAGE_WRITEONLY,
	//	FVF_VERTEX_2D,
	//	D3DPOOL_MANAGED,
	//	&myRender2DCom.VtxComp.pVertex,
	//	NULL);

	//// 頂点バッファをロック
	//VERTEX_2D* pVtx = NULL;
	//myRender2DCom.VtxComp.pVertex->Lock(0, 0, (void**)&pVtx, 0);

	//if (pVtx != NULL)
	//{
	//	pVtx[0].pos = VEC3_NULL;
	//	pVtx[1].pos = VEC3_NULL;
	//	pVtx[2].pos = VEC3_NULL;
	//	pVtx[3].pos = VEC3_NULL;

	//	pVtx[0].col = WHITE;
	//	pVtx[1].col = WHITE;
	//	pVtx[2].col = WHITE;
	//	pVtx[3].col = WHITE;

	//	pVtx[0].rhw = 1.0f;
	//	pVtx[1].rhw = 1.0f;
	//	pVtx[2].rhw = 1.0f;
	//	pVtx[3].rhw = 1.0f;

	//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//}

	//// 頂点バッファをアンロック
	//myRender2DCom.VtxComp.pVertex->Unlock();

	return S_OK;
}

//*********************************************
// 終了処理
//*********************************************
void CObject2D::Uninit()
{
	//Render2DComponent& myRender2DCom = CManager::GetScene()->GetReg().get<Render2DComponent>(m_Entity);

	//// 頂点バッファの破棄
	//if (myRender2DCom.VtxComp.pVertex != NULL)
	//{
	//	myRender2DCom.VtxComp.pVertex->Release();
	//	myRender2DCom.VtxComp.pVertex = NULL;
	//}
	//// メモリの解放
	//Release();
}

//*********************************************
// 更新処理
//*********************************************
void CObject2D::Update()
{
	//Transform myTrans = CManager::GetScene()->GetReg().get<Transform>(m_Entity);
	//Object2DComponent& my2DCom = CManager::GetScene()->GetReg().get<Object2DComponent>(m_Entity);
	//Render2DComponent& myRender2DCom = CManager::GetScene()->GetReg().get<Render2DComponent>(m_Entity);

	//VERTEX_2D* pVtx = NULL;

	//if (myRender2DCom.VtxComp.pVertex != NULL)
	//{
	//	myRender2DCom.VtxComp.pVertex->Lock(0, 0, (void**)&pVtx, 0);
	//}
	//else return;

	//if (pVtx != NULL)
	//{
	//	//頂点座標の更新
	//	pVtx[0].pos.x = myTrans.Pos.x - my2DCom.Size.x;
	//	pVtx[0].pos.y = myTrans.Pos.y - my2DCom.Size.y;
	//	pVtx[0].pos.z = 0.0f;
	//	pVtx[1].pos.x = myTrans.Pos.x + my2DCom.Size.x;
	//	pVtx[1].pos.y = myTrans.Pos.y - my2DCom.Size.y;
	//	pVtx[1].pos.z = 0.0f;
	//	pVtx[2].pos.x = myTrans.Pos.x - my2DCom.Size.x;
	//	pVtx[2].pos.y = myTrans.Pos.y + my2DCom.Size.y;
	//	pVtx[2].pos.z = 0.0f;
	//	pVtx[3].pos.x = myTrans.Pos.x + my2DCom.Size.x;
	//	pVtx[3].pos.y = myTrans.Pos.y + my2DCom.Size.y;
	//	pVtx[3].pos.z = 0.0f;

	//	pVtx[0].rhw = 1.0f;
	//	pVtx[1].rhw = 1.0f;
	//	pVtx[2].rhw = 1.0f;
	//	pVtx[3].rhw = 1.0f;

	//	pVtx[0].col = my2DCom.Color;
	//	pVtx[1].col = my2DCom.Color;
	//	pVtx[2].col = my2DCom.Color;
	//	pVtx[3].col = my2DCom.Color;

	//	pVtx[0].tex = my2DCom.UV[0];
	//	pVtx[1].tex = my2DCom.UV[1];
	//	pVtx[2].tex = my2DCom.UV[2];
	//	pVtx[3].tex = my2DCom.UV[3];
	//}

	//myRender2DCom.VtxComp.pVertex->Unlock();
}

//*********************************************
// ポリゴンの位置を設定
//*********************************************
void CObject2D::SetPosition(D3DXVECTOR3 Pos)
{
	//Transform& myTrans = CManager::GetScene()->GetReg().get<Transform>(m_Entity);
	//myTrans.Pos = Pos;
}

//*********************************************
// ポリゴンの向きを設定
//*********************************************
void CObject2D::SetRotasion(const D3DXQUATERNION Quat)
{
	//Transform& myTrans = CManager::GetScene()->GetReg().get<Transform>(m_Entity);
	//myTrans.Quat = Quat;
}

//*********************************************
// ポリゴンの大きさを設定
//*********************************************
void CObject2D::SetSize(const D3DXVECTOR2 Size)
{
	//Object2DComponent& my2DCom = CManager::GetScene()->GetReg().get<Object2DComponent>(m_Entity);
	//my2DCom.Size = Size;
}

//*********************************************
// ポリゴンのUV座標を設定
//*********************************************
void CObject2D::SetUv(const D3DXVECTOR2 Uv, const float SizeU, const float SizeV)
{
	//// UvがカウントでSizeがテクスチャ座標の大きさ
	//Object2DComponent& my2DCom = CManager::GetScene()->GetReg().get<Object2DComponent>(m_Entity);
	//my2DCom.UV[0] = D3DXVECTOR2(SizeU * Uv.x, SizeV * Uv.y);
	//my2DCom.UV[1] = D3DXVECTOR2(SizeU + (Uv.x * SizeU), SizeV * Uv.y);
	//my2DCom.UV[2] = D3DXVECTOR2(SizeU * Uv.x, SizeV + (Uv.y * SizeV));
	//my2DCom.UV[3] = D3DXVECTOR2(SizeU + (Uv.x * SizeU), SizeV + (Uv.y * SizeV));
}

//*********************************************
// ポリゴンの色を設定
//*********************************************
void CObject2D::SetCol(const D3DXCOLOR Col)
{
	//Object2DComponent& my2DCom = CManager::GetScene()->GetReg().get<Object2DComponent>(m_Entity);
	//my2DCom.Color = Col;
}

//*********************************************
// テクスチャインデックスを設定
//*********************************************
void CObject2D::SetTexIndx(const int Indx)
{
	//Render2DComponent& myRender2DCom = CManager::GetScene()->GetReg().get<Render2DComponent>(m_Entity);
	//myRender2DCom.TexIndex = Indx;
}

//*********************************************
// 描画処理
//*********************************************
void CObject2D::Draw()
{
//	CRenderer* pRenderer;
//	pRenderer = CManager::GetRenderer();
//	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
//
//	//頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	// 頂点バッファをデータストリームに設定
//	pDevice->SetStreamSource(0, m_Registry.get<Object2DComponent>(m_Entity).m_pVertex, 0, sizeof(VERTEX_2D));
//
//	// テクスチャの設定
//	pDevice->SetTexture(0, CShadowMap::Instance()->GetTex());
//
//	// 描画
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//*********************************************
// 2Dポリゴンを作る
//*********************************************
CObject2D* CObject2D::Create(const D3DXVECTOR3 Pos, const D3DXQUATERNION Quat)
{
	CObject2D* pObject2D;
	pObject2D = new CObject2D;
	pObject2D->SetPosition(Pos);
	pObject2D->SetRotasion(Quat);
	pObject2D->SetCol(WHITE);
	pObject2D->Init();
	return pObject2D;
}