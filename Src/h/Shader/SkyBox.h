//================================================================
//
// スカイボックスの処理[SkyBox.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// インクルード
//****************************************************************
#include "Bace/object.h"

//****************************************************************
// クラスを定義
//****************************************************************
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

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void)override;
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void)override;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void)override;
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw(void)override;
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	inline static CSkyBox* Instance(void) {
		static std::unique_ptr<CSkyBox> Instance = std::make_unique<CSkyBox>();
		return Instance.get();
	}
private:
	/// <summary>
	/// テクスチャのロード
	/// </summary>
	/// <param name=""></param>
	void Load(void);
	/// <summary>
	/// テクスチャを解析
	/// </summary>
	/// <param name="FilePath"></param>
	/// <param name="Out"></param>
	void ProcessSkyBox(const std::string& FilePath, SkyBoxPathList& Out);
	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	LPDIRECT3DCUBETEXTURE9 m_pCubeTex;	// テクスチャ
	D3DXMATRIX m_mtxWorld;				// マトリックス
};