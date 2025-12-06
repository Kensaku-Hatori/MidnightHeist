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
const D3DXVECTOR3 CCamera::Config::Title::PosV = { -400.0f,20.0f ,1020.0f };
const D3DXVECTOR3 CCamera::Config::Title::PosR = { 200.0f,375.0f ,170.0f };
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

	// 読み込み
	LoadMotion("data\\TEXT\\CameraWork\\CameraWork.txt");

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

	// ムービー中だったら
	if (m_isMovie == true)
	{
		// エンターですぐに飛ばす
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputMouse()->OnDown(1) ||
			CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_BACK) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
		{
			// ムービー中のフラグをfalseにする
			m_isMovie = false;
		}

		// ムービー再生
		UpdateMotion();

		// 処理を終わる
		return;
	}

	// それぞれの更新処理を呼ぶ
	UpdateMouseMove();
	//UpdateJoyPadMove();

	//UpdateCameraPositionV();
	//UpdateCameraPositionR();

	// ホイールでカメラの距離を変える
	SetMouseWheel(m_pInputMouse->GetMouseState().lZ);
}

//***************************************
// 終了処理
//***************************************
void CCamera::Uninit(void)
{
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
// 敵の弱点に対してエイムアシストする
//***************************************
D3DXVECTOR3 CCamera::CalcAssistVec(void)
{
	// エイムアシストベクトルの一時変数
	D3DXVECTOR3 AimAssist = VEC3_NULL;

	// NULLを返す
	return VEC3_NULL;
}

//***************************************
// アイムアシストする
//***************************************
void CCamera::Assist(D3DXVECTOR3 AssistVec, float Strength)
{
	// エイムアシストを使いやすいスケールに直す
	const float NormalizeScale = 0.005f;

	// エイムアシスト実行
	m_rot.x += (AssistVec.y * NormalizeScale) * Strength;
	m_rot.y += (AssistVec.x * NormalizeScale) * Strength;
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
// ムービーを設定
//***************************************
void CCamera::SetMovie(MOTIONTYPE Type)
{
	// モーションの種類を設定
	m_nMotionType = Type;

	// キーを初期化
	m_nKey = 0;

	// 次のキーを初期化
	m_nNextKey = 0;

	// モーションカウンタを初期化
	m_nCounterMotion = 0;

	// 全体フレームを初期化
	m_nAllFrame = 0;

	// 終了フラグを初期化
	m_bFinishMotion = false;

	// ムービーフラグを設定
	m_isMovie = true;
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
// モーション読み込み
//***************************************
void CCamera::LoadMotion(std::string Path)
{
	// ファイルを開く
	ifstream ifs(Path);

	// 開けなかったら
	if (!ifs)
	{
		MessageBox(NULL, "ファイルが読み込めませんでした", "終了メッセージ", MB_OK);
		return;
	}

	// 行を読み込む用の変数
	string line = {};

	// パラメーター、イコール用の変数
	string label = {}, equal = {};

	// キーカウントを宣言
	int KeyCount = 0;

	// モーションを代入する変数を宣言
	MOTIONINFO LocalMotion;

	// 終了まで繰り返す
	while (getline(ifs, line))
	{
		// 読み込むためのフォーマと鬼変換
		istringstream iss(line);

		// キーワードを計算
		if (line.find("LOOP") != string::npos)
		{
			// ループするかどうかを読み込む
			iss >> label >> equal >> LocalMotion.bLoop;
		}
		if (line.find("NUMKEY") != string::npos)
		{
			// キーの総数を読み込む
			iss >> label >> equal >> LocalMotion.nNumKey;

			// ベクターのサイズを確保
			LocalMotion.pKeyInfo.resize(LocalMotion.nNumKey);
		}
		if (line.find("POSV") != string::npos)
		{
			// 始点の位置を読み込む
			iss >> label >> equal >> LocalMotion.pKeyInfo[KeyCount].fPosVX >> LocalMotion.pKeyInfo[KeyCount].fPosVY >> LocalMotion.pKeyInfo[KeyCount].fPosVZ;
		}
		if (line.find("POSR") != string::npos)
		{
			// 注視点の位置を読み込む
			iss >> label >> equal >> LocalMotion.pKeyInfo[KeyCount].fPosRX >> LocalMotion.pKeyInfo[KeyCount].fPosRY >> LocalMotion.pKeyInfo[KeyCount].fPosRZ;
		}
		if (line.find("FRAME") != string::npos)
		{
			// フレーム数を読み込む
			iss >> label >> equal >> LocalMotion.pKeyInfo[KeyCount].nFrame;
		}
		if (line.find("END_KEY") != string::npos)
		{
			// きーかうんとをインクリメント
			KeyCount++;
		}
		if (line.find("END_CAMERAWORK") != string::npos)
		{
			// キーカウントを初期化
			KeyCount = 0;

			// 連結
			m_vMotionInfo.push_back(LocalMotion);

			// ローカル変数を初期化
			LocalMotion = {};
		}
	}
}

//***************************************
// PosVの座標更新
//***************************************
void CCamera::UpdateCameraPositionV()
{
	// 視点の座標更新、高さだけより離す
	m_posVDest.x = m_posRDest.x + cosf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	m_posVDest.y = m_posRDest.y + sinf(m_rot.x) * m_fDistance;
	m_posVDest.z = m_posRDest.z + cosf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	m_posV.x += (m_posVDest.x - m_posV.x) * m_fSpeedV;
	m_posV.y += (m_posVDest.y - m_posV.y) * m_fSpeedV;
	m_posV.z += (m_posVDest.z - m_posV.z) * m_fSpeedV;
}

//***************************************
// PosRの座標更新
//***************************************
void CCamera::UpdateCameraPositionR()
{
	//// 視点の座標更新、高さだけ高く設定する
	//m_posRDest.x = m_posV.x - cosf(m_rot.x) * sinf(m_rot.y) * m_fDistance;
	//m_posRDest.y = m_posV.y - sinf(m_rot.x) * m_fDistance;
	//m_posRDest.z = m_posV.z - cosf(m_rot.x) * cosf(m_rot.y) * m_fDistance;

	//// 注視点の更新
	//m_posR.x += (m_posRDest.x - m_posR.x) * Config::CatchSpeedR.x;
	//m_posR.y += (m_posRDest.y - m_posR.y) * Config::CatchSpeedR.y;
	//m_posR.z += (m_posRDest.z - m_posR.z) * Config::CatchSpeedR.z;
}

//***************************************
// モーションを再生
//***************************************
void CCamera::UpdateMotion(void)
{
	// モーションカウンタをインクリメント
	m_nCounterMotion++;

	// 全体フレームをインクリメント
	m_nAllFrame++;

	// キーの総数が読み込めていたら次のキーを設定
	if (m_vMotionInfo[m_nMotionType].nNumKey != NULL) m_nNextKey = (m_nKey + 1) % m_vMotionInfo[m_nMotionType].nNumKey;

	// 次のキーに進める
	if (m_nCounterMotion >= m_vMotionInfo[m_nMotionType].pKeyInfo[m_nKey].nFrame)
	{
		// カウンタを初期化
		m_nCounterMotion = 0;

		// キーの総数が読み込めていたらキーを進める
		if (m_vMotionInfo[m_nMotionType].nNumKey != NULL) m_nKey = (m_nKey + 1) % m_vMotionInfo[m_nMotionType].nNumKey;

		// キーの総数に達したら
		if (m_nKey >= m_vMotionInfo[m_nMotionType].nNumKey - 1)
		{
			// 全体フレームを初期化
			m_nAllFrame = 0;

			// ループしないタイプだったら
			if (m_vMotionInfo[m_nMotionType].bLoop == false)
			{
				// 終了状態にする
				m_bFinishMotion = true;
			}
		}
	}

	// 今のキーと次のキーの一時変数
	CCamera::KEY nKey = m_vMotionInfo[m_nMotionType].pKeyInfo[m_nKey];
	CCamera::KEY nNexKey = m_vMotionInfo[m_nMotionType].pKeyInfo[m_nNextKey];

	// 差分と目標の一時変数
	CCamera::KEY KeyDef;
	CCamera::KEY KeyDest;

	// 差分
	KeyDef.fPosRX = nNexKey.fPosRX - nKey.fPosRX;
	KeyDef.fPosRY = nNexKey.fPosRY - nKey.fPosRY;
	KeyDef.fPosRZ = nNexKey.fPosRZ - nKey.fPosRZ;

	KeyDef.fPosVX = nNexKey.fPosVX - nKey.fPosVX;
	KeyDef.fPosVY = nNexKey.fPosVY - nKey.fPosVY;
	KeyDef.fPosVZ = nNexKey.fPosVZ - nKey.fPosVZ;

	// 希望の値
	KeyDest.fPosVX = nKey.fPosVX + KeyDef.fPosVX * ((float)m_nCounterMotion / (float)nKey.nFrame);
	KeyDest.fPosVY = nKey.fPosVY + KeyDef.fPosVY * ((float)m_nCounterMotion / (float)nKey.nFrame);
	KeyDest.fPosVZ = nKey.fPosVZ + KeyDef.fPosVZ * ((float)m_nCounterMotion / (float)nKey.nFrame);

	// 希望の値
	KeyDest.fPosRX = nKey.fPosRX + KeyDef.fPosRX * ((float)m_nCounterMotion / (float)nKey.nFrame);
	KeyDest.fPosRY = nKey.fPosRY + KeyDef.fPosRY * ((float)m_nCounterMotion / (float)nKey.nFrame);
	KeyDest.fPosRZ = nKey.fPosRZ + KeyDef.fPosRZ * ((float)m_nCounterMotion / (float)nKey.nFrame);

	// 足す
	m_posV.x = KeyDest.fPosVX;
	m_posV.y = KeyDest.fPosVY;
	m_posV.z = KeyDest.fPosVZ;

	// 足す
	m_posR.x = KeyDest.fPosRX;
	m_posR.y = KeyDest.fPosRY;
	m_posR.z = KeyDest.fPosRZ;

	// 終了していたら
	if (m_bFinishMotion == true)
	{
		// ムービーフラグを切り替える
		m_isMovie = false;
	}
}