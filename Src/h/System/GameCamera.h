//================================================================
//
// ゲームカメラの処理[GameCamera.h]
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
// 継承クラスを定義
//****************************************************************
class CGameCameraSystem final : public CBaseCameraSystem
{
public:
	CGameCameraSystem() = default;
	~CGameCameraSystem() = default;
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
		// デフォルト
		struct Default {
			// 視点・注視点
			static const D3DXVECTOR3 PosV;
			static const D3DXVECTOR3 PosR;
		};
		// プレイヤーが登場した時
		struct FirstAnimation {
			// 近さ
			static constexpr float Zoom = 1400.0f;
		};
		// プレイヤーがピッキングしているとき
		struct PickingZoom {
			// 近さ
			static constexpr float Zoom = 1200.0f;
		};
	};
};