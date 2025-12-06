//****************************************************************
//
// スカイボックスの処理[SkyBox.h]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "object.h"

// クラスを定義
class CSkyBox : public CObject
{
public:
	// 6面分のファイルパス
	struct SkyBoxPathList {
		std::string Faces[6];
	};

	// コンスタント・デストラクタ
	CSkyBox() = default;
	~CSkyBox() = default;

	// メンバ関数
	HRESULT Init(void)override;
	void Uninit(void)override;
	void Update(void)override;
	void Draw(void)override;

	// ローダー
	void Load(void);

	// 6面分のテクスチャを区別
	void ProcessSkyBox(const std::string& FilePath, SkyBoxPathList& Out);

	// 静的メンバ関数
	static CSkyBox* Instance(void) {
		static std::unique_ptr<CSkyBox> Instance = std::make_unique<CSkyBox>();
		return Instance.get();
	}
private:
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	LPDIRECT3DCUBETEXTURE9 m_pCubeTex;	// テクスチャ
	D3DXMATRIX m_mtxWorld;				// マトリックス
};