//****************************************************************
//
// コンポーネント達[Components.hpp]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#pragma once

// 剛体コンポーネント
struct RigidBodyComponent{
	RigidBodyComponent(const float _Mass = 0.0f, const int _Group = 0, const int _Mask = 0) :Mass(_Mass), Group(_Group), Mask(_Mask) {};
	// 剛体
	btRigidBody* Body;
	// 当たり判定形状
	btCollisionShape* Shape;
	// 質量
	float Mass;
	// グループ
	int Group;
	// マスク
	int Mask;
	// 着地判定
	bool IsGroundOld, IsGround;
};

// 矩形コライダー
struct BoxColliderComponent {
	// 半分の大きさ
	D3DXVECTOR3 HalfSize;
	// ローカル空間の位置
	D3DXVECTOR3 LocalPos;
	// ローカル空間の回転
	D3DXQUATERNION LocalQuat;
};

// カプセルコライダー
struct CapsuleColliderComponent {
	// 高さ
	float Height;
	// 大きさ
	float Radius;
	// ローカル空間の位置
	D3DXVECTOR3 LocalPos;
	// ローカル空間の回転
	D3DXQUATERNION LocalQuat;
};