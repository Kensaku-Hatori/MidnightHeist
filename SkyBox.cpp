//****************************************************************
//
// スカイボックスの処理[SkyBox.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "SkyBox.h"
#include "manager.h"
#include "defaultcubemap.h"

//*************************************
// 初期化
//*************************************
HRESULT CSkyBox::Init(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 8,
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

	float Size = 50;

	pVtx[0].pos = D3DXVECTOR3(-Size, Size, -Size);
	pVtx[0].col = WHITE;

	pVtx[1].pos = D3DXVECTOR3(Size, Size, -Size);
	pVtx[1].col = WHITE;

	pVtx[2].pos = D3DXVECTOR3(Size, -Size, -Size);
	pVtx[2].col = WHITE;

	pVtx[3].pos = D3DXVECTOR3(-Size, -Size, -Size);
	pVtx[3].col = WHITE;

	pVtx[4].pos = D3DXVECTOR3(Size, Size, Size);
	pVtx[4].col = WHITE;

	pVtx[5].pos = D3DXVECTOR3(-Size, Size, Size);
	pVtx[5].col = WHITE;

	pVtx[6].pos = D3DXVECTOR3(-Size, -Size, Size);
	pVtx[6].col = WHITE;

	pVtx[7].pos = D3DXVECTOR3(Size, -Size, Size);
	pVtx[7].col = WHITE;

	//頂点バッファをアンロック　
	m_pVtxBuff->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 36,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// --- 前面 ---
	pIdx[0] = 0; pIdx[1] = 1; pIdx[2] = 2;
	pIdx[3] = 0; pIdx[4] = 2; pIdx[5] = 3;

	// --- 背面 ---
	pIdx[6] = 4; pIdx[7] = 5; pIdx[8] = 6;
	pIdx[9] = 4; pIdx[10] = 6; pIdx[11] = 7;

	// --- 左面 ---
	pIdx[12] = 5; pIdx[13] = 0; pIdx[14] = 3;
	pIdx[15] = 5; pIdx[16] = 3; pIdx[17] = 6;

	// --- 右面 ---
	pIdx[18] = 1; pIdx[19] = 4; pIdx[20] = 7;
	pIdx[21] = 1; pIdx[22] = 7; pIdx[23] = 2;

	// --- 上面 ---
	pIdx[24] = 5; pIdx[25] = 4; pIdx[26] = 1;
	pIdx[27] = 5; pIdx[28] = 1; pIdx[29] = 0;

	// --- 底面 ---
	pIdx[30] = 3; pIdx[31] = 2; pIdx[32] = 7;
	pIdx[33] = 3; pIdx[34] = 7; pIdx[35] = 6;

	// インデックスバッファのアンロック
	m_pIdxBuff->Unlock();

	Load();

	return S_OK;
}

//*************************************
// 終了処理
//*************************************
void CSkyBox::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
	if (m_pCubeTex != nullptr)
	{
		m_pCubeTex->Release();
		m_pCubeTex = nullptr;
	}
}

//*************************************
// 更新処理
//*************************************
void CSkyBox::Update(void)
{
}

