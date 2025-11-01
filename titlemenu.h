//****************************************************************
//
// タイトルメニューの処理[titlemenu.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _TITLEMENU_H_
#define _TITLEMENU_H_

// インクルード
#include "object2D.h"

// タイトルメニューの基底クラスを定義
class CTitleMenu : public CObject2D
{
public:
	// ポーズの規定値を設定
	struct Config {
		static const D3DXCOLOR SelectColor;		// 選んでいる時の色
		static const D3DXCOLOR DefoultColor;	// 通常の色
	};

	// メニューの列挙型
	typedef enum
	{
		START = 0,	// スタート(始める)
		EXIT,		// イグジット(やめる)
		MAX
	}Menu;

	// コンストラクタ・デストラクタ
	CTitleMenu();
	~CTitleMenu();

	// 仮想関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	// 静的メンバ関数
	// 生成
	static CTitleMenu* Create(D3DXVECTOR3 Pos, Menu Menu);
protected:
	int m_nAnimCounter;		// カウンター
};

// スタートメニュークラスを定義
class CStart : public CTitleMenu
{
public:
	// ポーズメニューの規定値を設定
	struct Config {
		static constexpr const char* FilePath = "data\\TEXTURE\\Title\\start.png";	// パス
	};

	// コンストラクタ・デストラクタ
	CStart();
	~CStart();

	// オーバーライドされた関数
	HRESULT Init(void)	override;
	void Uninit(void)	override;
	void Update(void)	override;
	void Draw(void)		override;
private:
};

// Exitメニュークラスを定義
class CExit : public CTitleMenu
{
public:
	// ポーズメニューの規定値を設定
	struct Config {
		static constexpr const char* FilePath = "data\\TEXTURE\\Title\\quit01.png";	// パス
	};

	// コンストラクタ・デストラクタ
	CExit();
	~CExit();

	// オーバーライドされた関数
	HRESULT Init(void)	override;
	void Uninit(void)	override;
	void Update(void)	override;
	void Draw(void)		override;
private:
};
#endif // !_TITLEMENU_H_