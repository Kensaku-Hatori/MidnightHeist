//****************************************************************
//
// テクスチャを一括管理するクラスの処理[texmanager.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "texmanager.h"
#include "manager.h"
#include "objectX.h"
#include "modelmanager.h"

// 名前空間
using namespace std;

// 静的メンバ変数宣言
unordered_map<string, LPDIRECT3DTEXTURE9> CLoadTexture::m_TexMap = {};
std::unique_ptr<CXTexture> CXTexture::m_Instance = NULL;

//*********************************************
// コンストラクタ
//*********************************************
CLoadTexture::CLoadTexture()
{
}

//*********************************************
// デストラクタ
//*********************************************
CLoadTexture::~CLoadTexture()
{
}

//*********************************************
// テクスチャを外部ファイルから読みこむ
//*********************************************
void CLoadTexture::Load(std::string LoadFilePath)
{
	// ファイルを開く
	ifstream ifs(LoadFilePath);

	// 開けなかったら
	if (!ifs)
	{
		MessageBox(NULL, "ファイルが読み込めませんでした", "終了メッセージ", MB_OK);
		return;
	}

	// 行を読み取る用
	string line = {};
	// ラベル、イコールを読み込む用
	string label = {}, equal = {};

	while (getline(ifs, line))
	{
		string FilePath = {};

		istringstream iss(line);
		if (line.find("TEX_FILENAME") != string::npos)
		{
			iss >> label >> equal >> FilePath;
		}

		if (FilePath[0] != NULL)
		{
			GetTex(FilePath);
		}

		if (line.find("#") != string::npos)
		{
			continue;
		}
	}
}

//*********************************************
// テクスチャの取得処理
//*********************************************
LPDIRECT3DTEXTURE9& CLoadTexture::GetTex(string Path)
{
	// 検索結果を代入
	auto Find = m_TexMap.find(Path);

	// すでに存在したら
	if (Find != m_TexMap.end())return Find->second;

	// 被りがなくてパスの長さが短かったらクリエイト
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの情報を代入する一時変数
	LPDIRECT3DTEXTURE9 TexInfo = NULL;

	// 作成
	D3DXCreateTextureFromFile(
		pDevice,
		Path.c_str(),
		&TexInfo
	);

	// 代入
	m_TexMap[Path] = TexInfo;

	// 情報を返す
	return m_TexMap[Path];
}

////*********************************************
//// インデックスでテクスチャ取得
////*********************************************
//LPDIRECT3DTEXTURE9& CLoadTexture::GetTex(const int Idx)
//{
//	// NULLを返す
//	if (Idx == -1) return NULL;
//
//	// 不正なインデックスならアサート
//	if (Idx < 0 || Idx >= (int)m_TexMap.size()) assert(0 && "存在しないテクスチャにアクセスしようとしています");
//
//	// 配列でアクセスするためのイテレーター
//	auto Map = m_TexMap.begin();
//
//	// 要素を進める
//	std::advance(Map, Idx);
//
//	// Vlueを返す
//	return Map->second;
//}

//*********************************************
// テクスチャの破棄
//*********************************************
void CLoadTexture::UnRegistTex(void)
{
	// マップの中にアクセス
	for (auto& Map : m_TexMap) 
	{
		// nullptrチェック
		if (Map.second != nullptr)
		{
			// メモリ開放
			Map.second->Release();

			// nullptrを敷き詰める
			Map.second = nullptr;
		}
	}

	// マップ自体をクリア
	m_TexMap.clear();
}

//*********************************************
// デストラクタ
//*********************************************
CXTexture::~CXTexture()
{
	// マップの中にアクセス
	for (auto& Map : m_XTex)
	{
		// nullptrチェック
		if (Map.second.Tex != nullptr)
		{
			// メモリ開放
			Map.second.Tex->Release();

			// nullptrを敷き詰める
			Map.second.Tex = nullptr;
		}
	}

	// マップ自体をクリア
	m_XTex.clear();
}

//*********************************************
// 初期化処理
//*********************************************
HRESULT CXTexture::Init(void)
{
	return S_OK;
}

//*********************************************
// 終了処理
//*********************************************
void CXTexture::Uninit(void)
{
}

//*********************************************
// 更新処理
//*********************************************
void CXTexture::Update(void)
{
}

