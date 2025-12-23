//****************************************************************
//
// カメラの処理[camera.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _CAMER_H_
#define _CAMER_H_

// インクルード
#include "main.h"
#include "input.h"
#include "BaceCameraSystem.h"

// カメラクラスを定義
class CCamera
{
public:
	// カメラの規定値を設定
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
		struct Title {
			static const D3DXVECTOR3 PosV;
			static const D3DXVECTOR3 PosR;
			static const D3DXVECTOR3 Rot;
		};
		struct Game {
			static const D3DXVECTOR3 PosV;
			static const D3DXVECTOR3 PosR;
			static const D3DXVECTOR3 Rot;
		};
	};

	// コンストラクタ・デストラクタ
	CCamera();
	~CCamera();

	// メンバ関数
	HRESULT Init(void);
	void Update(void);
	void Uninit(void);
	void EndSystems(void);
	void SetCamera(void);

	// セッター
	void SetDistance(float Distance) { m_fDistance = Distance; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPosV(const D3DXVECTOR3 PosV) { m_posV = PosV; }
	void SetPosVDest(const D3DXVECTOR3 PosVDest) { m_posVDest = PosVDest; }
	void SetPosR(const D3DXVECTOR3 PosR) { m_posR = PosR; }
	void SetPosRDest(const D3DXVECTOR3 PosRDest) { m_posRDest = PosRDest; }
	void SetMouseWheel(int zDelta);
	void SetSpeedV(const float Speed) { m_fSpeedV = Speed; }
	void AddSystem(CBaceSystem* _Add);
	static void ResetProjectionMtx(void);

	// ゲッター
	D3DXVECTOR2 MouseMove(void) { return m_pInputMouse->GetVelocity(); }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	D3DXVECTOR3 GetPosVDest(void) { return m_posVDest; }
	D3DXVECTOR3 GetPosRDest(void) { return m_posRDest; }
	float GetSpeedV(void) { return m_fSpeedV; }
private:
	// 更新
	// 角度の近道化
	void NormalizeCameraRot(void);
	// 目標の位置に近づける
	void UpdateCameraPositionV(void);
	void UpdateCameraPositionR(void);
	// デバイスでのカメラ移動
	void UpdateMouseMove(void);
	void UpdateJoyPadMove(void);
	// 条件式の関数
	bool isMovePosV(void) { return m_pInputMouse->OnPress(0) && m_pInputKeyboard->GetPress(DIK_LALT); }
	bool isMovePosR(void) { return m_pInputMouse->OnPress(1) && m_pInputKeyboard->GetPress(DIK_LALT); }
	bool isSlideMove(void) { return m_pInputMouse->OnPress(2); }
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
#endif // !