//****************************************************************
//
// モデルを一括管理するクラスの処理[modelmanager.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "modelmanager.h"
#include "manager.h"

// 名前空間
using namespace std;
// 静的メンバ変数
unordered_map<std::string, CModelManager::MapObject> CModelManager::m_ModelMap = {};

//*********************************************
// コンストラクタ
//*********************************************
CModelManager::CModelManager()
{
}

//*********************************************
// デストラクタ
//*********************************************
CModelManager::~CModelManager()
{
}

void CModelManager::SmoothMesh(std::string Path)
{
}

//*********************************************
// ファイルを開く
//*********************************************
void CModelManager::Load(std::string LoadFilePath)
{
	ifstream ifs(LoadFilePath.c_str());

	if (!ifs)
	{
		MessageBox(NULL, "ファイルが読み込めませんでした", "終了メッセージ", MB_OK);
		return;
	}

	string line = {};
	string label = {}, equal = {};

	while (getline(ifs, line))
	{
		string FilePath;
		string Category;

		istringstream iss(line);
		if (line.find("MODELFILE_NAME") != string::npos)
		{
			iss >> label >> equal >> FilePath;
		}
		if (line.find("CATEGORY") != string::npos)
		{
			iss >> label >> equal >> Category;
		}

		if (FilePath[0] != NULL && Category[0] != NULL)
		{
			GetModelInfo(FilePath);
		}

		if (line.find("#") != string::npos)
		{
			continue;
		}
	}
}

//*********************************************
// モデルの情報を取得 & 追加
//*********************************************
CModelManager::MapObject& CModelManager::GetModelInfo(string Path)
{
	// 検索結果を代入
	auto Find = m_ModelMap.find(Path);

	// すでに存在したら
	if (Find != m_ModelMap.end())return Find->second;

	// 被りがなくてパスの長さが短かったらクリエイト
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// モデルの情報を読み込む一時変数
	MapObject LocalInfo = {};

	// Xファイル読込
	HRESULT hr;
	hr = D3DXLoadMeshFromX(
		Path.c_str(),
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&LocalInfo.modelinfo.pBuffMat,
		NULL,
		&LocalInfo.modelinfo.dwNumMat,
		&LocalInfo.modelinfo.pMesh
	);

	// メッシュをコピー
	LocalInfo.modelinfo.pMesh->CloneMeshFVF(
		D3DXMESH_SYSTEMMEM,
		LocalInfo.modelinfo.pMesh->GetFVF(),
		pDevice,
		&LocalInfo.modelinfo.pSmoothMesh
	);

	// 読み込めなかったら
	if (FAILED(hr))
	{
		// 失敗したら
		assert(0 && "モデルパスが間違っています");
	}

	// 法線のスムース化
	const float Epsilon = 1e-6f;
	std::vector<DWORD> adjacency(LocalInfo.modelinfo.pSmoothMesh->GetNumFaces() * 3);
	LocalInfo.modelinfo.pSmoothMesh->GenerateAdjacency(Epsilon, adjacency.data());

	hr = D3DXComputeNormals(LocalInfo.modelinfo.pSmoothMesh, adjacency.data());

	if (FAILED(hr))
	{
		// 失敗したら
		assert(0 && "モデルのスムース化に失敗しました");
	}

	//マテリアル辺のポインタを取得
	D3DXMATERIAL* pMat;
	pMat = (D3DXMATERIAL*)LocalInfo.modelinfo.pBuffMat->GetBufferPointer();

	// マテリアル分パスを確保
	LocalInfo.modelinfo.TexPath.resize(LocalInfo.modelinfo.dwNumMat);

	// マテリアルの数分回す
	for (int nCntMat = 0; nCntMat < (int)LocalInfo.modelinfo.dwNumMat; nCntMat++)
	{
		// パスが存在したら
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			// 代入
			LocalInfo.modelinfo.TexPath[nCntMat] = pMat[nCntMat].pTextureFilename;
		}
	}
	// マップにVlueとして設定
	m_ModelMap[Path] = LocalInfo;

	// 情報を返す
	return m_ModelMap[Path];
}

//*********************************************
// いろいろ破棄
//*********************************************
void CModelManager::UnRegistModel(void)
{
	// マップの中にアクセス
	for (auto& Map : m_ModelMap)
	{
		// nullptrチェック
		if (Map.second.modelinfo.pMesh != nullptr)
		{
			// メモリ開放
			Map.second.modelinfo.pMesh->Release();

			// nullptrを代入する
			Map.second.modelinfo.pMesh = nullptr;
		}
		// nullptrチェック
		if (Map.second.modelinfo.pSmoothMesh != nullptr)
		{
			// メモリ開放
			Map.second.modelinfo.pSmoothMesh->Release();

			// nullptrを代入する
			Map.second.modelinfo.pSmoothMesh = nullptr;
		}
		// nullptrチェック
		if (Map.second.modelinfo.pBuffMat != nullptr)
		{
			// メモリ開放
			Map.second.modelinfo.pBuffMat->Release();

			// nullptrを代入する
			Map.second.modelinfo.pBuffMat = nullptr;
		}
	}

	// マップ自体をクリア
	m_ModelMap.clear();
}