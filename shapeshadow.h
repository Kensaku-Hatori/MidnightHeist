//****************************************************************
//
// 物陰の処理[shapeshadow.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _SHAPESHADOW_H_
#define _SHAPESHADOW_H_

// インクルード
#include "shader.h"

// クラスを定義
class CShapeShadow : public CShader
{
public:
	// デストラクタ
	~CShapeShadow();

	// メンバ関数
	HRESULT Init(void);
	void DrawTex(void);
	void Clear(void);
	void BeginObject(void);
	void BeginScene(void);
	void EndTexs(void);
	void ReSet(void);
	void ReStart(void);

	// セッター
	void SetParameters(D3DXMATRIX World); // ライトやマテリアルなどの設定

	// ゲッター
	LPDIRECT3DTEXTURE9 GetTex(void) { return m_SceneTex; }
	LPDIRECT3DTEXTURE9 GetObjectTex(void) { return m_ObjectTex; }

	// 静的メンバ関数
	// シングルトン
	static CShapeShadow* Instance(void) {
		static std::unique_ptr<CShapeShadow> Instance(new CShapeShadow);
		return Instance.get();
	};
private:
	// コンストラクタ
	CShapeShadow() {};
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffMT;		// 頂点バッファ

	LPDIRECT3DTEXTURE9 m_ObjectTex;					// Z値テクスチャ
	LPDIRECT3DSURFACE9 m_ObjectSurface;				// Z値サーフェイス
	LPDIRECT3DSURFACE9 m_ObjectDepthSurface;		// 震度バッファサーフェイス

	LPDIRECT3DTEXTURE9 m_SceneTex;					// シーンのテクスチャ
	LPDIRECT3DSURFACE9 m_SceneSurface;				// シーンサーフェイス
	LPDIRECT3DSURFACE9 m_SceneDepthSurface;			// 震度バッファサーフェイス

	LPDIRECT3DSURFACE9 m_OldRT;						// 既存のレンダータゲット
	LPDIRECT3DSURFACE9 m_OldDepthSurface;			// 既存の震度バッファ
};
#endif // !_SHAPESHADOW_H_