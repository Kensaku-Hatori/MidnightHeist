//================================================================
//
// システムを管理[SystemManager.h]
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
#include "System/Update/baceUpdatesystem.hpp"
#include "System/Rendering/baceRenderingsystem.hpp"

//****************************************************************
// クラスを定義
//****************************************************************
class CSystemManager
{
public:
	~CSystemManager() {};

	/// <summary>
	/// 全システムの更新
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	static void UpdateAll(entt::registry& Reg);
	/// <summary>
	/// 全システムの描画
	/// </summary>
	/// <param name="Reg">レジストリー</param>
	static void RenderingAll(entt::registry& Reg);
	/// <summary>
	/// 更新システムの追加
	/// </summary>
	/// <param name="System">追加するシステム</param>
	static void AddUpdateSystem(BaceSystem* System);
	/// <summary>
	/// 描画システムの追加
	/// </summary>
	/// <param name="System">追加するシステム</param>
	static void AddRenderingSystem(BaceRenderingSystem* System);
	/// <summary>
	/// ポーズ状態を設定
	/// </summary>
	/// <param name="Set"></param>
	inline static void SetPause(bool Set) { m_IsPause = Set; };
	/// <summary>
	/// ポーズ状態かどうか
	/// </summary>
	/// <param name=""></param>
	/// <returns>ポーズ状態</returns>
	inline static bool IsPause(void) { return m_IsPause; }
	/// <summary>
	/// システムの破棄
	/// </summary>
	/// <param name=""></param>
	static void EndSystem(void);
	/// <summary>
	/// 削除リストに追加
	/// </summary>
	/// <param name="Entity">追加したいエンティティ</param>
	static void AddDestroyList(entt::entity Entity);
	/// <summary>
	/// 更新システムの大きさを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>大きさ</returns>
	inline static size_t GetUpdateSystemSize(void) { return m_UpdateSystems.size(); }
	/// <summary>
	/// 描画システムの大きさを取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>大きさ</returns>
	inline static size_t GetRenderingSystemSize(void) { return m_RenderingSystems.size(); }
private:
	CSystemManager() {}
	// メンバ変数
	static std::vector<BaceSystem*> m_UpdateSystems;				// 更新処理の配列
	static std::vector<BaceRenderingSystem*> m_RenderingSystems;	// 描画処理の配列
	static std::vector<entt::entity> m_DestroyList;					// 削除予定のエンティティ
	static bool m_IsPause;											// ポーズ中かどうか
};