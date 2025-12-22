//*************************************
//
//　Manager[manager.h]
//　Author:Hatori Kensaku
//
//*************************************

// 二十インクルード防止
#ifndef _MANAGER_H_
#define _MANAGER_H_

// インクルード
#include "main.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "line.h"

// 前方宣言
class CObject3D;
class CShaderFade;
class CEditer;
class CPlayer;
class CScene;
class CFade;

// クラスを定義
class CManager
{
public:
	// 規定値
	struct Config {
		struct FilePath {
			static constexpr const char* TexList = "data\\TEXT\\TexFileList.txt";		// テクスチャリスト
			static constexpr const char* ModelList = "data\\TEXT\\ModelList.txt";		// モデルリスト
		};
	};

	// コンストラクタ・デストラクタ
	CManager();
	~CManager();

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	// セッター
	static void ChangePause(void) { m_isPause = !m_isPause; };
	static void OffPause(void) { m_isPause = false; };
	static void OnPause(void) { m_isPause = true; };
	static void SetClear(const bool isClear) { m_isClear = isClear; };
	static void SetScene(CScene* Scene);

	// ゲッター
	static CRenderer* GetRenderer(void) { return m_Renderer; };
	static CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard; };
	static CInputJoypad* GetInputJoypad(void) { return m_pInputJoypad; };
	static CInputMouse* GetInputMouse(void) { return m_pInputMouse; };
	static CCamera* GetCamera(void) { return m_pCamera; };
	static bool GetIsClear(void) { return m_isClear; };
	static CShaderFade* GetShaderFade(void) { return m_pFadeShader; };
	static CLight* GetLight(void) { return m_pLight; };
	static CScene* GetScene(void) { return m_pScene; };
	static CFade* GetFade(void) { return m_pFade; };
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static btDiscreteDynamicsWorld* GetDynamicsWorld(void) { return m_pDynamicsWorld.get(); }

	// プレイヤーをリスポーンさせる
	static void RespawPlayer(void);

	// 条件式の関数か
	static bool isPause(void) { return m_isPause; };
private:
	// 静的メンバ変数
	static CRenderer* m_Renderer;				// レンダラー生成用
	static CInputKeyboard* m_pInputKeyboard;	// キーボードのインスタンス
	static CInputJoypad* m_pInputJoypad;		// ゲームパッドのインスタンス
	static CInputMouse* m_pInputMouse;			// マウスのインスタンス
	static CCamera* m_pCamera;					// カメラのインスタンス
	static CLight* m_pLight;					// ライトのインスタンス
	static CShaderFade* m_pFadeShader;			// シェーダフェード
	static CPlayer* m_pPlayer;
	static CScene* m_pScene;					// シーンのインスタンス
	static CFade* m_pFade;						// 画面遷移用のフェードのインスタンス
	static std::unique_ptr<btDiscreteDynamicsWorld> m_pDynamicsWorld;				// 物理世界
	static bool m_isPause;						// ポーズ中かどうか
	static bool m_isClear;						// 敵を全滅させたかどうか
	// メンバ変数
	std::unique_ptr<btDbvtBroadphase> m_pBroadPhase;					// 衝突判定のクラス
	std::unique_ptr<btDefaultCollisionConfiguration> m_pConfiguration;	// 衝突判定を実行するクラス
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_pSolver;		// 制約ソルバー
	std::unique_ptr<btCollisionDispatcher> m_pDispatcher;				// 衝突判定を検出するクラス
};
#endif // !_MANAGER_H_