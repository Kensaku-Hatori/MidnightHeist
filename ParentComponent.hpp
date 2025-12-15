//****************************************************************
//
// 親子関係のコンポーネント[Parent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// シンプルな親子関係コンポーネントを宣言
struct SingleParentComp {
	// イニシャライズコンストラクタ
	SingleParentComp(const entt::entity ParentEntity = entt::null) : Parent(ParentEntity) {}
	// 描画順
	entt::entity Parent;
};

struct MulParentComp {
	// イニシャライズコンストラクタ
	MulParentComp(const std::vector<entt::entity>& ParentEntity) : Parents(ParentEntity) {}
	// 描画順
	std::vector<entt::entity> Parents;
};