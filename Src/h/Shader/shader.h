//================================================================
//
// シェーダーの処理[shader.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// 基底クラスを定義
//****************************************************************
class CShader
{
public:
	// コンストラクタ・デストラクタ
	CShader() {};
	~CShader();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="ResourseId">読み込みID</param>
	/// <returns>結果</returns>
	HRESULT Init(int ResourseId);
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void);
	/// <summary>
	/// 開始
	/// </summary>
	/// <param name=""></param>
	void Begin(void);
	/// <summary>
	/// 描画パスを指定して開始
	/// </summary>
	/// <param name="Pass"></param>
	void BeginPass(const UINT Pass = 0);
	/// <summary>
	/// 終了
	/// </summary>
	/// <param name=""></param>
	void End(void);
	/// <summary>
	/// 現在の描画パスを終了
	/// </summary>
	/// <param name=""></param>
	void EndPass(void);
	/// <summary>
	/// リセット処理(デバイスロスト時)
	/// </summary>
	/// <param name=""></param>
	void Reset(void);
	/// <summary>
	/// リスタート処理(デバイスロスト復帰時)
	/// </summary>
	/// <param name=""></param>
	void Restart(void);
	/// <summary>
	/// ハンドルを取得
	/// </summary>
	/// <param name="Key">取得したいパラメーターハンドルの名前</param>
	/// <returns>ハンドル</returns>
	D3DXHANDLE& GetHandle(std::string Key);
	/// <summary>
	/// エフェクトを取得
	/// </summary>
	/// <returns>エフェクト</returns>
	inline LPD3DXEFFECT& GetEffect() { return m_pEffect; }
private:
	// メンバ変数
	LPSTR m_EffectPath = {};							// シェーダーファイルへのファイルパス
	LPD3DXEFFECT m_pEffect = NULL;						// シェーダーオブジェクト
	D3DXHANDLE m_Tech = NULL;							// テクニックへのハンドル
	std::map<std::string, D3DXHANDLE> m_vHandle = {};	// ハンドル
};