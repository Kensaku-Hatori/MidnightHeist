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
		static constexpr float MoveRDist = -8.0f;
		static constexpr float MoveSpeedR = 0.03f;
		static const D3DXVECTOR3 OffSetR;
		static const D3DXVECTOR3 CatchSpeedR;
		// 初期化時の定数
		struct Defoult {
			static constexpr float Distance = 1100.0f;
			static constexpr float RDist = -10.0f;
			static constexpr float Fov = 45.0f;
			static constexpr float Near = 10.0f;
			static constexpr float Far = 10000.0f;
			static constexpr float SpeedV = 0.3f;
		};
	};

	// モーションの種類
	typedef enum
	{
		MOTIONTYPE_STARTMOVIE = 0,
		MOTIONTYPE_MAX
	}MOTIONTYPE;

	// キー情報
	struct KEY
	{
		// 注視点の目標の位置
		float fPosRX;
		float fPosRY;
		float fPosRZ;
		// 始点の目標の位置
		float fPosVX;
		float fPosVY;
		float fPosVZ;
		// フレーム
		int nFrame;
	};

	// モーション情報
	struct MOTIONINFO
	{
		bool bLoop;					// ループするかどうか
		int nNumKey;				// キーの総数
		std::vector<KEY> pKeyInfo;	// キー情報の可変長配列
	};

	// コンストラクタ・デストラクタ
	CCamera();
	~CCamera();

	// メンバ関数
	HRESULT Init(void);
	void Update(void);
	void Uninit(void);
	void SetCamera(void);
	void UpdateMouseMove(void);
	void UpdateJoyPadMove(void);
	void UpdateCameraPositionV(void);
	void UpdateCameraPositionR(void);
	void UpdateMotion(void);

	void NormalizeCameraRot(void);
	D3DXVECTOR3 CalcAssistVec(void);
	void Assist(D3DXVECTOR3 AssistVec,float Strength);

	// 条件式の関数
	bool isMovePosV(void) { return m_pInputMouse->OnPress(0) && m_pInputKeyboard->GetPress(DIK_LALT); }
	bool isMovePosR(void) { return m_pInputMouse->OnPress(1) && m_pInputKeyboard->GetPress(DIK_LALT); }
	bool isSlideMove(void) { return m_pInputMouse->OnPress(2); }
	bool IsAnim(void) { return m_isMovie; };

	// セッター
	void SetDistance(float Distance) { m_fDistance = Distance; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetMouseWheel(int zDelta);
	void SetSpeedV(const float Speed) { m_fSpeedV = Speed; }
	void SetMovie(MOTIONTYPE Type);
	static void ResetProjectionMtx(void);

	// ゲッター
	D3DXVECTOR2 MouseMove(void) { return m_pInputMouse->GetVelocity(); }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }
	MOTIONTYPE GetMotion(void) { return m_nMotionType; }
	float GetSpeedV(void) { return m_fSpeedV; }

	// ローダー
	void LoadMotion(std::string Path);
private:
	// 静的メンバ変数
	// 入力デバイスのメンバ変数
	static CInputKeyboard* m_pInputKeyboard;
	static CInputJoypad* m_pInputJoypad;
	static CInputMouse* m_pInputMouse;

	// メンバ変数
	D3DXMATRIX m_mtxProjection;				// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;					// ビューマトリックス


	D3DXVECTOR3 m_posV;						// 視点
	D3DXVECTOR3 m_posR;						// 注視点
	D3DXVECTOR3 m_RSpeed;					// 注視点の速さ

	D3DXVECTOR3 m_posVDest;					// 視点
	D3DXVECTOR3 m_posRDest;					// 注視点

	D3DXVECTOR3 m_vecU;						// 上方向ベクトル

	D3DXVECTOR3 m_rot;						// 向き

	float m_fDistance;						// 視点から注視点の距離
	float m_fFov;							// 視野角
	float m_fZnear;							// 視界の最小値
	float m_fZfar;							// 視界の最大値
	float m_fSpeedV;						// 目標の位置までの速度
	bool m_isMovie;							// ムービー中かどうか

	// ムービー用
	std::vector<MOTIONINFO> m_vMotionInfo;					// モーションの情報
	MOTIONTYPE m_nMotionType;								// 今のモーションの種類
	int m_nAllFrame;										// 全体フレーム
	int m_nKey, m_nCounterMotion, m_nNextKey;				// キーの情報とモーションカウンター
	bool m_bFinishMotion;									// モーションが終わったかどうか
};
#endif // !