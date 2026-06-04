//================================================================
//
// リザルトカメラの処理[ResultCameraSystem.h]
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
#include "Bace/camera.h"

//****************************************************************
// 成功時のカメラワーククラスを定義
//****************************************************************
class CClearCameraSystem final : public CBaseCameraSystem
{
public:
	CClearCameraSystem() = default;
	~CClearCameraSystem() = default;
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void)override;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void)override;
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void)override;
private:
	// 定数を宣言
	struct Config {
		// 視点・注視点
		static const D3DXVECTOR3 PosV;
		static const D3DXVECTOR3 PosR;
	};
};

//****************************************************************
// 失敗時のカメラワーククラスを定義
//****************************************************************
class CFailedCameraSystem final : public CBaseCameraSystem
{
public:
	CFailedCameraSystem() {};
	~CFailedCameraSystem() {};
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	void Init(void)override;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void)override;
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void)override;
private:
	// 定数
	struct Config {
		// 視点・注視点
		static const D3DXVECTOR3 PosV;
		static const D3DXVECTOR3 PosR;
	};
};