//****************************************************************
//
// ポーズマネージャーの処理[pausemanager.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _PAUSEMANAGER_H_
#define _PAUSEMANAGER_H_

// インクルード
#include "pause.h"

// ポーズマネジャークラスを定義
class CPauseManager
{
public:
	// ポーズメニューの規定値を設定
	struct Config {
		static constexpr float OffSet = 75.0f;		// メニューの隙間
		static constexpr float Width = 100.0f;		// 横幅
		static constexpr float Height = 25.0f;		// 立幅
		static constexpr float DestWidth = 125.0f;	// 横幅
		static constexpr float DestHeight = 50.0f;	// 立幅
		static constexpr int Frame = 30;			// フレーム
	};

	// デストラクタ
	~CPauseManager();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// セッター
	void SetMenu(CPause::Menu Menu) { m_SelectMenu = Menu; };

	// ゲッター
	CPause::Menu GetSelectMenu(void) { return m_SelectMenu; };

	// 静的メンバ関数
	// ゲッター
	static CPauseManager* GetSingleton(void) { return m_Singleton; };
	// 生成
	static CPauseManager* CreateSingleton(void);
private:
	// コンストラクタ
	CPauseManager() {};

	// 静的メンバ変数
	static CPauseManager* m_Singleton;	// シングルトン
	std::vector<CPause*> m_apPauseMenu;	// ポーズメニュー
	CPause::Menu m_SelectMenu;			// 選んでいるメニュー
};
#endif // !_PAUSEMANAGER_H_