//================================================================
//
//　入力処理[input.h]
//　Author:Hatori Kensaku
//
//================================================================

//****************************************************************
// 二十インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************
#include "renderer.h"

//****************************************************************
// 基底クラスを定義[入力に共通して必要なクラス]
//****************************************************************
class CInput
{
public:
	// 入力デバイスの定数を宣言
	struct InputConfig {
		static constexpr int NumKey = 256;
		static constexpr int DeadZone = 10000;
	};
	// コンストラクタ・デストラクタ
	CInput();
	virtual ~CInput();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="hInstance">インスタンスハンドル</param>
	/// <param name="hWnd">ウィンドウアンドル</param>
	/// <returns></returns>
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	virtual void Uninit(void);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	virtual void Update(void) = 0;
protected:
	// 静的メンバ変数
	static LPDIRECTINPUT8 m_pInput;	// 入力処理に必要な共通の変数
	// メンバ変数
	LPDIRECTINPUTDEVICE8 m_pDevice;	// それぞれの入力処理に必要な変数
private:
};

//****************************************************************
// 派生クラスを定義[キーボード]
//****************************************************************
class CInputKeyboard : public CInput
{
public:
	// コンストラクタ・デストラクタ
	CInputKeyboard();
	~CInputKeyboard();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="hInstance">インスタンスハンドル</param>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <returns></returns>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
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
	/// 押されているかどうか
	/// </summary>
	/// <param name="nKey">判別したいキー</param>
	/// <returns>押しているかどうか</returns>
	bool GetPress(int nKey);
	/// <summary>
	/// 押した瞬間
	/// </summary>
	/// <param name="nKey">判別したいキー</param>
	/// <returns>押しているかどうか</returns>
	bool GetTrigger(int nKey);
	/// <summary>
	/// 離した瞬間
	/// </summary>
	/// <param name="nKey">判別したいキー</param>
	/// <returns>押しているかどうか</returns>
	bool GetRelease(int nKey);
	/// <summary>
	/// 押している間
	/// </summary>
	/// <param name="nKey">判別したいキー</param>
	/// <returns>押しているかどうか</returns>
	bool GetRepeat(int nKey);
protected:
	// メンバ変数
	BYTE m_aKeyState[InputConfig::NumKey];		// 今のキー
	BYTE m_OldaKeyState[InputConfig::NumKey];	// 1F前のキー
private:
};

//****************************************************************
// 派生クラスを定義[マウス]
//****************************************************************
class CInputMouse
{
public:
	// 光線の構造体
	struct Ray
	{
		D3DXVECTOR3 Origin;	// 原点
		D3DXVECTOR3 Dir;	// 向き
	};

