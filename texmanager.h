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
#include "object.h"
#include "objectX.h"
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
	static LPDIRECT3DTEXTURE9 GetTex(std::string Path);
	static LPDIRECT3DTEXTURE9 GetTex(const int Idx);
	static void UnRegistTex(void);
private:
	// コンストラクタ
	CLoadTexture();

	// 静的メンバ変数
	static std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_TexMap;	// テクスチャへのマップを作成
};

// モデルテクスチャを生成するクラスを定義
class CXTexture : public CObject
{
public:
	// モデルテクスチャに必要な情報
	struct XInfo {
		std::string Path;			// パス
		LPDIRECT3DTEXTURE9 Tex;		// テクスチャ
		LPDIRECT3DSURFACE9 Surface;	// サーフェイス
		LPDIRECT3DSURFACE9 DepthBuffer;	// サーフェイス
	};

	// デストラクタ
	~CXTexture();

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void)override;
	void Update(void)override;
	void Draw(void)override;
	void DrawObject(std::string Path);

	// パスを格納
	void ResistX(std::string Path, std::string ModelPath);

	// ゲッター
	LPDIRECT3DTEXTURE9 GetTexFromX(std::string Path);

	// セッター
	void ResetStart(void);
	void ResetEnd(void);
	// 静的メンバ関数
	static CXTexture* Instance(void);
private:
	// コンストラクタ
	CXTexture() {};

	// 静的メンバ変数
	static std::unique_ptr<CXTexture> m_Instance;		// シングルトン
	std::vector<CObjectX> m_ModelList;					// モデルテクスチャの配列
	LPDIRECT3DSURFACE9 m_CurrentRender;					// 昔のレンダーターゲット
	LPDIRECT3DSURFACE9 m_CurrentDepth;					// 昔の震度バッファ
	std::unordered_map<std::string, XInfo> m_XTex;		// テクスチャへのインデックスのマップを作成
};
#endif // !_TEXMANAGER_H_