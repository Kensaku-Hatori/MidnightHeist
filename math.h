//****************************************************************
//
// 計算の処理[math.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _MATH_H_
#define _MATH_H_

// インクルード
#include "manager.h"
#include "FanInfoComponent.hpp"
#include "XRenderingComponent.hpp"
#include "RayComponent.hpp"
#include "EnemyPatrolPointComp.hpp"

// クラスを定義
class CMath
{
public:
	// 静的メンバ関数
	static float randf(float Max, float Min);
	static D3DXVECTOR3 Get3Dto2DPosition(const D3DXVECTOR3 Position);
	static D3DXVECTOR3 Get2Dto3DPosition(const D3DXVECTOR3 Position);
	static D3DXVECTOR3 CalcMove(D3DXVECTOR3* Pos,float Speed);
	static D3DXVECTOR3 RandVector(D3DXVECTOR3 MaxDir, D3DXVECTOR3 MinDir);
	static float VectersFromAngle(D3DXVECTOR3 VecA, D3DXVECTOR3 VecB, D3DXVECTOR3 Border);
	static float ConvertFloat(float Value, int Format = 2);
	static btQuaternion SetQuad(D3DXQUATERNION Set);
	static D3DXQUATERNION SetQuad(btQuaternion Set);
	static btVector3 SetVec(D3DXVECTOR3 Set);
	static D3DXVECTOR3 SetVec(btVector3 Set);
	static D3DXVECTOR3 CalcModelSize(std::string Path);
	static float Heuristic(const D3DXVECTOR3& Start, const D3DXVECTOR3& Goal);
	static std::vector<int> AStar(std::vector<PatrolPoint::PatrolPointInfo>& Points, const int StartIdx, const int GoalIdx);
	static int NearCanMovePoint(D3DXVECTOR3 Origin, std::vector<PatrolPoint::PatrolPointInfo>& Points, std::vector<entt::entity>& MapObjects);
	static bool IsCanSight(const D3DXVECTOR3& Origin, const D3DXVECTOR3& DestPos, std::vector<entt::entity>& MapObjects);
	static bool IsPointInFan(const FanComp Fan, const D3DXVECTOR3 Point);
	static bool IsMeshOnTheRay(const LPD3DXMESH Mesh, const D3DXMATRIX MeshMtx, const RayComp Ray, float* Distance = nullptr);
	//static float CalcSegSegDist(CCollision::Segment Seg1, CCollision::Segment Seg2, float* S, float* T, D3DXVECTOR3* C1, D3DXVECTOR3* C2);
	//static float CalcCapCapDist(CCollision::Capsule Cap1, CCollision::Capsule Cap2, float* S, float* T, D3DXVECTOR3* C1, D3DXVECTOR3* C2);
private:
	// コンストラクタ・デストラクタ
	CMath();
	~CMath();
};
#endif // !_MATH_H_