//================================================================
//
//　Manager[manager.h]
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
#include "main.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "line.h"
#include "ThreadPool.h"

//****************************************************************
// 前方宣言
//****************************************************************
class CObject3D;
class CShaderFade;
class CEditer;
class CPlayer;
class CScene;
class CFade;

//****************************************************************
// クラスを定義
//****************************************************************
class CManager
{
public:
	// 定数
	struct Config {
		struct FilePath {
			static constexpr const char* TexList = "data\\TEXT\\TexFileList.txt";		// テクスチャリスト
			static constexpr const char* ModelList = "data\\TEXT\\ModelList.txt";		// モデルリスト
		};
	};

	// コンストラクタ・デストラクタ
	CManager();
	~CManager();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="hInstance">インスタンスハンドル</param>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="bWnd">ウィンドウかどうか</param>
	/// <returns>結果</returns>
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWnd);
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
	/// 描画
	/// </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary>
	/// (ECS)Systemを初期化
	/// </summary>
	/// <param name=""></param>
	static void InitSystems(void);
	/// <summary>
	/// クリアフラグを設定
	/// </summary>
	/// <param name="isClear">クリアしたかどうか</param>
	static void SetClear(const bool isClear) { m_isClear = isClear; };
	/// <summary>
	/// シーンを設定
	/// </summary>
	/// <param name="Scene">設定したいシーン</param>
	static void SetScene(CScene* Scene);
	/// <summary>
	/// ポーズ状態を変更
	/// </summary>
	/// <param name=""></param>
	static inline void ChangePause(void) { m_isPause = !m_isPause; }
	/// <summary>
	/// ポーズ状態を解除
	/// </summary>
	/// <param name=""></param>
	static inline void OffPause(void) { m_isPause = false; }
	/// <summary>
	/// ポーズ状態に変更
	/// </summary>
	/// <param name=""></param>
	static inline void OnPause(void) { m_isPause = true; }
	/// <summary>
	/// レンダラーのインスタンスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static inline CRenderer* GetRenderer(void) { return m_Renderer; }
	/// <summary>
	/// キーボードのインスタンスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static inline CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard; }
	/// <summary>
	/// ゲームパッドのインスタンスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static inline CInputJoypad* GetInputJoypad(void) { return m_pInputJoypad; };
	/// <summary>
	/// マウスのインスタンスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static inline CInputMouse* GetInputMouse(void) { return m_pInputMouse; }
	/// <summary>
	/// カメラのインスタンスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static inline CCamera* GetCamera(void) { return m_pCamera; }
	/// <summary>
	/// クリアかどうかを確認
	/// </summary>
	/// <param name=""></param>
	/// <returns>true = クリア</returns>
	static inline bool GetIsClear(void) { return m_isClear; }
	/// <summary>
	/// ライトのインスタンスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static inline CLight* GetLight(void) { return m_pLight; }
	/// <summary>
	/// シーンのインスタンスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static inline CScene* GetScene(void) { return m_pScene; }
	/// <summary>
	/// 物理ワールドのインスタンスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static inline btDiscreteDynamicsWorld* GetDynamicsWorld(void) { return m_pDynamicsWorld.get(); }
	/// <summary>
	/// スレッドプールのインスタンスを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>インスタンス</returns>
	static inline CThreadPool* GetThreadPool(void) { return m_pThreadPool.get(); }
	/// <summary>
	/// ポーズ中かどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>ポーズ中かどうか</returns>
	static inline bool isPause(void) { return m_isPause; }
private:
	// 静的メンバ変数
	static CRenderer* m_Renderer;										// レンダラー生成用
	static CInputKeyboard* m_pInputKeyboard;							// キーボードのインスタンス
	static CInputJoypad* m_pInputJoypad;								// ゲームパッドのインスタンス
	static CInputMouse* m_pInputMouse;									// マウスのインスタンス
	static CCamera* m_pCamera;											// カメラのインスタンス
	static CLight* m_pLight;											// ライトのインスタンス
	static CScene* m_pScene;											// シーンのインスタンス
	static std::unique_ptr<btDiscreteDynamicsWorld> m_pDynamicsWorld;	// 物理世界
	static bool m_isPause;												// ポーズ中かどうか
	static bool m_isClear;												// 敵を全滅させたかどうか
	// メンバ変数
	static std::unique_ptr<CThreadPool> m_pThreadPool;					// スレッドプール
	std::unique_ptr<btDbvtBroadphase> m_pBroadPhase;					// 衝突判定のクラス(バウンディングボックス)
	std::unique_ptr<btDefaultCollisionConfiguration> m_pConfiguration;	// 衝突判定の設定
	std::unique_ptr<btSequentialImpulseConstraintSolver> m_pSolver;		// 衝突判定を検知
	std::unique_ptr<btCollisionDispatcher> m_pDispatcher;				// 衝突判定後の押し出し等を設定
};