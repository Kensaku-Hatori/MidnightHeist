//****************************************************************
//
// スレッドプールの処理[ThreadPool.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "ThreadPool.h"

//***************************************
// コンストラクタ
//***************************************
CThreadPool::CThreadPool(const int ThreadNum)
{
    // スレッド数を代入
    m_ThreadCount = ThreadNum;
    m_Threads.resize(ThreadNum);
    m_IsRunning = true;

    // スレッドを設定
    for (int ThredCount = 0; ThredCount < ThreadNum; ThredCount++)
    {
        m_Threads[ThredCount] = std::thread(&CThreadPool::Worker, this);
    }
}

//***************************************
// デストラクタ
//***************************************
CThreadPool::~CThreadPool()
{
    {
        // ロック
        std::lock_guard<std::mutex> lock(m_Mutex);
        // フラグを無効にする
        m_IsRunning = false;
    }

    // スレッドをすべて起床させる
    m_Vc.notify_all();

    // スレッドを終了
    for (int ThredCount = 0; ThredCount < m_ThreadCount; ThredCount++)
    {
        m_Threads[ThredCount].join();
    }
}

//***************************************
// 更新
//***************************************
void CThreadPool::Worker(void)
{
    // 更新
    while (true)
    {
        // 自分のタスク
        std::function<void()> Mytask;

        {
            // ロック
            std::unique_lock<std::mutex> Lock(m_Mutex);
            // 待機
            m_Vc.wait(Lock, [&] {return !m_Tasks.empty() || !m_IsRunning; });
            // 終了していたら
            if (!m_IsRunning && m_Tasks.empty()) return;
            // タスクを取り出す
            Mytask = std::move(m_Tasks.front());
            m_Tasks.pop();
        }
        // 処理を実行
        Mytask();
    }
}