//****************************************************************
//
// タイトルの処理[title.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _TITLE_H_
#define _TITLE_H_

// インクルード
#include "scene.h"

class CPlayerTitle;
class CMapEditer;
class CTitleManager;

// タイトルクラスを定義
class CTitle : public CScene
{
public:
	// 定数を設定
	struct Config {
		//  カメラ
		struct Camera {
			static const D3DXVECTOR3 PosV;	// 始点の位置
			static const D3DXVECTOR3 PosR;	// 注視点の位置
			static const D3DXVECTOR3 Rot;	// 向き
		};
		// プレイヤー
		struct Player {
			static const D3DXVECTOR3 Pos;	// 位置
			static const D3DXVECTOR3 Rot;	// 向き
		};
		// 草
		struct Grass {
			static constexpr float MaxHeight = 40.0f;	// 草の高さ(最大)
			static constexpr float MinHeight = 30.0f;	// 草の高さ(最小)
		};
	};

	// コンストラクタ・デストラクタ
	CTitle();
	~CTitle();

	// メンバ関数
	HRESULT Init(void)	override;
	void Update(void)	override;
	void Uninit(void)	override;
	void Draw(void)		override;
private:
	// 静的メンバ変数
	static CPlayerTitle* m_pPlayer;				// プレイヤーのポインタ
	static CTitleManager* m_pTitleManager;		// ポーズマネージャーへのポインタ
};
#endif // !_TITLE_H_