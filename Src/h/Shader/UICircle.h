//================================================================
//
// 円形UIの処理[UICircle.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************
#include "shader.h"

//****************************************************************
// クラスを定義
//****************************************************************
class CUICircle : public CShader
{
public:
	// デストラクタ
	~CUICircle() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void);
	/// <summary>
	/// パラメーター設定
	/// </summary>
	/// <param name="World">ワールドマトリックス</param>
	/// <param name="OriginWorld">中心位置を計算するためのマトリックス</param>
	/// <param name="FillAmount">塗りつぶす量</param>
	/// <param name="Radius">大きさ</param>
	/// <param name="MaxFillAngle">最大塗りつぶし量</param>
	void SetParameters(const D3DXMATRIX& World, const D3DXMATRIX& OriginWorld, const float FillAmount, const float Radius, const float MaxFillAngle);
	/// <summary>
	/// シングルトン
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static CUICircle& Instance(void) {
		static CUICircle Instance;
		return Instance;
	};
private:
	// コンストラクタ
	CUICircle() = default;
	// メンバ変数
	D3DXHANDLE m_mtxWorldHandle;	// ワールドマトリックスのハンドル
	D3DXHANDLE m_ViewHandle;		// ビューマトリックスのハンドル
	D3DXHANDLE m_ProjHandle;		// プロジェクションマトリックスのハンドル
	D3DXHANDLE m_FillAmoutHandle;	// 塗りつぶし量のハンドル
	D3DXHANDLE m_CenterHandle;		// 中心位置のハンドル
	D3DXHANDLE m_RadiusHandle;		// 半径のハンドル
	D3DXHANDLE m_MaxFillHandle;		// 最大塗りつぶし量のハンドル
};