	// コンストラクタ・デストラクタ
	CInputMouse();
	~CInputMouse();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="hInstance">インスタンスハンドル</param>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <returns></returns>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void);
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void);
	/// <summary>
	/// 光線情報を更新
	/// </summary>
	/// <param name=""></param>
	void UpdateRay(void);
	/// <summary>
	/// 3D空間でのマウス位置を更新
	/// </summary>
	/// <param name=""></param>
	void UpdatePos3D(void);
	/// <summary>
	/// 押しているかどうか
	/// </summary>
	/// <param name="button_type">左か右か(0 = 左,1 = 右,2 = ホイールボタン,3 = サイドボタン,,,)</param>
	/// <returns></returns>
	bool OnPress(int button_type);
	/// <summary>
	/// 押した瞬間
	/// </summary>
	/// <param name="button_type">左か右か(0 = 左,1 = 右,2 = ホイールボタン,3 = サイドボタン,,,)</param>
	/// <returns></returns>
	bool OnDown(int button_type);
	/// <summary>
	/// 押し切った後
	/// </summary>
	/// <param name="button_type">左か右か(0 = 左,1 = 右,2 = ホイールボタン,3 = サイドボタン,,,)</param>
	/// <returns></returns>
	bool OnUp(int button_type);
	/// <summary>
	/// マウスが動いているかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>動いているかどうか</returns>
	bool IsMove(void);
	/// <summary>
	/// 光線情報を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>光線情報</returns>
	inline Ray GetRay(void) { return m_MouseRay; }
	/// <summary>
	/// 一フレーム前の位置
	/// </summary>
	/// <param name=""></param>
	/// <returns>位置</returns>
	D3DXVECTOR2 GetPosOld(void);
	/// <summary>
	/// 現在の位置
	/// </summary>
	/// <param name=""></param>
	/// <returns>位置</returns>
	D3DXVECTOR2 GetPos(void);
	/// <summary>
	/// マウスの移動ベクトルを取得(正規化されていない)
	/// </summary>
	/// <param name=""></param>
	/// <returns>移動ベクトル</returns>
	inline D3DXVECTOR2 GetVelocity(void) { return GetPos() - GetPosOld(); }
	/// <summary>
	/// 3D空間上での移動ベクトル
	/// </summary>
	/// <param name=""></param>
	/// <returns>ベクトル</returns>
	inline D3DXVECTOR3 GetVelocity3D(void) { return m_Pos3D - m_OldPos3D; }
	/// <summary>
	/// 3D空間上での一フレーム前の位置
	/// </summary>
	/// <param name=""></param>
	/// <returns>位置</returns>
	inline D3DXVECTOR3 GetPosOld3D(void) { return m_OldPos3D; }
	/// <summary>
	/// 3D空間上での現在の位置
	/// </summary>
	/// <param name=""></param>
	/// <returns>位置</returns>
	inline D3DXVECTOR3 GetPos3D(void) { return m_Pos3D; }
	/// <summary>
	/// マウスのステート
	/// </summary>
	/// <param name=""></param>
	/// <returns>ステート</returns>
	inline DIMOUSESTATE GetMouseState(void) { return m_MouseState; };
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

//****************************************************************
// 派生クラスを定義[ジョイパット]
//****************************************************************
class CInputJoypad : public CInput
{
public:
	//キーの種類
	typedef enum
	{
		JOYKEY_UP = 0,	// ↑
		JOYKEY_DOWN,	// ↓
		JOYKEY_LEFT,	// ←
		JOYKEY_RIGET,	// →
		JOYKEY_START,	// スタートボタン
		JOYKEY_BACK,	// バックボタン
		JOYKEY_L3,		// 左スティック押し込み
		JOYKEY_R3,		// 右スティック押し込み
		JOYKEY_L1,		// L1
		JOYKEY_R1,		// R1
		JOYKEY_L2,		// L2
		JOYKEY_R2,		// R2
		JOYKEY_A,		// Aボタン
		JOYKEY_B,		// Bボタン
		JOYKEY_X,		// Xボタン
		JOYKEY_Y,		// Yボタン
		JOYKEY_MAX		// 最大数
	}JOYKEY;

	// コンストラクタ・デストラクタ
	CInputJoypad();
	~CInputJoypad();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="hInstance">インスタンスハンドル</param>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <returns></returns>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
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
	/// バイブレーション開始
	/// </summary>
	/// <param name="R">右モーターの揺れの強さ</param>
	/// <param name="L">左モーターの揺れの強さ</param>
	/// <param name="Frame">フレーム</param>
	void BeginVibration(const float R = 0.5f, const float L = 0.5f, const int Frame = 1);
	/// <summary>
	/// ボタンを押しているかどうか
	/// </summary>
	/// <param name="nKey">判別したいキー</param>
	/// <returns>押しているかどうか</returns>
	bool GetPress(JOYKEY nKey);
	/// <summary>
	/// ボタンを押した瞬間
	/// </summary>
	/// <param name="nKey">判別するキー</param>
	/// <returns>押しているかどうか</returns>
	bool GetTrigger(JOYKEY nKey);
	/// <summary>
	/// 押し切った瞬間
	/// </summary>
	/// <param name="nKey">判別するキー</param>
	/// <returns>押しているかどうか</returns>
	bool GetRelease(JOYKEY nKey);
	/// <summary>
	/// Lスティックが動いているかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>動いているかどうか</returns>
	bool GetJoyStickL(void);
	/// <summary>
	/// Rスティックが動いているかどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>動いているかどうか</returns>
	bool GetJoyStickR(void);
	/// <summary>
	/// スティックのステートを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>ステート</returns>
	inline XINPUT_STATE* GetJoyStickAngle(void) {	return &m_JoyKeyState;	};
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