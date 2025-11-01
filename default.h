//****************************************************************
//
// 通常描画の処理[default.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _DEFOULT_H_
#define _DEFOULT_H_

// クラスを定義
class CDefoult
{
public:
	// デストラクタ
	~CDefoult();

	// メンバ関数
	HRESULT Init(std::string Path);
	void Begin(void);
	void BeginPass(const UINT Pass = 0);
	void End(void);
	void EndPass(void);

	// セッター
	void SetParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, D3DXVECTOR4 Light, D3DXVECTOR4 Col = { 1.0f,1.0f,1.0f,1.0f }); // ライトやマテリアルなどの設定

	// ゲッター
	LPD3DXEFFECT GetEffect() const { return m_pEffect; }

	// 静的メンバ関数
	static CDefoult* Instance(void);
private:
	// コンストラクタ
	CDefoult() {};

	// メンバ変数
	LPD3DXEFFECT m_pEffect;
	D3DXHANDLE m_Tech;							// テクニックへのハンドル
	D3DXHANDLE m_mtxWorld;						// シェーダのグローバル変数用ハンドル(マトリクス)
	D3DXHANDLE m_View;							// シェーダのグローバル変数用ハンドル(マトリクス)
	D3DXHANDLE m_Proj;							// シェーダのグローバル変数用ハンドル(マトリクス)
	D3DXHANDLE m_vecLight;						// シェーダのグローバル変数用ハンドル(ライトの向き)
	D3DXHANDLE m_Diffuse;						// シェーダのグローバル変数用ハンドル(モデルの色)

		// 静的メンバ変数
	static std::unique_ptr<CDefoult> m_Instance;
};
#endif // !_DEFOULT_H