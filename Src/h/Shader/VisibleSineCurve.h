//================================================================
//
// サインカーブシェーダーの処理[VisibleSineCurve.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// インクルード
//****************************************************************
#include "shader.h"

//****************************************************************
// クラスを定義
//****************************************************************
class CVisibleSineCurve : public CShader
{
public:
	// デストラクタ
	~CVisibleSineCurve() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void);
	/// <summary>
	/// パラメーター設定
	/// </summary>
	/// <param name="mtxWorld">ワールドマトリックス</param>
	/// <param name="Counter"></param>
	/// <param name="Speed"></param>
	/// <param name="Ripple"></param>
	/// <param name="Micro"></param>
	/// <param name="Radius"></param>
	void SetParameters(D3DXMATRIX mtxWorld, const int Counter, const float Speed, const float Ripple, const float Micro, const float Radius);

	// ゲッター
	LPD3DXEFFECT& GetEffect() { return CShader::GetEffect(); }

	// 静的メンバ関数
	static CVisibleSineCurve& Instance(void) {
		static CVisibleSineCurve Instance;
		return Instance;
	};
private:
	// コンストラクタ
	CVisibleSineCurve() = default;
	// ハンドル
	D3DXHANDLE m_mtxWorldHandle;// ワールドマトリックスのハンドル
	D3DXHANDLE m_ViewHandle;	// ビューマトリックスのハンドル
	D3DXHANDLE m_ProjHandle;	// プロジェクションマトリックスのハンドル
	D3DXHANDLE m_CenterPos;		// 中心位置
	D3DXHANDLE m_Radius;		// 半径
	D3DXHANDLE m_SineCount;		// カウンタ
	D3DXHANDLE m_SineSpeed;		// 速さ
	D3DXHANDLE m_SineRipple;	// 普及率
	D3DXHANDLE m_SineMicro;		// 細かさ
};