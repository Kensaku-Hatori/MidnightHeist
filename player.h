#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "objectX.h"

class CLaser;

class CPlayer : public CObjectX
{
public:
	CPlayer();
	~CPlayer();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DrawShape(void);
	void CastShadow(void);
	CLaser* GetLaser(void) { return m_Lasertest; }

	static CPlayer* Create(void);
private:
	CLaser* m_Lasertest;
	std::unique_ptr<btCollisionShape> m_CollisionShape;
	std::unique_ptr<btRigidBody> m_RigitBody;		// リジットボディー
};
#endif // !_PLAYER_H_