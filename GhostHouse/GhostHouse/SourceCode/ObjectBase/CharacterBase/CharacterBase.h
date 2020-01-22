#ifndef CHARACTER_BASE_H
#define CHARACTER_BASE_H

#include "..\ObjectBase.h"


class CCharacterBase : public CObjectBase
{
protected:
	CCharacterBase()
	{
		m_pCollision = make_shared<CCollisionManager>( &m_vPosition, &m_vRotation.y );
	};
	virtual ~CCharacterBase(){};

	// 2�_�̋����𑪂�֐�.
	virtual float GetTwoDistance( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos );
	// 2�_�̊p�x�𑪂�֐�.
	virtual float GetTwoRadian( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos );


	D3DXVECTOR3 AxisZProc( float y );
	
};

#endif // #ifndef CHARACTER_BASE_H.