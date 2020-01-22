#include "PlayerBase.h"
#include "..\..\..\XInput\XInput.h"
#include "..\..\UI\Fade\FadeUI.h"

bool CPlayerBase::StickInclination( float divideValue )
{
	m_isWalk = false;
	if( CFadeUI::isActive() == true ) return false;
	// �X�e�B�b�N�̗V�т𒴂�������͌��m.
	if( CXInput::GetLThumbX() > INPUT_THUMB_MAX ||// / divideValue ||
		CXInput::GetLThumbX() < INPUT_THUMB_MIN ||// /divideValue  ||
		CXInput::GetLThumbY() > INPUT_THUMB_MAX ||// / divideValue ||
		CXInput::GetLThumbY() < INPUT_THUMB_MIN ){ // / divideValue) {

		m_fMoveVec.x = CXInput::GetLThumbX();			//�R���g���[���̍��E�����̓��͂��擾.
		m_fMoveVec.y = CXInput::GetLThumbY();			//�R���g���[���̏㉺�����̓��͂��擾.

		m_fRadian = atan2f( m_fMoveVec.x, m_fMoveVec.y );	//atan2f��float�^�̃��W�A���ɕϊ�.
		m_isWalk = true;
		return true;
	}
	return false;
}

void CPlayerBase::Move()
{
	//m_fMoveVec.x = CXInput::GetLThumbX();			//�R���g���[���̍��E�����̓��͂��擾.
	//m_fMoveVec.y = CXInput::GetLThumbY();			//�R���g���[���̏㉺�����̓��͂��擾.
	//m_vMoveXYtoVec2 = D3DXVECTOR2( m_fMoveVec.x, m_fMoveVec.y );	//Vec2�ɕϊ�.
	/*float movespd = 0.17f;*/
	//auto SetMoveSpeed = [&](float spd, float divideValue, int count)
	//{
	//	if (m_StickCount < count) {
	//		if (StickInclination(divideValue) == true) {
	//			m_StickCount = count - 1;
	//			movespd = spd;
	//		}
	//	}
	//};
	//SetMoveSpeed(0.05f, 2.0f, 4);
	//SetMoveSpeed(0.01f, 3.0f, 3);
	//SetMoveSpeed(0.005f, 5.0f, 2);
	StickInclination( NULL );

	if( m_isWalk == true ){
		//���x�N�g����p��.
		vecAxisZ = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );//�ϊ��O�x�N�g��.
		m_vRotation.y = m_fRadian;

		D3DXMATRIX mYaw;
		//Y����].
		D3DXMatrixRotationY( &mYaw, m_fRadian );

		//Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
		D3DXVec3TransformCoord( &vecAxisZ,	//(out)�ϊ���x�N�g��.
			&vecAxisZ, &mYaw );

		//-----------------------------------------------------------------------------
		//		�J�����̌����ɑΉ�������.
		//////
		if( GetBitFlag( BitFlag::isCameraMoveUp ) == true ){
			//D3DXMatrixInverse(&m_SayakaMat, nullptr, &m_ViewMat);
			m_Mat.m[1][1] = 0;	//Y�����̓����Œ�.
			m_Mat.m[2][1] = 0;	//Y�����̓����Œ�.
			D3DXVec3TransformNormal( &vecAxisZ, &vecAxisZ, &m_Mat );

			//m_vPosition = m_vPosition + *D3DXVec3TransformNormal( &vecAxisZ, &vecAxisZ, &m_Mat ) / SLOW_DOWN_VALUE;
			m_vPosition.x = m_vPosition.x + vecAxisZ.x * SLOW_DOWN_VALUE;
			m_vPosition.z = m_vPosition.z + vecAxisZ.z * SLOW_DOWN_VALUE;
		} else{
			D3DXMatrixInverse( &m_Mat, nullptr, &m_ViewMat );	//�J�����̌����ɑΉ�.
			m_Mat.m[1][1] = 0;	//Y�����̓����Œ�.
			m_Mat.m[2][1] = 0;	//Y�����̓����Œ�.
			D3DXVec3TransformNormal( &vecAxisZ, &vecAxisZ, &m_Mat );

			/*m_vPosition = m_vPosition + *D3DXVec3TransformNormal( &vecAxisZ, &vecAxisZ, &m_Mat ) / SLOW_DOWN_VALUE;*/
			m_vPosition.x = m_vPosition.x + vecAxisZ.x * SLOW_DOWN_VALUE;
			m_vPosition.z = m_vPosition.z + vecAxisZ.z * SLOW_DOWN_VALUE;
		}
	}
}
