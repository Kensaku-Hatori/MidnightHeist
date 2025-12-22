//****************************************************************
//
// テクスチャを一括管理するクラスの処理[texmanager.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二十インクルード防止
#ifndef _TEXMANAGER_H_
#define _TEXMANAGER_H_

// インクルード
#include "main.h"
#include <unordered_map>

// テクスチャを外部ファイルから読み込むクラス
class CLoadTexture
{
public:
	// デストラクタ
	~CLoadTexture();

	// 読み込み・破棄
	static void Load(std::string LoadFilePath);

	// 静的メンバ関数
	static LPDIRECT3DTEXTURE9& GetTex(std::string Path);
	static void UnRegistTex(void);
private:
	// コンストラクタ
	CLoadTexture();

	// 静的メンバ変数
	static std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_TexMap;	// テクスチャへのマップを作成
};
#endif // !_TEXMANAGER_H_