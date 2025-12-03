//****************************************************************
//
// シェーダーの処理[shader.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _SHADER_H_
#define _SHADER_H_

// 基底クラスを定義
class CShader
{
public:
	// コンストラクタ・デストラクタ
	CShader() {};
	~CShader();

	// メンバ関数
	HRESULT Init(int ResourseId);
	void Uninit(void);
	void Begin(void);
	void BeginPass(const UINT Pass = 0);
	void End(void);
	void EndPass(void);
	void Reset(void);
	void Restart(void);

	// ゲッター
	D3DXHANDLE& GetHandle(std::string Key);
	LPD3DXEFFECT& GetEffect() { return m_pEffect; }
private:
	// メンバ変数
	LPSTR m_EffectPath;						// シェーダーファイルへのファイルパス
	LPD3DXEFFECT m_pEffect = NULL;					// シェーダーオブジェクト
	D3DXHANDLE m_Tech = NULL;						// テクニックへのハンドル
	std::map<std::string, D3DXHANDLE> m_vHandle;	// ハンドル
};
#endif // !_SHADER_H_