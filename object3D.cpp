//*************************************
//
//　3Dポリゴン処理[Object3D.cpp]
//　Author:Hatori Kensaku
//
//*************************************

// インクルード
#include "object3D.h"
#include "texmanager.h"

//*********************************************
// コンストラクタ
//*********************************************
CObject3D::CObject3D(int Priority) : CObject(Priority)
{
	m_pVertex = NULL;
	m_fWidth = NULL;
	m_fHeigth = NULL;
	m_Pos = VEC3_NULL;
	m_Rot = VEC3_NULL;
	m_RotDest = VEC3_NULL;
	m_Col = WHITE;
	m_UVMax = { 1.0f,1.0f };
	m_UVMin = { 0.0f,0.0f };
	m_nTexIndx = -1;
}

//*********************************************
// デストラクタ
//*********************************************
CObject3D::~CObject3D()
{
}

//*********************************************
// 初期化処理
//*********************************************
HRESULT CObject3D::Init(void)
{
	// デバイスを取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVertex,
		NULL);

	// 頂点バッファをロック
	VERTEX_3D* pVtx = NULL;
	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth,0.0f,m_fHeigth);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fHeigth);
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, -m_fHeigth);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, -m_fHeigth);

		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);

		pVtx[0].tex = D3DXVECTOR2(m_UVMin.x, m_UVMin.y);
		pVtx[1].tex = D3DXVECTOR2(m_UVMax.x, m_UVMin.y);
		pVtx[2].tex = D3DXVECTOR2(m_UVMin.x, m_UVMax.y);
		pVtx[3].tex = D3DXVECTOR2(m_UVMax.x, m_UVMax.y);
	}

	// 頂点バッファをアンロック
	m_pVertex->Unlock();

	return S_OK;
}

//*********************************************
// 終了処理
//*********************************************
void CObject3D::Uninit(void)
{
	if (m_pVertex != NULL)
	{
		m_pVertex->Release();
		m_pVertex = NULL;
	}
	// メモリの開放
	Release();
}

//*********************************************
// 更新処理
//*********************************************
void CObject3D::Update(void)
{
	// 頂点バッファをロック
	VERTEX_3D* pVtx = NULL;
	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth, 0.0f, m_fHeigth);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fHeigth);
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, -m_fHeigth);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, -m_fHeigth);

		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].tex = D3DXVECTOR2(m_UVMin.x, m_UVMin.y);
		pVtx[1].tex = D3DXVECTOR2(m_UVMax.x, m_UVMin.y);
		pVtx[2].tex = D3DXVECTOR2(m_UVMin.x, m_UVMax.y);
		pVtx[3].tex = D3DXVECTOR2(m_UVMax.x, m_UVMax.y);
	}

	// 頂点バッファをアンロック
	m_pVertex->Unlock();
}

//*********************************************
// 描画処理
//*********************************************
void CObject3D::Draw(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// テクスチャを設定
	pDevice->SetTexture(0, CLoadTexture::GetTex("data\\TEXTURE\\floor.jpg"));
	// 頂点バッファをデバイスからデータストリームに設定
	pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(VERTEX_3D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//*********************************************
// 生成
//*********************************************
CObject3D* CObject3D::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot)
{
	CObject3D* pObject3D = NULL;
	// メモリ確保
	pObject3D = new CObject3D;
	pObject3D->SetPosition(Pos);
	pObject3D->SetRotasion(Rot);
	pObject3D->SetSize(D3DXVECTOR2(2000.0f, 1000.0f));
	pObject3D->Init();
	return pObject3D;
}