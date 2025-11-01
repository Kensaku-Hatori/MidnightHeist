//****************************************************************
//
// ポーズの処理[pause.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _PAUSE_H_
#define _PAUSE_H_

// インクルード
#include "object2D.h"

// ポーズクラスを定義
class CPause : public CObject2D
{
public:
	// メニューの列挙型
	typedef enum
	{
		CONTINUE = 0,	// コンティニュー(続ける)
		RETRY,			// リトライ(やり直す)
		QUIT,			// クイット(やめる)
		MAX
	}Menu;

	// ポーズの規定値を設定
	struct Config {
		static const D3DXCOLOR SelectColor;		// 選んでいるときの色
		static const D3DXCOLOR DefoultColor;	// 通常の色
	};

	// コンストラクタ・デストラクタ
	CPause();
	~CPause();

	// 仮想関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	// 静的メンバ関数
	// 生成
	static CPause* Create(D3DXVECTOR3 Pos, Menu Menu);
protected:
	int m_nAnimCounter;		// カウンター
private:
};

// コンテニュークラスを定義
class CContinue : public CPause
{
public:
	// コンテニューメニューの規定値を設定
	struct Config {
		static constexpr const char* FilePath = "data\\TEXTURE\\Pause\\continue.png" ;		// メニューのパス
	};

	// コンストラクタ・デストラクタ
	CContinue();
	~CContinue();

	// オーバーライドされた関数
	HRESULT Init(void)	override;
	void Uninit(void)	override;
	void Update(void)	override;
	void Draw(void)		override;
private:
};

// リトライメニュークラスを定義を定義
class CRetry : public CPause
{
public:
	// コンテニューメニューの規定値を設定
	struct Config {
		static constexpr const char* FilePath = "data\\TEXTURE\\Pause\\retry.png";		// メニューのパス
	};

	// コンストラクタ・デストラクタ
	CRetry();
	~CRetry();

	// オーバーライドされた関数
	HRESULT Init(void)	override;
	void Uninit(void)	override;
	void Update(void)	override;
	void Draw(void)		override;
private:
};

// クイットメニュークラスを定義を定義
class CQuit : public CPause
{
public:
	// コンテニューメニューの規定値を設定
	struct Config {
		static constexpr const char* FilePath = "data\\TEXTURE\\Pause\\quit.png";		// メニューのパス
	};

	// コンストラクタ・デストラクタ
	CQuit();
	~CQuit();

	// オーバーライドされた関数
	HRESULT Init(void)	override;
	void Uninit(void)	override;
	void Update(void)	override;
	void Draw(void)		override;
private:
};
#endif // !_PAUSE_H_