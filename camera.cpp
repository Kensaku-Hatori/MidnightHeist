//****************************************************************
//
// カメラの処理[camera.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "camera.h"
#include "manager.h"
#include "math.h"

// 名前空間
using namespace std;

// 定数を設定
const D3DXVECTOR3 CCamera::Config::Title::PosV = { 400.0f,20.0f ,1020.0f };
const D3DXVECTOR3 CCamera::Config::Title::PosR = { 200.0f,0.0f ,170.0f };
const D3DXVECTOR3 CCamera::Config::Title::Rot = { -D3DX_PI * 0.15f,-D3DX_PI * 0.25f ,0.0f };

const D3DXVECTOR3 CCamera::Config::Game::PosV = { 0.0f,1280.0f ,800.0f };
const D3DXVECTOR3 CCamera::Config::Game::PosR = VEC3_NULL;
const D3DXVECTOR3 CCamera::Config::Game::Rot = { D3DX_PI * 0.35f,0.0f ,0.0f };

// 静的メンバ変数宣言
CInputKeyboard* CCamera::m_pInputKeyboard = CManager::GetInputKeyboard();
CInputJoypad* CCamera::m_pInputJoypad = CManager::GetInputJoypad();
CInputMouse* CCamera::m_pInputMouse = CManager::GetInputMouse();

//***************************************
// コンストラクタ
//***************************************
CCamera::CCamera()
{
	// カメラの初期化の値
	m_posV = VEC3_NULL;
	m_posVDest = VEC3_NULL;
	m_posR = VEC3_NULL;
	m_posRDest = VEC3_NULL;
	m_vecU = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_fDistance = NULL;

	// 視野角、視界の広さの初期値を設定
	m_fFov = NULL;
	m_fZnear = NULL;
	m_fZfar = NULL;
}

//***************************************
// デストラクタ
//***************************************
CCamera::~CCamera()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CCamera::Init(void)
{
	// カメラの初期化の値
	m_posV = VEC3_NULL;
	m_posVDest = VEC3_NULL;
	m_posR = VEC3_NULL;
	m_posRDest = VEC3_NULL;
	m_vecU = VEC_UP;
	m_rot = { D3DX_PI * 0.35f,0.0f,0.0f };

	// 視点の補完速度を初期化
	m_fSpeedV = CCamera::Config::Defoult::SpeedV;

	// 注視点までの距離
	m_fDistance = Config::Defoult::Distance;

	// 視野角、視界の広さの初期値を設定
	m_fFov = Config::Defoult::Fov;
	m_fZnear = Config::Defoult::Near;
	m_fZfar = Config::Defoult::Far;

	// デバイスを取得
	m_pInputKeyboard = CManager::GetInputKeyboard();
	m_pInputJoypad = CManager::GetInputJoypad();
	m_pInputMouse = CManager::GetInputMouse();

	// 正常終了
	return S_OK;
}

//***************************************
// 更新処理
//***************************************
void CCamera::Update(void)
{
	// ポーズ中だったら早期リターン
	if (CManager::isPause() == true) return;

	for (auto& Systems : m_SystemList)
	{
		Systems->Update();
	}

	// それぞれの更新処理を呼ぶ
	//UpdateMouseMove();
	//UpdateJoyPadMove();

	UpdateCameraPositionV();
	UpdateCameraPositionR();

	// ホイールでカメラの距離を変える
	//SetMouseWheel(m_pInputMouse->GetMouseState().lZ);
}

//***************************************
// 終了処理
//***************************************
void CCamera::Uninit(void)
{
}

//***************************************
// システムの終了処理
//***************************************
void CCamera::EndSystems(void)
{
	for (auto& Systems : m_SystemList)
	{
		Systems->Uninit();
		Systems.reset();
	}
	m_SystemList.clear();
}

//***************************************
// カメラの設定処理
//***************************************
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(m_fFov),						//視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, //アスペクト比
		m_fZnear,
		m_fZfar);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//***************************************
