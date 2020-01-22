#include "CharacterBase.h"

// 2�_�̋����𑪂�֐�.
float CCharacterBase::GetTwoDistance( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos )
{
	float distance = sqrtf( ( vtargetPos.x - vMyPos.x ) * ( vtargetPos.x - vMyPos.x )
		+ ( vtargetPos.z - vMyPos.z ) * ( vtargetPos.z - vMyPos.z ) );

	return distance;
}

// 2�_�̊p�x�𑪂�֐�.
float CCharacterBase::GetTwoRadian( const D3DXVECTOR3& vMyPos, const D3DXVECTOR3& vtargetPos )
{
	float radian = atan2f( vtargetPos.x - vMyPos.x, vtargetPos.z - vMyPos.z );

	return radian;
}

D3DXVECTOR3 CCharacterBase::AxisZProc( float y )
{
	// ���x�N�g����p��.
	D3DXVECTOR3 vecAxisZ( 0.0f, 0.0f, 1.0f );	// �ϊ��O�x�N�g��.

	D3DXMATRIX mYaw;	// Y����]�s��.

	// Y����].
	D3DXMatrixRotationY( &mYaw, y );

	// Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord( &vecAxisZ,	// (out)�ԊҌ�x�N�g��.
		&vecAxisZ, &mYaw );

	return vecAxisZ;
}