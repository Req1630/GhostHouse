#ifndef COLLISION_BASE_H
#define COLLISION_BASE_H

#include "..\..\Main\Global.h"
#include "..\..\DebugMode\Box\Box.h"

class CDX9Mesh;

class CCollisionBox
{
public:
	CCollisionBox( D3DXVECTOR3* pvPos );
	~CCollisionBox();
	
	// ���菈��.
	bool isCollision( CCollisionBox* collBox );
	
	// �{�b�N�X������.
	HRESULT InitBBox( LPD3DXMESH pMesh );
	// �{�b�N�X�̃T�C�Y��ύX����.
	void BoxScaleSet( const float& size )
	{
		m_vPositionMax = m_vPositionMax * size;
		m_vPositionMin = m_vPositionMin * size;
	}

	// �e���擾�֐�.
	D3DXVECTOR3	GetPosition()	 const { return *m_pvPosition; }
	D3DXVECTOR3	GetPositionMax() const { return m_vPositionMax; }
	D3DXVECTOR3	GetPositionMin() const { return m_vPositionMin; }
	D3DXVECTOR3 GetLength()		 const { return m_vLength; }

private:
	CCollisionBox();

private:
	D3DXVECTOR3*	m_pvPosition;		// �ʒu���.		
	D3DXVECTOR3		m_vPositionMax;		// �ő咸�_.
	D3DXVECTOR3		m_vPositionMin;		// �ŏ����_.

	//	�� �f�o�b�O�p.
	D3DXVECTOR3		m_vLength;			// ����.
	float*			m_pfYaw;			// Y����]�l.

};

#endif	// #ifndef COLLISION_BASE_H.