// カメラのマウス移動
//***************************************
void CCamera::UpdateMouseMove(void)
{
	// マウスの座標を取得
	D3DXVECTOR2 Pos = CManager::GetInputMouse()->GetPos();
	D3DXVECTOR2 OldPos = CManager::GetInputMouse()->GetPosOld();

	// 1フレームの移動量を計算
	D3DXVECTOR2 DiffMouse = D3DXVECTOR2((FLOAT)Pos.x - (FLOAT)OldPos.x,
		(FLOAT)Pos.y - (FLOAT)OldPos.y);

	// 視点を動かせるかどうか
	if (isMovePosV() == true)
	{
		// 移動量に係数をかけてちょうどよくする
		DiffMouse *= Config::MouseSpeedV;

		// 一回転しないようにする
		if (m_rot.x + DiffMouse.y < D3DX_PI * Config::RotLimit &&
			m_rot.x + DiffMouse.y > -D3DX_PI * Config::RotLimit)
		{
			// 向きを動かす
			m_rot.x += DiffMouse.y;
		}
		// 向きを動かす
		m_rot.y += DiffMouse.x;

		UpdateCameraPositionV();
	}
	// 注視点を動かせるかどうか
	else if (isMovePosR() == true)
	{
		// 移動量に係数をかけてちょうどよくする
		DiffMouse *= Config::MouseSpeedR;

		// 一回転しないようにする
		if (m_rot.x + DiffMouse.y < D3DX_PI * Config::RotLimit &&
			m_rot.x + DiffMouse.y > -D3DX_PI * Config::RotLimit)
		{
			// 向きを動かす
			m_rot.x += DiffMouse.y;
		}
		// 向きを動かす
		m_rot.y += DiffMouse.x;

		UpdateCameraPositionR();
	}

	if (isSlideMove() == true)
	{
		D3DXVECTOR3 VecZ = m_posRDest - m_posVDest;
		D3DXVECTOR3 VecX, VecY, Move;
		Move = VEC3_NULL;
		D3DXVec3Cross(&VecX, &VecZ, &m_vecU);
		D3DXVec3Normalize(&VecX, &VecX);
		D3DXVec3Cross(&VecY, &VecX, &VecZ);
		D3DXVec3Normalize(&VecY, &VecY);
		Move += VecX * m_pInputMouse->GetVelocity().x;
		Move += VecY * m_pInputMouse->GetVelocity().y;
		Move *= 0.5f;
		m_posRDest += Move;
		m_posVDest += Move;
		m_posR += Move;
		m_posV += Move;
	}
	// 正規化
	NormalizeCameraRot();
}

//***************************************
// カメラのゲームパッド移動
//***************************************
void CCamera::UpdateJoyPadMove(void)
{
	// マウスカーソルの戻す位置
	const POINT SetMousePos = { (LONG)SCREEN_WIDTH / (LONG)2.0f,(LONG)SCREEN_HEIGHT / (LONG)2.0f };

	// パッドのスティックの角度を取得
	XINPUT_STATE* pStick = m_pInputJoypad->GetJoyStickAngle();

	// スティックが動いていたら
	if (m_pInputJoypad->GetJoyStickR() == true)
	{
		// スティックの位置を取得
		float XAngle = pStick->Gamepad.sThumbRX;
		float YAngle = pStick->Gamepad.sThumbRY;

		// 倒した深さを取得
		float Distance = sqrtf((XAngle * XAngle) + (YAngle * YAngle));

		// 最低限の深さを設定
		float DeadZone = 32767.0f * 0.0005f;

		// 深さが一定だったら
		if (Distance > DeadZone)
		{
			// 角度を計算
			float Angle = atan2f(XAngle, YAngle);

			// 上まで行かなかったら
			if (m_rot.x - cosf(-Angle) * Distance * Config::PadSpeed < D3DX_PI * Config::RotLimit &&
				m_rot.x - cosf(-Angle) * Distance * Config::PadSpeed > -D3DX_PI * Config::RotLimit)
			{
				// 向きを動かす
				m_rot.x -= cosf(Angle) * Distance * Config::PadSpeed;
			}
			// 向きを動かす
			m_rot.y += sinf(Angle) * Distance * Config::PadSpeed;
		}
	}

	// 正規化
	NormalizeCameraRot();
}

