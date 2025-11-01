//****************************************************************
//
// タイトルロゴの処理[titlelogo.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

// インクルード
#include "object2D.h"

// タイトルロゴクラスを定義
class CTitleLogo : public CObject2D
{
public:
	// コンストラクタ・デストラクタ
	CTitleLogo();
	~CTitleLogo();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Lower(void);

	// ゲッター
	bool GetLower(void);

	// 静的メンバ関数
	// 生成
	static CTitleLogo* Create(const D3DXVECTOR3 Pos, const D3DXVECTOR3 Rot, const D3DXVECTOR2 Size, const char* FilePath);
private:
	int m_nFrameCounter;	// フレームをカウント
};
#endif // !_TITLELOGO_H_