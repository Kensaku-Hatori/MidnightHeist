//================================================================
//
// モデルを一括管理するクラスの処理[modelmanager.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************
#include "texmanager.h"

//****************************************************************
// モデルマネージャーを定義
//****************************************************************
class CModelManager
{
public:
	// モデルの基礎情報の構造体
	struct ModelInfo
	{
		// モデル情報の構造体
		LPD3DXMESH pMesh;						// メッシュへのポインタ
		LPD3DXMESH pSmoothMesh;					// なめらかメッシュへのポインタ
		LPD3DXBUFFER pBuffMat;					// マテリアルへのポインタ
		DWORD dwNumMat;							// マテリアルの数
		std::vector<LPDIRECT3DTEXTURE9> Tex;	// テクスチャパス
	};

	// マップに出すオブジェクトの基礎情報
	struct MapObject
	{
		ModelInfo modelinfo;	// モデルの基本情報
	};

	// デストラクタ
	~CModelManager();

	// 読み込み・破棄

	/// <summary>
	/// モデルを読み込み
	/// </summary>
	/// <param name="LoadFilePath">ファイルパス</param>
	static void Load(std::string LoadFilePath);
	/// <summary>
	/// 登録解除
	/// </summary>
	/// <param name=""></param>
	static void UnRegistModel(void);
	/// <summary>
	/// モデル情報を取得
	/// </summary>
	/// <param name="Path"></param>
	/// <returns></returns>
	static MapObject& GetModelInfo(std::string Path);
private:
	// コンストラクタ・デストラクタ
	CModelManager();
	/// <summary>
	/// 読み込み失敗した時のモデルを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	static MapObject& FailedLoadMesh(void);

	// 静的メンバ変数
	static std::unordered_map<std::string, MapObject> m_ModelMap;	// モデル情報を格納する変数
};