//***************************************
// カメラの向きの正規化
//***************************************
void CCamera::NormalizeCameraRot(void)
{
	// 角度の正規化(X)
	if (m_rot.x > D3DX_PI)
	{
		m_rot.x -= D3DX_PI * 2.0f;
	}
	else if (m_rot.x < -D3DX_PI)
	{
		m_rot.x += D3DX_PI * 2.0f;
	}

	// 角度の正規化(Y)
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	// 角度の正規化(Z)
	if (m_rot.z > D3DX_PI)
	{
		m_rot.z -= D3DX_PI * 2.0f;
	}
	else if (m_rot.z < -D3DX_PI)
	{
		m_rot.z += D3DX_PI * 2.0f;
	}
}

//***************************************
// カメラの距離を変える
//***************************************
void CCamera::SetMouseWheel(int zDelta)
{
	if (zDelta == 0) return;

	// 距離を反映
	m_fDistance += zDelta * Config::WheelSpeed;

	// 座標を更新
	m_posV.x = m_posR.x + cosf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posV.y = m_posR.y + sinf(m_rot.x) * m_fDistance;
	m_posV.z = m_posR.z + cosf(m_rot.x) * cosf(m_rot.y) * m_fDistance;
}

//***************************************
// システムの追加
//***************************************
void CCamera::AddSystem(CBaceSystem* _Add)
{
	// ポインタ生成
	std::unique_ptr<CBaceSystem> Add(_Add);
	// オーナー設定
	Add->SetOwner(this);
	// 初期化
	Add->Init();
	// 所有権移動
	m_SystemList.push_back(std::move(Add));
}

//***************************************
// ビュー系のマトリックスやビューポートをリセット
//***************************************
void CCamera::ResetProjectionMtx(void)
{
	// 設定する用のマトリックス
	D3DXMATRIX ViewMtx, ProjectionMtx;

	// アスペクト比を設定
	float fAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 始点、注視点、上向きの方向ベクトルを設定
	D3DXVECTOR3 PosV, PosR, VecU;
	PosV = CManager::GetCamera()->GetPosV();
	PosR = CManager::GetCamera()->GetPosR();
	VecU = { 0.0f,1.0f,0.0f };

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&ViewMtx);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&ViewMtx,
		&PosV,
		&PosR,
		&VecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &ViewMtx);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&ProjectionMtx);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&ProjectionMtx,			// マトリックス
		D3DXToRadian(CCamera::Config::Defoult::Fov),	// 視野角
		fAspect,										// アスペクト比
		CCamera::Config::Defoult::Near,					// ニア
		CCamera::Config::Defoult::Far);					// ファー

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &ProjectionMtx);
}

//***************************************
// PosVの座標更新
//***************************************
void CCamera::UpdateCameraPositionV()
{
	if (CMath::IsValidVector(m_posR) == false)
	{
		m_posV = m_posVDest;
	}
	m_posV.x += (m_posVDest.x - m_posV.x) * m_fSpeedV;
	m_posV.y += (m_posVDest.y - m_posV.y) * m_fSpeedV;
	m_posV.z += (m_posVDest.z - m_posV.z) * m_fSpeedV;
}

//***************************************
// PosRの座標更新
//***************************************
void CCamera::UpdateCameraPositionR()
{
	if (CMath::IsValidVector(m_posR) == false)
	{
		m_posR = m_posRDest;
	}
	// 注視点の更新
	m_posR.x += (m_posRDest.x - m_posR.x) * m_fSpeedV;
	m_posR.y += (m_posRDest.y - m_posR.y) * m_fSpeedV;
	m_posR.z += (m_posRDest.z - m_posR.z) * m_fSpeedV;
}