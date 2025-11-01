#include "manager.h"
#include "laser.h"

CLaser::CLaser()
{
	D3DXQuaternionIdentity(&m_Quat);
	m_fLength = 1000.0f;
	m_fRadius = 1.0f;
}

CLaser::~CLaser()
{
}

HRESULT CLaser::Init(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_nVertexNum = (m_nDivX + 1) * (m_nDivY + 1);					// 頂点数
	m_nPolyNum = (2 * m_nDivX * m_nDivY + (m_nDivY - 1) * 4);		// ポリゴン数
	m_nIndxBuffNum = m_nPolyNum + 2;								// インデックス

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertexNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	//インデックスへのポインタ
	WORD* pIdx = NULL;

	//頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtx = 0;

	// スタートの高さ
	float StartHeight = m_Origin.y;

	// 高さを下げる割合
	float HeightRatio = m_fLength / m_nDivY;

	// 角度の割合
	float fRatio = (D3DX_PI * 2) / m_nDivX;

	//頂点情報の設定
	for (int nCntZ = 0; nCntZ <= m_nDivY; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_nDivX; nCntX++)
		{
			//頂点座標の設定
			pVtx[nCntVtx].pos.x = sinf(fRatio * nCntX) * m_fRadius;
			pVtx[nCntVtx].pos.y = StartHeight + (HeightRatio * nCntZ);
			pVtx[nCntVtx].pos.z = cosf(fRatio * nCntX) * m_fRadius;

			// 法線を計算
			D3DXVECTOR3 CenterVec = D3DXVECTOR3(m_Origin.x - pVtx[nCntVtx].pos.x, 0.0f, m_Origin.z - pVtx[nCntVtx].pos.z);
			D3DXVec3Normalize(&CenterVec, &CenterVec);

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = CenterVec;

			//頂点カラーの設定
			pVtx[nCntVtx].col = RED;

			float TexX = 1.0f / (m_nDivX);
			float TexY = 1.0f / m_nDivY;

			//テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2((float)TexX * nCntX, (float)TexY * nCntZ);

			nCntVtx++;
		}
	}

	//頂点バッファをアンロック　
	m_pVtxBuff->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nIndxBuffNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	int nIdxA = m_nDivX + 1;	// 上側インデックス
	int nIdxB = 0;				// 今のインデックス
	int nCntIdx = 0;			// インデックスカウンター

	// インデックスを求める
	for (int nCntA = 0; nCntA < m_nDivY; nCntA++)
	{// Zの分割数分回す
		for (int nCntB = 0; nCntB <= m_nDivX; nCntB++)
		{// Xの分割数分回す
			pIdx[nCntIdx] = (WORD)nIdxA;
			pIdx[nCntIdx + 1] = (WORD)nIdxB;
			nCntIdx += 2;
			nIdxA++;
			nIdxB++;
		}
		if (nCntA < m_nDivY - 1)
		{// 安全装置
			pIdx[nCntIdx] = (WORD)nIdxB - 1;
			pIdx[nCntIdx + 1] = (WORD)nIdxA;
			nCntIdx += 2;
		}
	}
	// インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

void CLaser::Uninit(void)
{
	// インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	// 自分自身の破棄
	Release();
}

void CLaser::Update(void)
{
	m_Dir.x = m_mtxWorld._21;
	m_Dir.y = m_mtxWorld._22;
	m_Dir.z = m_mtxWorld._23;

	CDebugProc::print("Origin[%.2f,%.2f,%.2f]\n", m_Origin.x, m_Origin.y, m_Origin.z);
	CDebugProc::print("Dir[%.2f,%.2f,%.2f]\n", m_Dir.x, m_Dir.y, m_Dir.z);
	CDebugProc::print("Length[%.2f]\n", m_fLength);

	//頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntVtx = 0;

	// スタートの高さ
	float StartHeight = 0.0f;

	// 高さを下げる割合
	float HeightRatio = m_fLength / m_nDivY;

	// 角度の割合
	float fRatio = (D3DX_PI * 2) / m_nDivX;

	//頂点情報の設定
	for (int nCntZ = 0; nCntZ <= m_nDivY; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_nDivX; nCntX++)
		{
			//頂点座標の設定
			pVtx[nCntVtx].pos.x = sinf(fRatio * nCntX) * m_fRadius;
			pVtx[nCntVtx].pos.y = StartHeight + (HeightRatio * nCntZ);
			pVtx[nCntVtx].pos.z = cosf(fRatio * nCntX) * m_fRadius;

			// 法線を計算
			D3DXVECTOR3 CenterVec = D3DXVECTOR3(m_Origin.x - pVtx[nCntVtx].pos.x, 0.0f, m_Origin.z - pVtx[nCntVtx].pos.z);
			D3DXVec3Normalize(&CenterVec, &CenterVec);

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = CenterVec;

			//頂点カラーの設定
			pVtx[nCntVtx].col = RED;

			float TexX = 1.0f / (m_nDivX);
			float TexY = 1.0f / m_nDivY;

			//テクスチャ座標の設定
			pVtx[nCntVtx].tex = D3DXVECTOR2((float)TexX * nCntX, (float)TexY * nCntZ);

			nCntVtx++;
		}
	}

	//頂点バッファをアンロック　
	m_pVtxBuff->Unlock();
}

void CLaser::Draw(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// ALPHAテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ライトの影響を受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反転
	D3DXMatrixRotationQuaternion(&mtxRot, &m_Quat);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Origin.x, m_Origin.y, m_Origin.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//メッシュ床を描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nVertexNum, 0, m_nPolyNum);

	// 設定を元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CLaser::SetOrigin(D3DXVECTOR3 Pos)
{
	m_Origin = Pos;
}

void CLaser::SetFront(D3DXVECTOR3 XAxis)
{
	D3DXQuaternionRotationAxis(&m_Quat, &XAxis, -D3DX_PI * 0.5f);
}

void CLaser::SetHeight(const float Height)
{
	m_fLength = Height;
}

CLaser* CLaser::Create(const int DivX, const int DivY)
{
	CLaser* pLaser = NULL;
	pLaser = new CLaser;
	pLaser->m_nDivX = DivX;
	pLaser->m_nDivY = DivY;
	pLaser->Init();
	return pLaser;
}