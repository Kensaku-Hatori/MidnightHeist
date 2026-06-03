//================================================================
//
// ポリゴン処理[Object.h]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// 二十インクルード防止
//****************************************************************
#pragma once

//****************************************************************
// インクルード
//****************************************************************
#include "Bace/main.h"

//****************************************************************
// クラス宣言
//****************************************************************
class CObject
{
public:
	// 定数を設定
	struct Config {
		static constexpr int Priority = 8;	// 描画順の最大数
	};

	// コンストラクタ・デストラクタ
	CObject(int Priority = 3);
	virtual ~CObject();

	/// <summary>
	/// 描画順を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>描画順</returns>
	int GetPriority(void) { return m_nPriority; };
	/// <summary>
	/// 死亡フラグを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>フラグ</returns>
	bool GetDeathFlag(void) { return m_bDeath; };
	/// <summary>
	/// 次のオブジェクトへのインスタンス
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	CObject* GetNext(void) { return m_pNext; };

	// 純粋仮想関数
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	/// <summary>
	/// すべて破棄
	/// </summary>
	/// <param name=""></param>
	static void ReleaseAll(void);
	/// <summary>
	/// すべて更新
	/// </summary>
	/// <param name=""></param>
	static void UpdateAll(void);
	/// <summary>
	/// すべて描画
	/// </summary>
	/// <param name=""></param>
	static void DrawAll(void);
	/// <summary>
	/// 描画順を指定して削除
	/// </summary>
	/// <param name="PriorityCount"></param>
	static void Delete(const int PriorityCount);
	/// <summary>
	/// イブジェクトの総数を取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>総数</returns>
	static inline int GetObjectNum(void) { return m_nNumAll; };
	/// <summary>
	/// 描画順ごとの先頭を取得
	/// </summary>
	/// <param name="Priority"></param>
	/// <returns></returns>
	static inline CObject* GetTop(const int Priority) { return m_pTop[Priority]; };
protected:
	/// <summary>
	/// 死亡フラグを立てる
	/// </summary>
	/// <param name=""></param>
	void Release(void);
private:
	// メンバ変数
	static CObject* m_pTop[Config::Priority];			// 一番先頭のポインタ
	static CObject* m_pCur[Config::Priority];			// 一番後ろのポインタ
	CObject* m_pPrev;									// 前のポインタ
	CObject* m_pNext;									// 次のポインタ
	static int m_nNumAll;								// オブジェクトの数をカウント
	int m_nID;											// 自分のオブジェクトのID
	int m_nPriority;									// オブジェクトの優先順位
	bool m_bDeath;										// 死亡フラグ
};