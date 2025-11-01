//*************************************
//
//　ビルボード処理[ObjectBillBoard.cpp]
//　Author:Hatori Kensaku
//
//*************************************

// インクルード
#include "objectbillboard.h"

//***************************************
// コンストラクタ
//***************************************
CObjectBillBoard::CObjectBillBoard(const int Priority) : CObject(Priority)
{
	m_pVertex = NULL;
	m_nTexIndx = NULL;
	m_Size = VEC2_NULL;
	m_Pos = VEC3_NULL;
}

//***************************************
// デストラクタ
//***************************************
CObjectBillBoard::~CObjectBillBoard()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CObjectBillBoard::Init(void)
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
		pVtx[0].pos = D3DXVECTOR3(-m_Size.x, m_Size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_Size.x, m_Size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_Size.x, -m_Size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_Size.x, -m_Size.y, 0.0f);

		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		// 各頂点の法線の設定（一以上）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロック
	m_pVertex->Unlock();

	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CObjectBillBoard::Uninit(void)
{
	if (m_pVertex != NULL)
	{
		m_pVertex->Release();
		m_pVertex = NULL;
	}
	Release();
}

//***************************************
// 更新処理
//***************************************
void CObjectBillBoard::Update(void)
{
	VERTEX_3D* pVtx = NULL;

	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		pVtx[0].pos = D3DXVECTOR3(-m_Size.x, m_Size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_Size.x, m_Size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_Size.x, -m_Size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_Size.x, -m_Size.y, 0.0f);

		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		// 各頂点の法線の設定（一以上）
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	m_pVertex->Unlock();
}

//***************************************
// 描画処理
//***************************************
void CObjectBillBoard::Draw(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックス取得
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// カメラの逆行列を設定
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	// 頂点バッファをデバイスからデータストリームに設定
	pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(VERTEX_3D));
	//// テクスチャの設定
	//pDevice->SetTexture(0, NULL);
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//***************************************
// 生成
//***************************************
CObjectBillBoard* CObjectBillBoard::Create(D3DXVECTOR3 Pos,D3DXVECTOR2 Size)
{
	CObjectBillBoard* pObjectBillBoard = NULL;
	// メモリ確保
	pObjectBillBoard = new CObjectBillBoard;
	pObjectBillBoard->SetPosition(Pos);
	pObjectBillBoard->SetSize(Size);
	pObjectBillBoard->Init();
	return pObjectBillBoard;
}