//*************************************
//
//　入力処理[input.cpp]
//　Author:Hatori Kensaku
//
//*************************************

// インクルード
#include "input.h"
#include "manager.h"

// 静的メンバ変数宣言
LPDIRECTINPUT8 CInput::m_pInput = NULL;
LPDIRECTINPUT8 CInputMouse::m_pInputMouse = NULL;

//*********************************************
// インプットクラスのコンストラクタ
//*********************************************
CInput::CInput()
{
	m_pDevice = NULL;
}

//*********************************************
// インプットクラスのデストラクタ
//*********************************************
CInput::~CInput()
{

}

//*********************************************
// インプットクラスの初期化処理
//*********************************************
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		//キーボード初期化
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
		//
		if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
		{
			return E_FAIL;
		}
		//データフォーマットを設定
		if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
		{
			return E_FAIL;
		}
		//協調モードを設定
		if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}

		//キーボードへのアクセス権を獲得
		m_pDevice->Acquire();

		return S_OK;
	}
	return E_FAIL;
}

//*********************************************
// インプットクラスの終了処理
//*********************************************
void CInput::Uninit(void)
{
	//入力デバイスの破棄
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	//DirectInputオブジェクトの破棄
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}


//*********************************************
//*		 派生クラスの処理[マウス]			  *
//*********************************************

//*********************************************
// インプットマウスクラスのコンストラクタ
//*********************************************
CInputMouse::CInputMouse()
{
	m_pInputMouse = NULL;
	m_pDeviceMouse = NULL;
	//メモリのクリア
	memset(&m_MouseOldState, NULL, sizeof(DIMOUSESTATE));
	memset(&m_MouseState, NULL, sizeof(DIMOUSESTATE));
}

//*********************************************
// インプットマウスクラスのデストラクタ
//*********************************************
CInputMouse::~CInputMouse()
{

}

