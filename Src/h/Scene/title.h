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
		// セットアップ
		struct SetUp {
			static constexpr const char* Path = "data/TEXT/SetUp/TitleSetUp.json";
		};
		// 地面の当たり判定
		struct Ground {
			static const btVector3 Size;
			static const btVector3 Origin;
		};
	};
	// メンバ変数
	// 地面の当たり判定用変数
	std::unique_ptr<btBoxShape>		m_GroundShape;
	std::unique_ptr<btRigidBody>	m_GroundRB;
};
#endif // !_TITLE_H_