#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "..\Main\Global.h"
#include "..\Resource\SpriteResouce\Sprite\CSpriteUI.h"
#include "..\Resource\SpriteResouce\SpriteResouce.h"

#include "..\Collision\CollisionManager\CollisionManager.h"
#include <unordered_map>
#include <functional>

struct stCAMERA;

// �I�u�W�F�N�g�̎��.
enum class enObjectNo
{
	None,

	Signboard,			// �Ŕ�.
	Key,				// ��.
	// �w�i�I�u�W�F�N�g.
	Ground,		// �n��.
	Ground_I,	
	Ground_L,	
	Flor,
	Flor_I_one,
	Flor_I,
	Flor_L_one,
	Flor_L,
	Wall,		// ��.
	Wall_out,
	Pillar,
	Entrance,
	Entrance_close,
	Roof,
	Roof_L,
	Roof2_I,
	Roof2_L,
	OpenDoor,
	Tree,

	Event,
	Goal,
	TextEvent,
	InvisibleWall,
	InvisibleBox,
	// �v���C���[.
	Sayaka,			// �T���J.
	Towa,			// �g��.

	// �S�[�X�g.
	GhostA,			// �S�[�X�gA.
	GhostB,			// �S�[�X�gB.
	BigGhost,		// �r�b�O�S�[�X�g.

	// �M�~�b�N.
	BookShelf,			// �{�I.
	InvisibleFloor,		// �����Ȃ���.
	SlipThroughFloor,	// �����Ȃ����̂��蔲���o�[�W����.
	InvisibleStartFloor,// �����Ȃ����X�^�[�g�p.
	InvisibleEndFloor,	// �����Ȃ����G���h�p.
	KeyDoor,			// ���ŊJ���h�A.
	Pitfall,			// ���Ƃ���.
	CameraUpCollision,	// �����Ȃ����ڋߎ��A�J��������ɂ�����s���{�b�N�X.
	Switch,				// �X�C�b�`.
	SwitchDoor,			// �X�C�b�`�ŊJ���h�A.
	SwitchRed,				// �X�C�b�`.
	SwitchDoorRed,			// �X�C�b�`�ŊJ���h�A.
	UpDownFloor,		// �㉺��.

	NarrowRoad1,		// �ׂ���.
	NarrowRoad2,		// �ׂ���.
	NarrowRoad3,		// �ׂ���.
	NarrowRoad4,		// �ׂ���.

	Max,

};