//*********************************************
// 描画処理
//*********************************************
void CXTexture::Draw(void)
{
	// カメラの作成に使う一時変数
	D3DXMATRIX ViewMtx, ProjectionMtx;

	// アスペクト比を計算
	float fAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	// デバイスの取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 昔のレンダー周りのバックアップ用変数
	D3DVIEWPORT9 ViewPortDef;
	D3DXMATRIX ViewMtxDef, ProjectionMtxDef;

	// 昔のやつを取得
	pDevice->GetRenderTarget(0, &m_CurrentRender);
	pDevice->GetDepthStencilSurface(&m_CurrentDepth);
	pDevice->GetViewport(&ViewPortDef);
	pDevice->GetTransform(D3DTS_VIEW, &ViewMtxDef);
	pDevice->GetTransform(D3DTS_PROJECTION, &ProjectionMtxDef);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&ViewMtx);

	// カメラの作成に必要な一時変数
	D3DXVECTOR3 V, R;

	// 始点を設定
	V = { 100.0f,100.0f,-200.0f };

	// 注視点を設定
	R = { 0.0f,0.0f,0.0f };

	// 上向きベクトルを設定
	D3DXVECTOR3 VecU = { 0.0f,1.0f,0.0f };

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&ViewMtx,
		&V,
		&R,
		&VecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &ViewMtx);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&ProjectionMtx);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&ProjectionMtx,
		D3DXToRadian(45),		// 視野角
		fAspect,				// アスペクト比
		10.0f,
		1000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &ProjectionMtx);

	for (auto ModelList = m_XTex.begin(); ModelList != m_XTex.end(); ModelList++)
	{
		// レンダータゲットを変える
		pDevice->SetRenderTarget(0, (*ModelList).second.Surface);

		// 震度バッファを変える
		pDevice->SetDepthStencilSurface((*ModelList).second.DepthBuffer);

		//画面クリア
		pDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			D3DCOLOR_RGBA(100, 100, 100, 255), 1.0f, 0);

		// オブジェクトの描画
		DrawObject((*ModelList).second.Path);
	}

	// 昔のやつに戻す
	pDevice->SetRenderTarget(0, m_CurrentRender);
	pDevice->SetDepthStencilSurface(m_CurrentDepth);
	pDevice->SetViewport(&ViewPortDef);
	pDevice->SetTransform(D3DTS_VIEW, &ViewMtxDef);
	pDevice->SetTransform(D3DTS_PROJECTION, &ProjectionMtxDef);

	// nullptrチェック
	if (m_CurrentDepth != nullptr)
	{
		// メモリ開放
		m_CurrentDepth->Release();

		// nullptrを敷き詰める
		m_CurrentDepth = nullptr;
	}

	// nullptrチェック
	if (m_CurrentRender != nullptr)
	{
		// メモリ開放
		m_CurrentRender->Release();

		// nullptrを敷き詰める
		m_CurrentRender = nullptr;
	}
}

//*********************************************
// オブジェクトの描画
//*********************************************
void CXTexture::DrawObject(std::string Path)
{
	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxWorld;		// 計算用マトリックス
	D3DMATERIAL9 matDef;								// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;									// マテリアルへのポインタ

	// モデルの情報を取得
	CModelManager::MapObject modelinfo = CModelManager::GetModelInfo(Path);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)modelinfo.modelinfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.modelinfo.TexPath.size(); nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, CLoadTexture::GetTex(modelinfo.modelinfo.TexPath[nCntMat]));

		// モデル(パーツ)の描画
		modelinfo.modelinfo.pMesh->DrawSubset(nCntMat);
	}
	pDevice->SetMaterial(&matDef);
}

//*********************************************
// インスタンス取得
//*********************************************
CXTexture* CXTexture::Instance(void)
{
	if (m_Instance != NULL) return m_Instance.get();
	m_Instance.reset(new CXTexture);
	return m_Instance.get();
}

//*********************************************
// 文字列を登録
//*********************************************
void CXTexture::ResistX(std::string Path, std::string ModelPath)
{
	// 検索結果を代入
	auto Find = m_XTex.find(Path);

	// すでに存在したら
	if (Find != m_XTex.end())return;

	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// パスを代入
	m_XTex[Path].Path = ModelPath;

	// 作成
	pDevice->CreateTexture(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_XTex[Path].Tex,
		NULL
	);

	// 深度バッファを作る
	pDevice->CreateDepthStencilSurface(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_XTex[Path].DepthBuffer,
		NULL
	);

	// サーフェイスを取得
	m_XTex[Path].Tex->GetSurfaceLevel(0, &m_XTex[Path].Surface);
}

//*********************************************
// テクスチャを取得
//*********************************************
LPDIRECT3DTEXTURE9 CXTexture::GetTexFromX(std::string Path)
{
	// 検索結果を代入
	auto Find = m_XTex.find(Path);

	// すでに存在したら
	if (Find != m_XTex.end())return Find->second.Tex;

	return NULL;
}

//*********************************************
// リセットした後の関数
//*********************************************
void CXTexture::ResetStart(void)
{
	// デバイス取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// マップの中にアクセス
	for (auto& Map : m_XTex)
	{
		// 作成
		pDevice->CreateTexture(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_X8R8G8B8,
			D3DPOOL_DEFAULT,
			&Map.second.Tex,
			NULL
		);

		// 深度バッファを作る
		pDevice->CreateDepthStencilSurface(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			D3DFMT_D24S8,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&Map.second.DepthBuffer,
			NULL
		);

		// サーフェイスを設定
		Map.second.Tex->GetSurfaceLevel(0, &Map.second.Surface);
	}
}

//*********************************************
// リセットする前の処理
//*********************************************
void CXTexture::ResetEnd(void)
{
	// マップの中にアクセス
	for (auto& Map : m_XTex)
	{
		// nullptrチェック
		if (Map.second.Tex != nullptr)
		{
			// メモリ開放
			Map.second.Tex->Release();

			// nullptrを敷き詰める
			Map.second.Tex = nullptr;
		}
		// nullptrチェック
		if (Map.second.Surface != nullptr)
		{
			// メモリ開放
			Map.second.Surface->Release();

			// nullptrを敷き詰める
			Map.second.Surface = nullptr;
		}
		// nullptrチェック
		if (Map.second.DepthBuffer != nullptr)
		{
			// メモリ開放
			Map.second.DepthBuffer->Release();

			// nullptrを敷き詰める
			Map.second.DepthBuffer = nullptr;
		}
	}
	// nullptrチェック
	if (m_CurrentDepth != nullptr)
	{
		// メモリ開放
		m_CurrentDepth->Release();

		// nullptrを敷き詰める
		m_CurrentDepth = nullptr;
	}

	// nullptrチェック
	if (m_CurrentRender != nullptr)
	{
		// メモリ開放
		m_CurrentRender->Release();

		// nullptrを敷き詰める
		m_CurrentRender = nullptr;
	}
}