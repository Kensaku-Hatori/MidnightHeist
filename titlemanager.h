//****************************************************************
//
// タイトルマネジャーの処理[titlemanager.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

// インクルード
#include "object.h"
#include "titlemenu.h"
#include "titlelogo.h"

// タイトルマネージャーのクラスを定義
class CTitleManager : public CObject
{
public:
	// ポーズメニューの規定値を設定
	struct Config {
		// メニューの規定値を設定
		struct Menu {
			static const D3DXVECTOR3 Bace;				// ロゴの基準となる位置
			static constexpr float OffSet = 100.0f;		// メニューの隙間
			static constexpr float Width = 100.0f;		// 横幅
			static constexpr float Height = 100.0f;		// 立幅
			static constexpr float DestWidth = 150.0f;	// 横幅
			static constexpr float DestHeight = 150.0f;	// 立幅
			static constexpr int Frame = 30;			// フレーム
		};
		// ロゴの規定値を設定
		struct Logo {
			static const D3DXVECTOR3 Apper;													// 初期位置
			static const D3DXVECTOR3 Dest;													// 目標
			static const D3DXVECTOR2 Size;													// 大きさ
			static constexpr int AnimFrame = 60;											// アニメーションフレーム
			static constexpr const char* FilePath = "data\\TEXTURE\\Title\\title.png";	// テクスチャのパス
		};
	};

	// デストラクタ
	~CTitleManager();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {};

	// セッター
	void SetMenu(CTitleMenu::Menu Menu) { m_SelectMenu = Menu; };

	// ゲッター
	CTitleMenu::Menu GetSelectMenu(void) { return m_SelectMenu; };
	CTitleLogo* GetTitleLogo(void) { return m_TitleLogo; };

	// 静的メンバ関数
	// ゲッター
	static CTitleManager* GetSingleton(void) { return m_Singleton; };
	static bool GetLower(void) { return m_Singleton->GetTitleLogo()->GetLower(); };
	// 生成
	static CTitleManager* CreateSingleton(void);
private:
	// コンストラクタ
	CTitleManager() {};

	// 静的メンバ変数
	static CTitleManager* m_Singleton;		// シングルトン
	CTitleLogo* m_TitleLogo;				// タイトルロゴ
	std::vector<CTitleMenu*> m_apTitleMenu;	// ポーズメニュー
	CTitleMenu::Menu m_SelectMenu;			// 選んでいるメニュー
};
#endif // !_TITLEMANAGER_H_