//================================================================
//
// オブジェクト処理[Object.cpp]
// Author Kensaku Hatori
//
//================================================================

//****************************************************************
// インクルード
//****************************************************************
#include "Bace/object.h"
#include "Bace/manager.h"

//****************************************************************
// 静的メンバ変数宣言
//****************************************************************
int CObject::m_nNumAll = 0;
CObject* CObject::m_pTop[Config::Priority] = {};
CObject* CObject::m_pCur[Config::Priority] = {};

//****************************************************************
// コンストラクタ
//****************************************************************
CObject::CObject(int Priority)
{
	// IDとオブジェクトの種類と描画の優先度を設定
	m_nID = 0;
	m_nPriority = Priority;
	m_bDeath = false;
	m_nNumAll++;

	CObject* pObjectCur = m_pCur[Priority];
	m_pCur[Priority] = this;
	m_pPrev = pObjectCur;
	if (m_pPrev != nullptr)
	{
		m_pPrev->m_pNext = this;
	}
	if (m_pTop[Priority] == nullptr)
	{
		m_pTop[Priority] = this;
	}
}

//****************************************************************
// デストラクタ
//****************************************************************
CObject::~CObject()
{

}

//****************************************************************
// 死亡フラグが立っている奴らを優先度単位で削除
//****************************************************************
void CObject::Delete(const int PriorityCount)
{
	// 今見ているオブジェクトをその優先度の中の一番先頭に設定する
	CObject* pObject = m_pTop[PriorityCount];

	// 最後尾まで続ける
	while (pObject != nullptr)
	{
		// 消すかもしれないから次のやつを保存
		CObject* pObjectNext = pObject->GetNext();
		// 死亡フラグ立っていたら
		if (pObject->m_bDeath == true)
		{
			// デリートする前にIDと優先度をローカル変数に保存
			int Priority = pObject->m_nPriority;

			// 先頭から削除
			if (pObject->m_pNext != nullptr && pObject->m_pPrev == nullptr)
			{
				// 一番先頭を自分の次のやつにする
				m_pTop[Priority] = pObject->m_pNext;
				// 自分の次の前のやつをきれいにする
				pObject->m_pNext->m_pPrev = nullptr;
			}
			// 最後尾から削除
			else if (pObject->m_pNext == nullptr && pObject->m_pPrev != nullptr)
			{
				// 一番後ろを自分の前のやつにする
				m_pCur[Priority] = pObject->m_pPrev;
				// 自分の前の次のやつをきれいにする
				pObject->m_pPrev->m_pNext = nullptr;
			}
			// 真ん中を消す場合
			else if (pObject->m_pNext != nullptr && pObject->m_pPrev != nullptr)
			{
				// 自分から見て前のやつの次を自分から見た次のやつに設定
				pObject->m_pPrev->m_pNext = pObject->m_pNext;
				// 自分から見て次のやつの前を自分から見た前のやつに設定
				pObject->m_pNext->m_pPrev = pObject->m_pPrev;
			}
			else
			{
				// 一番先頭をきれいにする
				m_pTop[Priority] = nullptr;
				// 一番後ろをきれいにする
				m_pCur[Priority] = nullptr;
			}

			// nullptrじゃなかったらリスト情報を消す
			if (pObject != nullptr)
			{
				pObject->m_pNext = nullptr;
				pObject->m_pPrev = nullptr;
			}
			m_nNumAll--;
			// メモリからの解放
			delete pObject;
			pObject = nullptr;
		}
		// 今見ているオブジェクトを次のオブジェクトに設定する
		pObject = pObjectNext;
	}
}

//****************************************************************
// すべての破棄
//****************************************************************
void CObject::ReleaseAll(void)
{
	for (int nCount = 0;nCount < Config::Priority;nCount++)
	{
		CObject* pObject = m_pTop[nCount];
		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->GetNext();
			pObject->Uninit();
			pObject = pObjectNext;
		}
	}
	for (int nCount = 0;nCount < Config::Priority;nCount++)
	{
		Delete(nCount);
	}
}

//****************************************************************
// すべての更新
//****************************************************************
void CObject::UpdateAll(void)
{
	for (int nCount = 0;nCount < Config::Priority;nCount++)
	{
		CObject* pObject = m_pTop[nCount];
		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->GetNext();
			pObject->Update();
			pObject = pObjectNext;
		}
	}
	for (int nCount = 0;nCount < Config::Priority;nCount++)
	{
		Delete(nCount);
	}
}

//****************************************************************
// すべての描画
//****************************************************************
void CObject::DrawAll(void)
{
	CCamera* pCamera = CManager::GetCamera();
	pCamera->SetCamera();

	for (int nCount = 0;nCount < Config::Priority;nCount++)
	{
		CObject* pObject = m_pTop[nCount];
		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->GetNext();
			pObject->Draw();
			pObject = pObjectNext;
		}
	}
}

//****************************************************************
// 死亡フラグを立てる
//****************************************************************
void CObject::Release(void)
{
	m_bDeath = true;
}