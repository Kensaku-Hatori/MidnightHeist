//****************************************************************
//
// サインカーブシェーダーの処理[VisibleSineCurve.h]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "shader.h"

// クラスを定義
class CVisibleSineCurve : public CShader
{
public:
	// デストラクタ
	~CVisibleSineCurve() = default;

	// メンバ関数
	HRESULT Init(void);

	// パラメータ設定
	void SetParameters(D3DXMATRIX mtxWorld, const int Counter, const float Speed, const float Ripple, const float Micro);

	// ゲッター
	LPD3DXEFFECT& GetEffect() { return CShader::GetEffect(); }

	// 静的メンバ関数
	static CVisibleSineCurve* Instance(void) {
		static std::unique_ptr<CVisibleSineCurve> Instance(new CVisibleSineCurve);
		return Instance.get();
	};
private:
	// コンストラクタ
	CVisibleSineCurve() = default;
	// ハンドル
	D3DXHANDLE m_mtxWorldHandle;// ワールドマトリックスのハンドル
	D3DXHANDLE m_ViewHandle;	// ビューマトリックスのハンドル
	D3DXHANDLE m_ProjHandle;	// プロジェクションマトリックスのハンドル
	D3DXHANDLE m_CenterPos;		// 中心位置
	D3DXHANDLE m_SineCount;		// カウンタ
	D3DXHANDLE m_SineSpeed;		// 速さ
	D3DXHANDLE m_SineRipple;	// 普及率
	D3DXHANDLE m_SineMicro;		// 細かさ
};