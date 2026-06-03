//================================================================
//
// テクスチャを一括管理するクラスの処理[texmanager.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二十インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************
#include "Bace/main.h"
#include <unordered_map>

//****************************************************************
// テクスチャを外部ファイルから読み込むクラス
//****************************************************************
class CLoadTexture
{
public:
	// デストラクタ
	~CLoadTexture();

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="LoadFilePath">パスリストへのパス</param>
	static void Load(std::string LoadFilePath);
	/// <summary>
	/// テクスチャを取得
	/// </summary>
	/// <param name="Path">パス</param>
	/// <returns>テクスチャ</returns>
	static LPDIRECT3DTEXTURE9& GetTex(std::string Path);
	/// <summary>
	/// テクスチャを破棄
	/// </summary>
	/// <param name=""></param>
	static void UnRegistTex(void);
private:
	// コンストラクタ
	CLoadTexture();
	// 静的メンバ変数
	static std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_TexMap;	// テクスチャへのマップを作成
};