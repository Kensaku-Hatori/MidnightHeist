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

// 前方宣言
class CPlayerTitle;
class CMapEditer;
class CTitleManager;
class CSound2D;
class CEmitter;

// タイトルクラスを定義
class CTitle : public CScene
{
public:
	// コンストラクタ・デストラクタ
	CTitle();
	~CTitle();

	// メンバ関数
	HRESULT Init(void)	override;
	void Update(void)	override;
	void Uninit(void)	override;
	void Draw(void)		override;
private:
	// 定数を設定
	struct Config {
		// プレイヤー
		struct Player {
			// 位置
			static const D3DXVECTOR3 Pos;
		};
		// タイトルロゴ
		struct TitleLogo {
			// 位置
			static const D3DXVECTOR2 Pos;
			static const D3DXVECTOR2 Size;
			static constexpr const char* Path = "data/TEXTURE/Title/MidNightHeist.png";
		};
		// メッシュフィールド
		struct MeshField {
			static constexpr int nDivH = 100;
			static constexpr int nDivV = 100;
			static const D3DXVECTOR2 Size;
		};
		// 地面の当たり判定
		struct Ground {
			static const btVector3 Size;
			static const btVector3 Origin;
		};
		// タイトルモデル
		struct TitleModel {
			static constexpr const char* Path = "data/MODEL/Museum.x";
		};
	};
	// メンバ変数
	// 地面の当たり判定用変数
	std::unique_ptr<btBoxShape> m_GroundShape;
	std::unique_ptr<btRigidBody> m_GroundRB;
};
#endif // !_TITLE_H_