//*************************************
// 描画処理
//*************************************
void CSkyBox::Draw(void)
{
	static D3DXVECTOR3 tset = {};
	tset.y += 0.0002f;

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxWorld, mtxTrans, mtxRot;
	D3DXMatrixIdentity(&mtxWorld);

	D3DXVECTOR3 CamPo = CManager::GetCamera()->GetPosV();
	D3DXMatrixRotationY(&mtxRot, tset.y);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	D3DXMatrixTranslation(&mtxTrans, CamPo.x, CamPo.y, CamPo.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ライトの影響を受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	CDefaultCubemap::Instance()->Begin();
	CDefaultCubemap::Instance()->SetParameters(mtxWorld, m_pCubeTex);
	CDefaultCubemap::Instance()->BeginPass(0);

	//メッシュ床を描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	CDefaultCubemap::Instance()->EndPass();
	CDefaultCubemap::Instance()->End();

	// ライトの影響を受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

//*************************************
// SKYBOXファイルを開く
//*************************************
void CSkyBox::Load(void)
{
	// テクスチャがまとめられているファイルを開く
	std::string Path;
	Path = "data\\SKYBOX";
	WIN32_FIND_DATA findFileData;
	HANDLE handle;
	handle = FindFirstFile((Path + "\\*.*").c_str(), &findFileData);

	SkyBoxPathList test = {};

	// ファイルの中を走査
	if (handle != INVALID_HANDLE_VALUE) {
		// 繰り返し
		do {
			// ファイル名が存在しなかったら
			if (strcmp(findFileData.cFileName, ".") == 0 ||
				strcmp(findFileData.cFileName, "..") == 0)
				continue;

			// 相対パスに変換
			std::string FullPath = Path + "\\" + findFileData.cFileName;

			// 面を判断
			ProcessSkyBox(FullPath, test);
		} while (FindNextFile(handle, &findFileData));

		// 閉じる
		FindClose(handle);
	}

	// まず1枚のテクスチャからサイズを取得
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(test.Faces[0].c_str(), &info);

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// １枚目のテクスチャの解像度でキューブマップを作成
	pDevice->CreateCubeTexture(
		info.Width,
		1,
		0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		&m_pCubeTex,
		NULL
	);

	// 面のテクスチャ
	LPDIRECT3DTEXTURE9 srcTex = nullptr;

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		// 一面の画像読み込み
		D3DXCreateTextureFromFile(pDevice, test.Faces[nCnt].c_str(), &srcTex);

		// サーフェイス取得
		IDirect3DSurface9* srcSurf = nullptr;
		srcTex->GetSurfaceLevel(0, &srcSurf);

		// フェイス取得
		IDirect3DSurface9* dstSurf = nullptr;
		m_pCubeTex->GetCubeMapSurface((D3DCUBEMAP_FACES)nCnt, 0, &dstSurf);

		// 流し込み
		D3DXLoadSurfaceFromSurface(
			dstSurf,
			NULL,
			NULL,
			srcSurf,
			NULL,
			NULL,
			D3DX_DEFAULT,
			0
		);

		// リソース開放
		srcSurf->Release();
		dstSurf->Release();
		srcTex->Release();
	}
}

//*************************************
// 受け取ったパスから面を判断
//*************************************
void CSkyBox::ProcessSkyBox(const std::string& FilePath, SkyBoxPathList& Out)
{
	if (FilePath.find("dds") == std::string::npos && FilePath.find("png") == std::string::npos && FilePath.find("jpg") == std::string::npos) return;
	if (FilePath.find("negx") != std::string::npos || FilePath.find("xneg") != std::string::npos || FilePath.find("lf") != std::string::npos || FilePath.find("left") != std::string::npos) Out.Faces[1] = FilePath;
	if (FilePath.find("posx") != std::string::npos || FilePath.find("xpos") != std::string::npos || FilePath.find("rt") != std::string::npos || FilePath.find("right") != std::string::npos) Out.Faces[0] = FilePath;
	if (FilePath.find("negy") != std::string::npos || FilePath.find("yneg") != std::string::npos || FilePath.find("dn") != std::string::npos || FilePath.find("down") != std::string::npos || FilePath.find("bottom") != std::string::npos) Out.Faces[3] = FilePath;
	if (FilePath.find("posy") != std::string::npos || FilePath.find("ypos") != std::string::npos || FilePath.find("up") != std::string::npos || FilePath.find("top") != std::string::npos) Out.Faces[2] = FilePath;
	if (FilePath.find("negz") != std::string::npos || FilePath.find("zneg") != std::string::npos || FilePath.find("bk") != std::string::npos || FilePath.find("back") != std::string::npos) Out.Faces[5] = FilePath;
	if (FilePath.find("posz") != std::string::npos || FilePath.find("zpos") != std::string::npos || FilePath.find("ft") != std::string::npos || FilePath.find("front") != std::string::npos) Out.Faces[4] = FilePath;
}