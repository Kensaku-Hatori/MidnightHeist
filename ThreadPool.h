//****************************************************************
//
// スレッドプールの処理[ThreadPool.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// インクルード
#include <queue>
#include <future>

// クラス定義
class CThreadPool
{
public:
	CThreadPool(const int ThreadNum = std::thread::hardware_concurrency());
	~CThreadPool();

	template <typename F, typename... Args, typename R = std::invoke_result_t<std::decay_t<F>, std::decay_t<Args>...>>
	std::future<R> submit(F&& func, const Args&&... args);
private:
	// メンバ関数
	template <typename F>
	void PushTask(const F& Task);
	void Worker(void);
	// メンバ変数
	// ミューテックス
	std::mutex m_Mutex;
	// バリアブルコンディション
	std::condition_variable m_Vc;
	// 積まれているタスク
	std::queue<std::function<void()>> m_Tasks;
	// スレッド
	std::vector<std::thread> m_Threads;
	// スレッド数
	int m_ThreadCount;
	// タスクがすべて終了したかどうか
	std::atomic<bool> m_IsRunning;
};

//***************************************
// タスクを提出
//***************************************
template<typename F, typename ...Args, typename R>
inline std::future<R> CThreadPool::submit(F&& func, const Args && ...args)
{
	auto task = std::make_shared<std::packaged_task<R()>>([func, args...]() {
		return func(args...);
	});
	auto future = task->get_future();

	PushTask([task]() { (*task)(); });
	return future;
}

//***************************************
// タスクを追加
//***************************************
template<typename F>
inline void CThreadPool::PushTask(const F& Task)
{
	{
		// ロック
		const std::lock_guard<std::mutex> lock(m_Mutex);

		// 終了後にタスクを追加されたら
		if (!m_IsRunning) {
			return;
		}

		// 追加
		m_Tasks.push(std::function<void()>(Task));
	}
	// スレッドを起こす
	m_Vc.notify_one();
}