//*********************************************
// インプットマウスクラスの初期化処理
//*********************************************
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//キーボード初期化
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputMouse, NULL)))
	{
		return E_FAIL;
	}
	//
	if (FAILED(m_pInputMouse->CreateDevice(GUID_SysMouse, &m_pDeviceMouse, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(m_pDeviceMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}
	//協調モードを設定
	if (FAILED(m_pDeviceMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	m_pDeviceMouse->Acquire();

	//メモリのクリア
	memset(&m_MouseOldState, NULL, sizeof(DIMOUSESTATE));
	memset(&m_MouseState, NULL, sizeof(DIMOUSESTATE));

	return S_OK;
}

//*********************************************
// インプットマウスクラスの終了処理
//*********************************************
void CInputMouse::Uninit(void)
{
	//入力デバイスの破棄
	if (m_pDeviceMouse != NULL)
	{
		m_pDeviceMouse->Unacquire();
		m_pDeviceMouse->Release();
		m_pDeviceMouse = NULL;
	}
	//DirectInputオブジェクトの破棄
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Release();
		m_pInputMouse = NULL;
	}
}


//*********************************************
// インプットマウスクラスの更新処理
//*********************************************
void CInputMouse::Update(void)
{
	// 更新前に最新マウス情報を保存する
	m_MouseOldState = m_MouseState;

	// 最新のマウスの状態を更新
	HRESULT	hr = m_pDeviceMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
	if (FAILED(hr))
	{
		m_pDeviceMouse->Acquire();
	}
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &p);
	m_MouseState.lX = p.x;
	m_MouseState.lY = p.y;

	UpdateRay();
	UpdatePos3D();
}

//*********************************************
// レイの情報を更新
//*********************************************
void CInputMouse::UpdateRay(void)
{
	// レイの始点と向きを代入するための変数
	D3DXVECTOR3 RayOrigin, RayDir;

	// ビューポート
	D3DVIEWPORT9 ViewPort;

	// 取得
	CManager::GetRenderer()->GetDevice()->GetViewport(&ViewPort);

	// ビューマトリックス、プロジェクションマトリックス、ワールドマトリックスを設定するための変数
	D3DXMATRIX ProjMtx, ViewMtx, WorldMtx;

	// 取得
	CManager::GetRenderer()->GetDevice()->GetTransform(D3DTS_PROJECTION, &ProjMtx);
	CManager::GetRenderer()->GetDevice()->GetTransform(D3DTS_VIEW, &ViewMtx);

	// 初期化
	D3DXMatrixIdentity(&WorldMtx);

	// マウス座標を代入
	D3DXVECTOR3 ScreenPoint((float)m_MouseState.lX, (float)m_MouseState.lY, 0.0f);

	// 始点として変換
	D3DXVec3Unproject(&RayOrigin, &ScreenPoint, &ViewPort, &ProjMtx, &ViewMtx, &WorldMtx);
	
	// 終点として変換
	ScreenPoint.z = 1.0f;

	// 変換
	D3DXVec3Unproject(&RayDir, &ScreenPoint, &ViewPort, &ProjMtx, &ViewMtx, &WorldMtx);

	// ベクトルを引く
	RayDir -= RayOrigin;

	// 正規化
	D3DXVec3Normalize(&RayDir, &RayDir);

	// 情報を代入
	m_MouseRay.Dir = RayDir;
	m_MouseRay.Origin = RayOrigin;
}

//*********************************************
// 3D座標上のマウスの位置の更新
//*********************************************
void CInputMouse::UpdatePos3D(void)
{
	// ビューポート
	D3DVIEWPORT9 ViewPort;

	// 取得
	CManager::GetRenderer()->GetDevice()->GetViewport(&ViewPort);

	// ビューマトリックス、プロジェクションマトリックス、ワールドマトリックスを設定するための変数
	D3DXMATRIX ProjMtx, ViewMtx, WorldMtx;

	// 取得
	CManager::GetRenderer()->GetDevice()->GetTransform(D3DTS_PROJECTION, &ProjMtx);
	CManager::GetRenderer()->GetDevice()->GetTransform(D3DTS_VIEW, &ViewMtx);

	// 初期化
	D3DXMatrixIdentity(&WorldMtx);

	// マウス座標を代入
	D3DXVECTOR3 Pos3D((float)m_MouseState.lX, (float)m_MouseState.lY, 0.0f);
	D3DXVECTOR3 Pos3DOld((float)m_MouseOldState.lX, (float)m_MouseOldState.lY, 0.0f);

	// 始点として変換
	D3DXVec3Unproject(&Pos3D, &Pos3D, &ViewPort, &ProjMtx, &ViewMtx, &WorldMtx);

	// 変換
	D3DXVec3Unproject(&Pos3DOld, &Pos3DOld, &ViewPort, &ProjMtx, &ViewMtx, &WorldMtx);

	// 情報を代入
	m_Pos3D = Pos3D;
	m_OldPos3D = Pos3DOld;
}

//*********************************************
// マウスのプレス情報を取得
//*********************************************
bool CInputMouse::OnPress(int button_type)
{
	return(m_MouseState.rgbButtons[button_type] & (0x80)) ? true : false;
}

//*********************************************
// マウスの押した瞬間
//*********************************************
bool CInputMouse::OnDown(int button_type)
{
	if (!(m_MouseOldState.rgbButtons[button_type] & (0x80)) &&
		m_MouseState.rgbButtons[button_type] & (0x80))
	{
		return true;
	}

	return false;
}

//*********************************************
// マウスの離した瞬間
//*********************************************
bool CInputMouse::OnUp(int button_type)
{
	if (m_MouseOldState.rgbButtons[button_type] & (0x80) &&
		!(m_MouseState.rgbButtons[button_type] & (0x80)))
	{
		return true;
	}

	return false;
}

//*********************************************
// マウスの位置を取得
//*********************************************
D3DXVECTOR2 CInputMouse::GetPos(void)
{
	return D3DXVECTOR2((float)m_MouseState.lX, (float)m_MouseState.lY);
}

//*********************************************
// マウスが動いているかどうか
//*********************************************
bool CInputMouse::IsMove(void)
{
	if (m_MouseState.lX == m_MouseOldState.lX &&
		m_MouseState.lY == m_MouseOldState.lY)
	{
		return false;
	}
	return true;
}

//*********************************************
// マウスの昔の位置を取得
//*********************************************
D3DXVECTOR2 CInputMouse::GetPosOld(void)
{
	return D3DXVECTOR2((float)m_MouseOldState.lX, (float)m_MouseOldState.lY);
}


//*********************************************
//*		 派生クラスの処理[キーボード]		  *
//*********************************************

//*********************************************
// インプットキーボードクラスのコンストラクタ
//*********************************************
CInputKeyboard::CInputKeyboard()
{
	for (int nCount = 0; nCount < InputConfig::NumKey; nCount++)
	{
		m_aKeyState[nCount] = NULL;
		m_OldaKeyState[nCount] = NULL;
	}
}

//*********************************************
// インプットキーボードクラスのデストラクタ
//*********************************************
CInputKeyboard::~CInputKeyboard()
{

}

//*********************************************
// インプットキーボードクラスの初期化処理
//*********************************************
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = CInput::Init(hInstance, hWnd);
	return hr;
}

//*********************************************
// インプットキーボードクラスの終了処理
//*********************************************
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}


//*********************************************
// インプットキーボードクラスの更新処理
//*********************************************
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[InputConfig::NumKey] = {};
	int nCntKey;

	for (nCntKey = 0; nCntKey < InputConfig::NumKey; nCntKey++)
	{
		m_OldaKeyState[nCntKey] = m_aKeyState[nCntKey];
	}

	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < InputConfig::NumKey; nCntKey++)
		{
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//*********************************************
//キーボードのプレス情報を取得
//*********************************************
bool CInputKeyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x080) ? true : false;
}

