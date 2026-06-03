//================================================================
//
// カメラの処理[camera.h]
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
#include "main.h"
#include "input.h"
#include "System/BaceCameraSystem.h"

//****************************************************************
// カメラクラスを定義
//****************************************************************
class CCamera
{
public:
	// カメラの定数を宣言
	struct Config {
		static constexpr float MouseSpeedV = 0.005f;
		static constexpr float MouseSpeedR = 0.0005f;
		static constexpr float PadSpeed = 0.0000005f;
		static constexpr float RotLimit = 0.4f;
		static constexpr float WheelSpeed = 0.1f;
		static constexpr float DistanceNear = 100.0f;
		static constexpr float DistanceFar = 200.0f;
		static constexpr float MoveSpeedR = 0.03f;
		// 初期化時の定数
		struct Defoult {
			static constexpr float Distance = 1100.0f;
			static constexpr float RDist = -10.0f;
			static constexpr float Fov = 45.0f;
			static constexpr float Near = 10.0f;
			static constexpr float Far = 3000.0f;
			static constexpr float SpeedV = 0.3f;
		};
		// タイトル時の定数
		struct Title {
			static const D3DXVECTOR3 PosV;
			static const D3DXVECTOR3 PosR;
			static const D3DXVECTOR3 Rot;
		};
		// ゲーム時の定数
		struct Game {
			static const D3DXVECTOR3 PosV;
			static const D3DXVECTOR3 PosR;
			static const D3DXVECTOR3 Rot;
		};
	};

	CCamera();
	~CCamera();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void);
	/// <summary>
	/// システムの破棄
	/// </summary>
	/// <param name=""></param>
	void EndSystems(void);
	/// <summary>
	/// カメラのマトリックスを設定
	/// </summary>
	/// <param name=""></param>
	void SetCamera(void);
	/// <summary>
	/// マウスホイールで距離を設定
	/// </summary>
	/// <param name="zDelta">マウスホイールのスピード</param>
	void SetMouseWheel(int zDelta);
	/// <summary>
	/// システムを追加
	/// </summary>
	/// <param name="_Add">追加したいシステムのポインタ</param>
	void AddSystem(CBaceSystem* _Add);
	/// <summary>
	/// マトリックスを初期値に再設定
	/// </summary>
	/// <param name=""></param>
	void ResetProjectionMtx(void);
	/// <summary>
	/// 視点・注視点の距離を設定
	/// </summary>
	/// <param name="Distance">距離</param>
	inline void SetDistance(float Distance) { m_fDistance = Distance; }
	/// <summary>
	/// 視点・注視点間の向きを設定
	/// </summary>
	/// <param name="rot">向き</param>
	inline void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	/// <summary>
	/// 視点を設定
	/// </summary>
	/// <param name="PosV">視点の位置</param>
	inline void SetPosV(const D3DXVECTOR3 PosV) { m_posV = PosV; }
	/// <summary>
	/// 視点の補完先を設定
	/// </summary>
	/// <param name="PosVDest">補完先の視点</param>
	inline void SetPosVDest(const D3DXVECTOR3 PosVDest) { m_posVDest = PosVDest; }
	/// <summary>
	/// 注視点の設定
	/// </summary>
	/// <param name="PosR">注視点の位置</param>
	inline void SetPosR(const D3DXVECTOR3 PosR) { m_posR = PosR; }
	/// <summary>
	/// 注視点の補完先を設定
	/// </summary>
	/// <param name="PosRDest">補完先の注視点</param>
	inline void SetPosRDest(const D3DXVECTOR3 PosRDest) { m_posRDest = PosRDest; }
	/// <summary>
	/// 視点を補完する速さを設定
	/// </summary>
	/// <param name="Speed">補完スピード</param>
	inline void SetSpeedV(const float Speed) { m_fSpeedV = Speed; }
	/// <summary>
	/// マウスの移動量を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>移動量</returns>
	inline D3DXVECTOR2 GetMouseMove(void) { return m_pInputMouse->GetVelocity(); }
	/// <summary>
	/// 向きを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>向き</returns>
	inline D3DXVECTOR3 GetRot(void) { return m_rot; }
	/// <summary>
	/// 視点を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>視点</returns>
	inline D3DXVECTOR3 GetPosV(void) { return m_posV; }
	/// <summary>
	/// 注視点を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>注視点</returns>
	inline D3DXVECTOR3 GetPosR(void) { return m_posR; }
	/// <summary>
	/// 視点の補完先を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>補完先の視点の位置</returns>
	inline D3DXVECTOR3 GetPosVDest(void) { return m_posVDest; }
	/// <summary>
	/// 注視点の補完先を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>補完先の注視点</returns>
	inline D3DXVECTOR3 GetPosRDest(void) { return m_posRDest; }
	/// <summary>
	/// 視点の補完スピードを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>補完スピード</returns>
	inline float GetSpeedV(void) { return m_fSpeedV; }
private:
	/// <summary>
	/// 角度の近道化
	/// 3.15->-3.13
	/// </summary>
	/// <param name=""></param>
	void NormalizeCameraRot(void);
	/// <summary>
	/// 現在の視点を目標の視点に補完
	/// </summary>
	/// <param name=""></param>
	void UpdateCameraPositionV(void);
	/// <summary>
	/// 現在の注視点を目標の注視点に補完
	/// </summary>
	/// <param name=""></param>
	void UpdateCameraPositionR(void);
	/// <summary>
	/// マウスでのカメラ移動
	/// </summary>
	/// <param name=""></param>
	void UpdateMouseMove(void);
	/// <summary>
	/// ゲームパッドでのカメラ移動
	/// </summary>
	/// <param name=""></param>
	void UpdateJoyPadMove(void);
	/// <summary>
	/// 視点移動の条件式
	/// </summary>
	/// <param name=""></param>
	/// <returns>移動できるかどうか</returns>
	inline bool isMovePosV(void) { return m_pInputMouse->OnPress(0) && m_pInputKeyboard->GetPress(DIK_LALT); }
	/// <summary>
	/// 注視点移動の条件式
	/// </summary>
	/// <param name=""></param>
	/// <returns>移動できるかどうか</returns>
	inline bool isMovePosR(void) { return m_pInputMouse->OnPress(1) && m_pInputKeyboard->GetPress(DIK_LALT); }
	/// <summary>
	/// ホイールを押しながら移動できるかどうかの条件式
	/// </summary>
	/// <param name=""></param>
	/// <returns>移動できるかどうか</returns>
	inline bool isSlideMove(void) { return m_pInputMouse->OnPress(2); }

	// 静的メンバ変数
	// 入力デバイスのメンバ変数
	static CInputKeyboard* m_pInputKeyboard;
	static CInputJoypad* m_pInputJoypad;
	static CInputMouse* m_pInputMouse;

	// メンバ変数
	std::vector<std::unique_ptr<CBaceSystem>> m_SystemList;	// システムのリスト
	D3DXMATRIX m_mtxProjection;								// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;									// ビューマトリックス

	D3DXVECTOR3 m_posV;						// 視点
	D3DXVECTOR3 m_posR;						// 注視点
	D3DXVECTOR3 m_posVDest;					// 視点
	D3DXVECTOR3 m_posRDest;					// 注視点
	D3DXVECTOR3 m_vecU;						// 上方向ベクトル
	D3DXVECTOR3 m_rot;						// 向き

	float m_fDistance;						// 視点から注視点の距離
	float m_fSpeedV;						// 目標の位置までの速度
	float m_fFov;							// 視野角
	float m_fZnear;							// 視界の最小値
	float m_fZfar;							// 視界の最大値
};