const unordered_map<string, enObjectNo> ObjectList
{
	{ "Ground",					enObjectNo::Ground },
	{ "Ground_I",				enObjectNo::Ground_I },
	{ "Ground_L",				enObjectNo::Ground_L },
	{ "Flor",					enObjectNo::Flor },
	{ "Flor_I_one",				enObjectNo::Flor_I_one },
	{ "Flor_I",					enObjectNo::Flor_I },
	{ "Flor_L_one",				enObjectNo::Flor_L_one },
	{ "Flor_L",					enObjectNo::Flor_L },
	{ "Wall",					enObjectNo::Wall },
	{ "Wall_out",				enObjectNo::Wall_out },
	{ "Entrance",				enObjectNo::Entrance },
	{ "Entrance_close",			enObjectNo::Entrance_close },
	{ "Roof",					enObjectNo::Roof },
	{ "Roof_L",					enObjectNo::Roof_L },
	{ "Roof2_I",				enObjectNo::Roof2_I },
	{ "Roof2_L",				enObjectNo::Roof2_L },
	{ "Pillar",					enObjectNo::Pillar },
	{ "Sayaka_s",				enObjectNo::Sayaka },
	{ "Towa_s",					enObjectNo::Towa },
	{ "GhostA",					enObjectNo::GhostA },
	{ "GhostB_s",				enObjectNo::GhostB },
	{ "BigGhost_s",				enObjectNo::BigGhost },
	{ "BookShelf",				enObjectNo::BookShelf },
	{ "Ground",					enObjectNo::Ground },
	{ "InvisibleFloor",			enObjectNo::InvisibleFloor },
	{ "SlipThroughFloor",		enObjectNo::SlipThroughFloor },
	{ "InvisibleStartFloor",	enObjectNo::InvisibleStartFloor },
	{ "InvisibleEndFloor",		enObjectNo::InvisibleEndFloor },
	{ "Signboard",				enObjectNo::Signboard },
	{ "Key",					enObjectNo::Key },
	{ "KeyDoor",				enObjectNo::KeyDoor },
	{ "Pitfall",				enObjectNo::Pitfall },
	{ "LongCollision",			enObjectNo::CameraUpCollision },
	{ "Switch",					enObjectNo::Switch },
	{ "SwitchDoor",				enObjectNo::SwitchDoor },
	{ "Switch_Red",				enObjectNo::SwitchRed },
	{ "SwitchDoor_Red",			enObjectNo::SwitchDoorRed },
	{ "UpDownFloor",			enObjectNo::UpDownFloor },
	{ "Tree",					enObjectNo::Tree },

	{ "NarrowRoad1",			enObjectNo::NarrowRoad1 },
	{ "NarrowRoad2",			enObjectNo::NarrowRoad2 },
	{ "NarrowRoad3",			enObjectNo::NarrowRoad3 },
	{ "NarrowRoad4",			enObjectNo::NarrowRoad4 },

	{ "Event",					enObjectNo::Event },
	{ "Goal",					enObjectNo::Goal },
	{ "TextEvent",				enObjectNo::TextEvent },
	{ "InvisibleWall",			enObjectNo::InvisibleWall },
	{ "InvisibleBox",			enObjectNo::InvisibleBox },
	{ "OpenDoor",				enObjectNo::OpenDoor },
};


// �I�u�W�F�N�g�̏���ǂݍ��ނ��߂̍\����.
struct stObjectInfo
{
	D3DXVECTOR3	vPos;		// ���W���.
	D3DXVECTOR3	vRot;		// ��]���.
	enObjectNo	enObjNo;	// �I�u�W�F�N�g�ԍ�.
	string		ObjName;
	float		fScale;		// ���f���̑傫��.
};

namespace BitFlag
{
	enum enFlagType : unsigned int
	{
		Init			= 0,		// �������p.

		isHit				= 1 << 0,	// ����������.
		isInvincible		= 1 << 1,	// ���G��.
		isHavKey			= 1 << 2,	// ���������Ă��邩.
		isKeyDoorOpen		= 1 << 3,	// ���t���h�A���J���Ă��邩�ǂ���.
		isMoveSwitch		= 1 << 4,	// �X�C�b�`�������Ă��邩.	
		isMoveSwitchRed		= 1 << 5,	// �X�C�b�`�������Ă��邩.	
		isMovement			= 1 << 6,	// �Ώۂ������Ă��邩.
		isHitSignboard		= 1 << 7,	// �ŔɐG��Ă��邩.
		isCameraMoveUp		= 1 << 8,	// �J�����������Ă��邩.
		isDead				= 1 << 9,	// ���S�������ǂ���.
		isNextScene			= 1 << 10,	// ���̃V�[���ɍs���邩.
		isNextText			= 1 << 11,	// �e�L�X�g�𑗂�.
		isGoal				= 1 << 12,	// �S�[���������ǂ���.
		isSignBoardRender	= 1 << 13,	// �Ŕ��\���ł��邩.
		isStopCamera		= 1 << 14,	// �J�������������Ȃ����.
		isNowFrontHouse		= 1 << 15,	// �Ƃ̊O�̃G���A�ɂ�����.
		isNowTowaControl	= 1 << 16,	// �g�����쒆.
		isTowaAfterHelping	= 1 << 17,	// �g������������.
	};
};

class CDX9SkinMesh;
class CDX9Mesh;

class CObjectBase
{
protected:
	CObjectBase()
		: m_vPosition		( 0.0f, 0.0f, 0.0f )
		, m_vRotation		( 0.0f, 0.0f, 0.0f )
		, m_fScale			( 0.0f )
		, m_LifePoint		( 0 )
		, m_fAlpha			( 1.0f )
		, m_isGamePause		( false )
		, m_enObjctNo		( enObjectNo::None )
		, m_ObjName			("")
		, m_CameraState		( 0 )
		, m_cBitFlag		( BitFlag::Init )
	{};
	virtual ~CObjectBase(){};