//*********************************************
//キーボードのトリガー情報を取得
//*********************************************
bool CInputKeyboard::GetTrigger(int nKey)
{
	bool Trigger = false;
	if (m_aKeyState[nKey] & 0x80 && !(m_OldaKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//*********************************************
//キーボードのリピート情報を取得
//*********************************************
bool CInputKeyboard::GetRepeat(int nKey)
{
	bool Trigger = false;
	if (m_OldaKeyState[nKey] & 0x80 && (m_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//*********************************************
//キーボードのリリース情報を取得
//*********************************************
bool CInputKeyboard::GetRelease(int nKey)
{
	bool Trigger = false;
	if (m_OldaKeyState[nKey] & 0x80 && !(m_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}


//*********************************************
//*		 派生クラスの処理[ジョイパット]		  *
//*********************************************

//*********************************************
// インプットジョイパットクラスのコンストラクタ
//*********************************************
CInputJoypad::CInputJoypad()
{
	//メモリのクリア
	memset(&m_JoyKeyState, NULL, sizeof(XINPUT_STATE));
	memset(&m_JoyKeyStateOld, NULL, sizeof(XINPUT_STATE));
}

//*********************************************
// インプットジョイパットクラスのデストラクタ
//*********************************************
CInputJoypad::~CInputJoypad()
{

}

//*********************************************
// インプットジョイパットクラスの初期化処理
//*********************************************
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = CInput::Init(hInstance, hWnd);
	//メモリのクリア
	memset(&m_JoyKeyState, NULL, sizeof(XINPUT_STATE));
	memset(&m_JoyKeyStateOld, NULL, sizeof(XINPUT_STATE));

	//XInputのステートを設定(有効)
	XInputEnable(true);

	return hr;
}

//*********************************************
// インプットジョイパットクラスの終了処理
//*********************************************
void CInputJoypad::Uninit(void)
{
	CInput::Uninit();
	//XInputのステートを設定(無効)
	XInputEnable(false);
}


//*********************************************
// インプットジョイパットクラスの更新処理
//*********************************************
void CInputJoypad::Update(void)
{
	XINPUT_STATE joykeyState;
	//
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		m_JoyKeyStateOld = m_JoyKeyState;
		m_JoyKeyState = joykeyState;
	}
}

//*********************************************
// ジョイパットのプレス情報を取得
//*********************************************
bool CInputJoypad::GetPress(JOYKEY nKey)
{
	switch (nKey)
	{
	case JOYKEY_L2:
		return (m_JoyKeyState.Gamepad.bLeftTrigger) ? true : false;
	case JOYKEY_R2:
		return (m_JoyKeyState.Gamepad.bRightTrigger) ? true : false;
	default:
		break;
	}
	return (m_JoyKeyState.Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//*********************************************
// ジョイパットのトリガー情報を取得
//*********************************************
bool CInputJoypad::GetTrigger(JOYKEY nKey)
{
	switch (nKey)
	{
	case JOYKEY_L2:
		return (m_JoyKeyState.Gamepad.bLeftTrigger >= 30 && m_JoyKeyStateOld.Gamepad.bLeftTrigger < 30);
	case JOYKEY_R2:
		return (m_JoyKeyState.Gamepad.bRightTrigger >= 30 && m_JoyKeyStateOld.Gamepad.bRightTrigger < 30);
	default:
		break;
	}
	return ((m_JoyKeyState.Gamepad.wButtons & (0x01 << nKey)) && !(m_JoyKeyStateOld.Gamepad.wButtons & (0x01 << nKey))) ? true : false;
}

//*********************************************
// ジョイパットのリリース情報を取得
//*********************************************
bool CInputJoypad::GetRelease(JOYKEY nKey)
{
	switch (nKey)
	{
	case JOYKEY_L2:
		return (m_JoyKeyState.Gamepad.bLeftTrigger < 30 && m_JoyKeyStateOld.Gamepad.bLeftTrigger >= 30);
	case JOYKEY_R2:
		return (m_JoyKeyState.Gamepad.bRightTrigger < 30 && m_JoyKeyStateOld.Gamepad.bRightTrigger >= 30);
	default:
		break;
	}
	return (!(m_JoyKeyState.Gamepad.wButtons & (0x01 << nKey)) && (m_JoyKeyStateOld.Gamepad.wButtons & (0x01 << nKey))) ? true : false;
}

//*********************************************
// ジョイパットがデッドゾーンいないか(L)
//*********************************************
bool CInputJoypad::GetJoyStickL(void)
{
	bool joykey = false;

	if (m_JoyKeyState.Gamepad.sThumbLX >= InputConfig::DeadZone
		|| m_JoyKeyState.Gamepad.sThumbLX <= -InputConfig::DeadZone
		|| m_JoyKeyState.Gamepad.sThumbLY >= InputConfig::DeadZone
		|| m_JoyKeyState.Gamepad.sThumbLY <= -InputConfig::DeadZone)
	{
		joykey = true;
	}
	return joykey;
}

//*********************************************
// ジョイパットがデッドゾーンいないか(R)
//*********************************************
bool CInputJoypad::GetJoyStickR(void)
{
	bool joykey = false;

	if (m_JoyKeyState.Gamepad.sThumbRX >= InputConfig::DeadZone
		|| m_JoyKeyState.Gamepad.sThumbRX <= -InputConfig::DeadZone
		|| m_JoyKeyState.Gamepad.sThumbRY >= InputConfig::DeadZone
		|| m_JoyKeyState.Gamepad.sThumbRY <= -InputConfig::DeadZone)
	{
		joykey = true;
	}
	return joykey;
}