//*************************************
//
//　入力処理[input.h]
//　Author:Hatori Kensaku
//
//*************************************

// 二十インクルード防止
#ifndef _INPUT_H_
#define _INPUT_H_

// インクルード
#include "renderer.h"

// 基底クラスを定義[入力に共通して必要なクラス]
class CInput
{
public:
	// 入力デバイスの規定値を設定
	struct InputConfig {
		static constexpr int NumKey = 256;
		static constexpr int DeadZone = 10000;
	};
	// コンストラクタ・デストラクタ
	CInput();
	virtual ~CInput();

	// メンバ関数
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
protected:
	// 静的メンバ変数
	static LPDIRECTINPUT8 m_pInput;	// 入力処理に必要な共通の変数
	// メンバ変数
	LPDIRECTINPUTDEVICE8 m_pDevice;	// それぞれの入力処理に必要な変数
private:
};



// 派生クラスを定義[キーボード]
class CInputKeyboard : public CInput
{
public:
	// コンストラクタ・デストラクタ
	CInputKeyboard();
	~CInputKeyboard();

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Update(void);
	void Uninit(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);
protected:
	// メンバ変数
	BYTE m_aKeyState[InputConfig::NumKey];		// 今のキー
	BYTE m_OldaKeyState[InputConfig::NumKey];	// 1F前のキー
private:

};



// 派生クラスを定義[マウス]
class CInputMouse
{
public:
	struct Ray
	{
		D3DXVECTOR3 Origin;
		D3DXVECTOR3 Dir;
	};

	// コンストラクタ・デストラクタ
	CInputMouse();
	~CInputMouse();

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void UpdateRay(void);
	void UpdatePos3D(void);
	bool OnPress(int button_type);
	bool OnDown(int button_type);
	bool OnUp(int button_type);

	// ゲッター
	bool IsMove(void);
	Ray GetRay(void) { return m_MouseRay; }
	D3DXVECTOR2 GetVelocity(void) { return GetPos() - GetPosOld(); }
	D3DXVECTOR2 GetPosOld(void);
	D3DXVECTOR2 GetPos(void);
	D3DXVECTOR3 GetVelocity3D(void) { return m_Pos3D - m_OldPos3D; }
	D3DXVECTOR3 GetPosOld3D(void) { return m_OldPos3D; }
	D3DXVECTOR3 GetPos3D(void) { return m_Pos3D; }

	// セッター
	DIMOUSESTATE GetMouseState(void) { return m_MouseState; };

protected:
	// 静的メンバ変数
	static LPDIRECTINPUT8 m_pInputMouse;	// マウスの入力に必要な変数
	// メンバ変数
	Ray m_MouseRay;							// マウスのレイ
	LPDIRECTINPUTDEVICE8 m_pDeviceMouse;	// マウスの入力処理に必要な変数
	DIMOUSESTATE m_MouseState;				// マウスのステート
	DIMOUSESTATE m_MouseOldState;			// マウスのステート
	D3DXVECTOR3 m_Pos3D;					// 3D座標の位置
	D3DXVECTOR3 m_OldPos3D;					// 3D座標の昔の位置
};



// 派生クラスを定義[ジョイパット]
class CInputJoypad : public CInput
{
public:
	//キーの種類
	typedef enum
	{
		JOYKEY_UP = 0,
		JOYKEY_DOWN,
		JOYKEY_LEFT,
		JOYKEY_RIGET,
		JOYKEY_START,
		JOYKEY_BACK,
		JOYKEY_L3,
		JOYKEY_R3,
		JOYKEY_L1,
		JOYKEY_R1,
		JOYKEY_L2,
		JOYKEY_R2,
		JOYKEY_A,
		JOYKEY_B,
		JOYKEY_X,
		JOYKEY_Y,
		JOYKEY_MAX
	}JOYKEY;

	// コンストラクタ・デストラクタ
	CInputJoypad();
	~CInputJoypad();

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Update(void);
	void Uninit(void);

	// セッター
	void BeginVibration(const float R = 0.5f, const float L = 0.5f, const int Frame = 1);
	// ゲッター
	bool GetPress(JOYKEY nKey);
	bool GetTrigger(JOYKEY nKey);
	bool GetRelease(JOYKEY nKey);
	bool GetJoyStickL(void);
	bool GetJoyStickR(void);
	XINPUT_STATE* GetJoyStickAngle(void) {	return &m_JoyKeyState;	};

protected:
	// メンバ変数
	XINPUT_STATE m_JoyKeyState;			// ジョイパットのステート
	XINPUT_STATE m_JoyKeyStateOld;		// ジョイパッドの昔の状態
private:
	int m_VibrationCounter = 0;			// バイブレーション用カウンタ
	int m_VibrationFrame;				// バイブレーション用フレーム
	float m_VibrationR;					// バイブレーション用の右の強さ
	float m_VibrationL;					// バイブレーション用の左の強さ
	bool m_IsFinishedVibration = false;	// バイブレーション終了フラグトリガー
};
#endif // !_INPUT_H_