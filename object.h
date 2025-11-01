//****************************************************************
//
// ポリゴン処理[Object.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二十インクルード防止
#ifndef _OBJECT_H_
#define _OBJECT_H_

// インクルード
#include "main.h"
#include "ObjectBaceComponent.hpp"

// クラス宣言
class CObject
{
public:
	// 規定値を設定
	struct Config {
		static constexpr int Priority = 8;	// 描画順の最大数
	};

	// コンストラクタ・デストラクタ
	CObject(int Priority = 3);
	virtual ~CObject();

	// メンバ関数
	// 純粋仮想関数
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	// 静的メンバ関数
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void Delete(const int PriorityCount);
	// ゲッター
	static int GetObjectNum(void) { return m_nNumAll; };
protected:
	// メンバ関数
	void Release(void) { m_Registry.get<ObjectBase>(m_Entity).SetFlag(true); }
	entt::entity m_Entity;								// エンテティ
	static entt::registry m_Registry;					// エンティティの集まり
private:
	// メンバ変数
	static CObject* m_pTop[Config::Priority];			// 一番先頭のポインタ
	static CObject* m_pCur[Config::Priority];			// 一番後ろのポインタ
	static int m_nNumAll;								// オブジェクトの数をカウント
	CObject* m_pPrev;									// 前のポインタ
	CObject* m_pNext;									// 次のポインタ
};
#endif // !_OBJECT_H_