	void SetObjectInfo( const stObjectInfo& objInfo )
	{
		// ���W���̃Z�b�g.
		m_vPosition = D3DXVECTOR3( objInfo.vPos.x, objInfo.vPos.y, objInfo.vPos.z );

		// ��]���̃Z�b�g.
		m_vRotation = D3DXVECTOR3( objInfo.vRot.x, objInfo.vRot.y, objInfo.vRot.z );

		// �I�u�W�F�N�g�ԍ��̃Z�b�g.
		m_enObjctNo = ObjectList.at(objInfo.ObjName);
		// �I�u�W�F�N�g���̃Z�b�g.
		m_ObjName	= objInfo.ObjName;
	}
public:
	// �X�V�֐�.
	virtual void Update( shared_ptr<CObjectBase> pObj ) = 0;
	// �`��֐�.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, Light& stLight, stCAMERA& stCamera ) = 0;
	// �����蔻��p�֐�.
	virtual void Collision( shared_ptr<CObjectBase> pObj ) = 0;
	// �ǂݍ��݊֐�.
	virtual void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 ) = 0;

	int			m_CameraState;
protected:
	D3DXVECTOR3	m_vPosition;	// ���W.
	D3DXVECTOR3	m_vRotation;	// ��].
	float		m_fScale;		// ���f���̑傫��.
	int			m_LifePoint;	// ���C�t�|�C���g.
	float		m_fAlpha;		// ���f���̓��ߒl.
	bool		m_isGamePause;	// �Q�[�����|�[�Y��Ԃ�.
	enObjectNo	m_enObjctNo;	// �I�u�W�F�N�g�ԍ�.
	string		m_ObjName;		// �I�u�W�F�N�g��.

public:
	// �����蔻��N���X : �X�t�B�A, �{�b�N�X.
	shared_ptr<CCollisionManager>	m_pCollision;

	// �X�^�e�B�b�N���b�V�����擾�֐�.
	virtual LPD3DXMESH	GetMeshData() = 0;

	// ���W���擾�֐�.
	const D3DXVECTOR3 GetPosition() const { return m_vPosition; }
	void SetPosition( const D3DXVECTOR3& pos ){ m_vPosition = pos; }
	// ��]���擾�֐�.
	const D3DXVECTOR3 GetRotation() const { return m_vRotation; }
	void SetRotation( const D3DXVECTOR3& rot ){ m_vRotation = rot; }
	// �g�k���擾�֐�.
	const float GetScale() const { return m_fScale; }

	// �I�u�W�F�N�g�ԍ��擾�֐�.
	enObjectNo	GetObjectNo() const { return m_enObjctNo; }

	// �o�E���f�B���O�{�b�N�X���擾�֐�.
	CCollisionBox* GetBox() const { return m_pCollision->GetBox(); }
	// �o�E���f�B���O�X�t�B�A���擾�֐�.
	CCollisionSphere* GetSphere() const { return m_pCollision->GetSphere(); }

	int GetLifePoint() const { return m_LifePoint; }

	void SetAlpha( const float& alpha ){ m_fAlpha = alpha; }

	void SetGamePause( const bool& isPause ){ m_isGamePause = isPause; }

	// �r�b�g�t���O�̐^�U���擾.
	bool GetBitFlag( const BitFlag::enFlagType& bitType ){ return m_cBitFlag & bitType ? true : false; }
	// �t���O�𗧂Ă�.
	void BitFlagON( const BitFlag::enFlagType& bitType ){ m_cBitFlag |= bitType; }
	// �t���O������.
	void BitFlagOFF( const BitFlag::enFlagType& bitType ){ m_cBitFlag &= ~bitType;}

private:
	int m_cBitFlag;	// �e�t���O���r�b�g�Ő���.
};

#endif // #ifndef OBJECT_BASE_H.