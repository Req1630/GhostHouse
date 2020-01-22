#ifndef CTOWA_H
#define CTOWA_H

#include "..\..\..\..\ObjectBase/CharacterBase/PlayerBase/PlayerBase.h"

#include "..\..\..\..\XInput\XInput.h"
#include "..\..\..\..\Camera\Camera.h"

#include "..\..\Resource\ModelResource\SkinMesh\CDX9SkinMesh.h"

enum class enTowaState
{
	enFollowSayaka = 0,	// ���₩�ɂ��Ă���.
	enStop = 1,			// ��~.
	enCanMoveAlone = 2,	// �ړ��\.

	enBeforehelp = 3,		// ������O.
};


namespace TowaAnim{
	enum enTowaAnimNo
	{
		None,

		Walk = 0,
		Wait,

		Happy,

		Max = Happy,
	};
};	// namespace Anim_No.

class CTowa : public CPlayerBase
{
	const char*	MAIN_MODEL_NAME = "Towa_s";	// �d�l���f����.
private:
	//�ǔ����x.
	const float FOLLOW_SPEED = 0.165f;
	//�ړ����x������l.
	const float SLOW_DOWN_VALUE = 10.0f;
	const float MODEL_SCALE		= 0.04f;
	//�@<�g�p�������A�j���[�V������, ����>.
	const map<TowaAnim::enTowaAnimNo, double> ANIM_SPEED
	{
		{ TowaAnim::Wait, 0.01 }, { TowaAnim::Walk, 0.06 }, { TowaAnim::Happy, 0.02 }
	};
public:
	CTowa();
	CTowa( stObjectInfo objInfo );
	virtual ~CTowa();

	virtual void Update(shared_ptr<CObjectBase> pObj)	override;
	virtual void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& Light, stCAMERA& stCamera)	override;
	// �����蔻��p�֐�.
	virtual void Collision( shared_ptr<CObjectBase> pObj ) override;
	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )		override;

//	D3DXVECTOR3 getpos() { return m_vPosition; }

	void ChangeTowaMoveState( shared_ptr<CObjectBase> pObj );

	void ChangeSayakaMoveState( shared_ptr<CObjectBase> pObj );

	void ControlTowa();

//	void ChangeTowaState();

	void FollowSayaka( shared_ptr<CObjectBase> pObj );

	void Move();

	void AnimetionControll();

	virtual LPD3DXMESH GetMeshData() override;
private:
	void RayHits( shared_ptr<CObjectBase> pObj ) override;
	void WallJudge( shared_ptr<CObjectBase> pObj );
	void dirCheck( float* pYaw );
	void GroundHit( shared_ptr<CObjectBase> pObj );

	void TowaStateAction( shared_ptr<CObjectBase> pObj );
	void ChangeStateButton( shared_ptr<CObjectBase> pObj );

	void LookCameraPos();
	void Respawn();

	void TextureLoad( ID3D11Device* pDevice11 );

private:
	shared_ptr<CDX9SkinMesh>	m_pSkinMesh;	// �X�L�����b�V��.
	shared_ptr<CCamera>			m_pCamera;
	shared_ptr<CDX9SkinMesh::CHANGE_TEXTURES>	m_pTexture;

	enTowaState m_TowaState;
	D3DXVECTOR3 m_vOldPosition;
	D3DXVECTOR3 m_vOldPos;


	TowaAnim::enTowaAnimNo m_NowAnimNo;
	TowaAnim::enTowaAnimNo m_OldAnimNo;
	double m_dAnimeSpeed;

	bool	m_isGoal;
	bool m_bChangeTexture;
};

#endif // #ifndef CTOWA_H.
