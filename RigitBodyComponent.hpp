//****************************************************************
//
// 剛体のコンポーネント[RigitBodyComponent.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

#include "manager.h"
// 剛体コンポーネント
struct RigitBodyComp {
	// デストラクタ
	~RigitBodyComp() {	
		// 剛体の削除
		if (RigitBody)
		{
			CManager::GetDynamicsWorld()->removeRigidBody(RigitBody.get());
			if (RigitBody->getMotionState())
			{
				delete RigitBody->getMotionState();
			}
			RigitBody.reset();
		}
	};
	// リジットボディー
	std::unique_ptr<btRigidBody> RigitBody;
};