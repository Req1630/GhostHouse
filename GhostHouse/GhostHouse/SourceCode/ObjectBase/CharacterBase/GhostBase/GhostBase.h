#ifndef GHOST_BASE_H
#define GHOST_BASE_H

#include "..\CharacterBase.h"

#include <math.h>

class CGhostBase : public CCharacterBase
{
private:
	// ���a.
	const float RADIUS = 0.001f;

protected:
	CGhostBase();
	virtual ~CGhostBase(){};

	// �e����֐�.
	virtual void Action( shared_ptr<CObjectBase> pObj ) = 0;
	// �ҋ@�֐�.
	virtual void IdleState();
	// ���ݓ���֐�.
	virtual float FadeChange();
	// ���G�͈͂̓����蔻�菈���֐�.
	virtual bool SearchCollision( shared_ptr<CObjectBase> pObj );


	// 2�_�̋����𑪂�֐�.
	virtual float GetTwoDistance( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos );
	// 2�_�̊p�x�𑪂�֐�.
	virtual float GetTwoRadian( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos );
		
	// �I�u�W�F�N�g�Ƃ̓����蔻��֐�.
	bool ObjSphereCollision( shared_ptr<CObjectBase> pObj );


	// �ړ��t���O.
	bool	m_bMoveFlag;

	float	m_fWaitDistance;
	bool	m_bAlphaFlag;
	float	m_fCount;

	unique_ptr<CCollisionSphere>	m_pSearchRange;

	D3DXVECTOR3 AxisZProc( float y );
	
private:
	int		m_iAngle;

};

#endif	// #ifndef GHOST_BASE_H.