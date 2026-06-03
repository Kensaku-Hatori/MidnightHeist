//================================================================
//
// タイトルの処理[title.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二重インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************
#include "scene.h"

//****************************************************************
// 前方宣言
//****************************************************************
class CPlayerTitle;
class CMapEditer;
class CTitleManager;
class CSound2D;
class CEmitter;

//****************************************************************
// タイトルクラスを定義
//****************************************************************
class CTitle : public CScene
{
public:
	// コンストラクタ・デストラクタ
	CTitle();
	~CTitle();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name=""></param>
	/// <returns>結果</returns>
	HRESULT Init(void)	override;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void)	override;
	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name=""></param>
	void Uninit(void)	override;
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
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