//****************************************************************
//
// モデルを一括管理するクラスの処理[modelmanager.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

// インクルード
#include "texmanager.h"

// モデルマネージャーを定義
class CModelManager
{
public:
	// モデルの基礎情報の構造体
	struct ModelInfo
	{
		// モデル情報の構造体
		LPD3DXMESH pMesh;					// メッシュへのポインタ
		LPD3DXMESH pSmoothMesh;				// なめらかメッシュへのポインタ
		LPD3DXBUFFER pBuffMat;				// マテリアルへのポインタ
		DWORD dwNumMat;						// マテリアルの数
		std::vector<std::string> TexPath;	// テクスチャパス
	};

	// マップに出すオブジェクトの基礎情報
	struct MapObject
	{
		ModelInfo modelinfo;	// モデルの基本情報
	};

	// デストラクタ
	~CModelManager();
	void SmoothMesh(std::string Path);

	// 読み込み・破棄
	static void Load(std::string LoadFilePath);

	// 静的メンバ関数
	static void UnRegistModel(void);

	// ゲッター
	static MapObject* GetModelInfo(std::string Path);
	static MapObject* GetModelInfo(const int Indx);
private:
	// コンストラクタ・デストラクタ
	CModelManager();

	// 静的メンバ変数
	static std::unordered_map<std::string, MapObject> m_ModelMap;	// モデル情報を格納する変数
};
#endif // !_MODELMANAGER_H_