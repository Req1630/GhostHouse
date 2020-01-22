#ifndef CSAYAKA_H
#define CSAYAKA_H

#include "..\..\..\..\ObjectBase/CharacterBase/PlayerBase/PlayerBase.h"

#include "..\..\..\..\XInput\XInput.h"

#include "..\..\..\..\Camera\Camera.h"
#include "..\..\Resource\ModelResource\SkinMesh\CDX9SkinMesh.h"

#include "..\..\..\..\Collision\CollisionRay\CollisionRay.h"
#include "..\..\..\..\Collision\CollisionLight\CollisionLight.h"

#include "..\Towa\Towa.h"

#if _DEBUG
#include "..\..\..\..\DebugMode\DebugText\DebugText.h"
#include "..\..\..\..\DebugMode\Sphere\Sphere.h"
#endif	// #if _DEBUG
#include "..\..\..\..\DebugMode\Box\Box.h"

namespace SayakaAnim
{
	enum enSyakaAnimNo
	{
		None = 0,

		Dead = 0,
//		Wait,
		Walk = 2,
		Wait,

		Happy,
		
		Max = Happy,
	};
};	// namespace Anim_No.

class CSayaka : public CPlayerBase
{
	const char*	MAIN_MODEL_NAME = "Sayaka_s";	// �d�l���f����.
	//�T���J�̃}�g���b�N�X�ړ��̌��������邽�߂̊��鐔�l.
	const float SLOW_DOWN_VALUE	= 10.0f;
	const int	INVINCIBLE_TIME = 200;
	const int	MAX_LIFE_POINT	= 3;
	const float MODEL_SCALE		= 0.04f;
	const float SPHERE_COLLISION_RADIUS = 1.0f;		// �����蔻��p�̔��a�̑傫��.

	const float LIFE_IMAGE_RENDER_POS_X = 60.0f;
	const float LIFE_IMAGE_RENDER_POS_Y = 60.0f;

	const float FALL_SPEED = 0.1f;

	//�@<�g�p�������A�j���[�V������, ����>.
	const map<SayakaAnim::enSyakaAnimNo, double> ANIM_SPEED
	{
		{ SayakaAnim::Dead, 0.02 }, { SayakaAnim::Wait, 0.01 }, { SayakaAnim::Walk, 0.06 }, { SayakaAnim::Happy, 0.02 }
	};
public:
	CSayaka();
	CSayaka( const stObjectInfo& objInfo );
	virtual ~CSayaka();

	virtual void Update(shared_ptr<CObjectBase> pObj)	override;
	virtual void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& Light, stCAMERA& stCamera)	override;
	// �����蔻��p�֐�.
	virtual void Collision( shared_ptr<CObjectBase> pObj ) override;
	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 )		override;

	D3DXVECTOR3 GetSayakaPosition_XYZ() { return m_vPosition; }

	void ControllSayaka();

	void SetLightMatrix( Light& light );

	void Move() override;

	virtual LPD3DXMESH GetMeshData() override;

private:
	void RayHits( shared_ptr<CObjectBase> pObj ) override;
	void LightHit( shared_ptr<CObjectBase> pObj );
	void GroundHit( shared_ptr<CObjectBase> pObj );
	void WallJudge( shared_ptr<CObjectBase> pObj );
	void EventBoxHit( shared_ptr<CObjectBase> pObj );
	void TextEventBoxHit( shared_ptr<CObjectBase> pObj );
	void GoalBoxHit( shared_ptr<CObjectBase> pObj );
	void dirCheck( float* pYaw );

	void LookCameraPos();

	void InvincibleTime();
	void SayakaFlasing();
	void Respawn();

	void AnimetionControll();
	void DeadAnimControll();
	void AnimWalkSoundPlay();

	void TextureLoad( ID3D11Device* pDevice11 );

private:
	shared_ptr<CDX9SkinMesh>	m_pSkinMesh;	// �X�L�����b�V��.
	shared_ptr<CCamera>			m_pCamera;
	unique_ptr<CCollisionLight>	m_pCollLight;	// ���C�g�̓����蔻��.
	shared_ptr<CDX9SkinMesh::CHANGE_TEXTURES>	m_pTexture;

	unique_ptr<CBox>		m_pBox;

	enObjectNo m_NowGroundType;

	D3DXVECTOR3 m_LightAxisZ;

	SayakaAnim::enSyakaAnimNo		m_enNowAnimNo;
	SayakaAnim::enSyakaAnimNo		m_enOldAnimNo;
	double	m_dAnimeSpeed;
	double	m_dDeadAnimTime;
	double	m_dCountAnimTime;
	bool	m_bLeftSound;
	bool	m_bRightSound;

	int			m_bInvincibleCount;			//���G����.
	int			m_FlashCount;
	bool		m_bDispFlag;

	bool		m_isUpdate;

	bool		m_bChangeTexture;
};

#endif // #ifndef CSAYAKA_H.
