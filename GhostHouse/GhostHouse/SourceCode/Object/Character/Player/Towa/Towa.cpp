#include "Towa.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\UI\Fade\FadeUI.h"

CTowa::CTowa()
	: m_pCamera(make_shared<CCamera>())
	, m_TowaState(enTowaState::enBeforehelp)
	, m_vOldPosition()
	, m_vOldPos()
	, m_pSkinMesh(nullptr)
	, m_NowAnimNo(TowaAnim::Wait)
	, m_OldAnimNo(TowaAnim::Wait)
	, m_dAnimeSpeed(ANIM_SPEED.at(m_OldAnimNo))
	, m_isGoal(false)
	, m_bChangeTexture(false)
{
	m_vPosition = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

CTowa::CTowa(stObjectInfo objInfo)
	: CTowa()
{
	SetObjectInfo(objInfo);
	m_fScale = MODEL_SCALE;
}

CTowa::~CTowa()
{
}

void CTowa::Load(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	if (m_pSkinMesh != nullptr) return;
	m_pSkinMesh = CMeshResorce::GetSkin(MAIN_MODEL_NAME);
	if (m_pSkinMesh == nullptr) return;
	m_pSkinMesh->ChangeAnimSet(TowaAnim::Wait);
	m_pCollision->GetSphere()->SetRadius(1.0f);

	TextureLoad(pDevice11);
	m_pSkinMesh->SetChangeTextures(m_pTexture);
}


void CTowa::Update(shared_ptr<CObjectBase> pObj)
{
	if (m_pSkinMesh == nullptr) return;

	if (pObj->GetBitFlag(BitFlag::isGoal) == true) {
		m_bChangeTexture = true;
		LookCameraPos();
		m_isGoal = true;
		return;
	}

	if (pObj->GetObjectNo() == enObjectNo::Sayaka) {
		if (pObj->GetBitFlag(BitFlag::isCameraMoveUp) == true) {
			/*m_pCamera->GimmickUpCamera();*/
		}
		else
		{
			m_pCamera->UpdateCamera();
		}
	}

	ChangeStateButton(pObj);

	TowaStateAction(pObj);

	Respawn();
}

void CTowa::Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
	Light& Light, stCAMERA& stCamera)
{
	if (m_pSkinMesh == nullptr) return;

	AnimetionControll();

	m_ViewMat = mView;

	// �J��������Ɉړ������Ȃ��悤�ɂ��邩������Ȃ��̂�
	// �ꎞ�I�ɃR�����g�A�E�g.
	//if( m_pSayaka->GetBitFlag( BitFlag::isCameraMoveUp ) == false ){
	//	stCamera.Pos = m_pCamera->MoveCameraPos(stCamera.vLook);
	//}
	//else {
	//	m_pCamera->GimmickUpCamera(stCamera.InvisibleCenter);
	//	stCamera.Pos = m_pCamera->GimmickUpCamera(stCamera.InvisibleCenter);
	//}
	//if ( m_pSayaka->GetBitFlag( BitFlag::isCameraMoveUp ) == true )
	//{
	//	m_pStaticMesh->SetRotation(D3DXVECTOR3(0.0f, m_pCamera->GetCameraRadian() + m_fRadian + D3DX_PI, 0.0f));
	//}
	//else
	//{
	//	m_pStaticMesh->SetRotation(D3DXVECTOR3(0.0f, m_pCamera->GetCameraRadian() + m_fRadian, 0.0f));
	//}

	if (m_isGoal == false) m_bChangeTexture = false;

	m_pSkinMesh->SetRotation(m_vRotation);


	if (GetBitFlag(BitFlag::isMovement) == true) {
		stCamera.Pos = m_pCamera->MoveCameraPos(stCamera.vLook);
		stCamera.vLook = m_vPosition;

		//�����[�X�r���h��D3DX_PI������.
#ifdef _DEBUG
		if( CFadeUI::isActive() == false && m_isGamePause == false ){
			if (CXInput::GetLThumbX() > INPUT_THUMB_MAX || CXInput::GetLThumbX() < INPUT_THUMB_MIN ||
				CXInput::GetLThumbY() > INPUT_THUMB_MAX || CXInput::GetLThumbY() < INPUT_THUMB_MIN) {
				if (GetBitFlag(BitFlag::isCameraMoveUp) == true) {
					D3DXVECTOR3 vRot(0.0f, m_pCamera->GetCameraRadian() + m_vRotation.y, 0.0f);
					m_pSkinMesh->SetRotation(vRot);	//Y������].
				}
				else {
					D3DXVECTOR3 vRot(0.0f, m_pCamera->GetCameraRadian() + m_vRotation.y, 0.0f);
					m_pSkinMesh->SetRotation(vRot);	//Y������].
				}
			}
		}
#else
		if( CFadeUI::isActive() == false && m_isGamePause == false ){
			if (CXInput::GetLThumbX() > INPUT_THUMB_MAX || CXInput::GetLThumbX() < INPUT_THUMB_MIN ||
				CXInput::GetLThumbY() > INPUT_THUMB_MAX || CXInput::GetLThumbY() < INPUT_THUMB_MIN) {
				if (GetBitFlag(BitFlag::isCameraMoveUp) == true) {
					D3DXVECTOR3 vRot(0.0f, m_pCamera->GetCameraRadian() + m_vRotation.y, 0.0f);
					m_pSkinMesh->SetRotation(vRot);	//Y������].
				}
				else {
					D3DXVECTOR3 vRot(0.0f, m_pCamera->GetCameraRadian() + m_vRotation.y, 0.0f);
					m_pSkinMesh->SetRotation(vRot);	//Y������].
				}
			}
	}
#endif
	}

	if (m_isGoal == true) {
		m_pCamera->SetCameraPos( stCamera.Pos );
		LookCameraPos();
	}
	m_pSkinMesh->SetPosition(m_vPosition);
	m_pSkinMesh->SetScale(MODEL_SCALE);
	m_pSkinMesh->SetAnimSpeed(m_dAnimeSpeed);
	m_pSkinMesh->Render(mView, mProj, Light, stCamera, m_bChangeTexture);

	//stCamera.Pos = m_pCamera->MoveCameraPos( stCamera.vLook );
//	m_pSkinMesh->SetRotation( m_vRotation );

}

void CTowa::Collision(shared_ptr<CObjectBase> pObj)
{
	if (pObj == nullptr) return;
	if (m_TowaState == enTowaState::enCanMoveAlone) {
		m_pCamera->RayColl(pObj);
	}
	RayHits(pObj);
	if (pObj->GetBitFlag(BitFlag::isKeyDoorOpen) == true) {
		if (m_TowaState == enTowaState::enBeforehelp) {
			ChangeSayakaMoveState(nullptr);
		}
	}

}

void CTowa::RayHits(shared_ptr<CObjectBase> pObj)
{
	if (pObj->GetObjectNo() == enObjectNo::Event) return;
	if (pObj->GetObjectNo() == enObjectNo::Goal) return;
	if (pObj->GetObjectNo() == enObjectNo::TextEvent) return;
	GroundHit(pObj);
	WallJudge(pObj);	// �ǂƂ̓����蔻��.
}

void CTowa::ChangeTowaMoveState(shared_ptr<CObjectBase> pObj)
{
	if (m_TowaState == enTowaState::enFollowSayaka) {
		pObj->BitFlagON(BitFlag::isNowTowaControl);
		m_TowaState = enTowaState::enCanMoveAlone;
		BitFlagON(BitFlag::isMovement);
		pObj->BitFlagOFF(BitFlag::isMovement);
	}
	else if (m_TowaState == enTowaState::enCanMoveAlone) {
		pObj->BitFlagOFF(BitFlag::isNowTowaControl);
		m_TowaState = enTowaState::enStop;
		m_isWalk = false;
		m_vOldPosition = m_vPosition;
		BitFlagOFF(BitFlag::isMovement);
	}
	else if (m_TowaState == enTowaState::enStop) {
		pObj->BitFlagON(BitFlag::isNowTowaControl);
		m_TowaState = enTowaState::enCanMoveAlone;
		BitFlagON(BitFlag::isMovement);
		pObj->BitFlagOFF(BitFlag::isMovement);
	}
	else if (m_TowaState == enTowaState::enBeforehelp) {
		m_TowaState = enTowaState::enFollowSayaka;
		m_isWalk = false;
		m_vOldPosition = m_vPosition;
		BitFlagOFF(BitFlag::isMovement);
	}
}

void CTowa::ChangeSayakaMoveState(shared_ptr<CObjectBase> pObj)
{

	if (m_TowaState == enTowaState::enFollowSayaka) {
		pObj->BitFlagOFF(BitFlag::isNowTowaControl);
		m_pSkinMesh->ChangeAnimSet(TowaAnim::Wait);
		m_NowAnimNo = TowaAnim::Wait;
		m_TowaState = enTowaState::enStop;
	}
	else if (m_TowaState == enTowaState::enStop) {
		pObj->BitFlagOFF(BitFlag::isNowTowaControl);

		m_TowaState = enTowaState::enFollowSayaka;
		m_isWalk = false;
		m_vOldPosition = m_vPosition;
		BitFlagOFF(BitFlag::isMovement);
	}
	else if (m_TowaState == enTowaState::enBeforehelp) {
		m_TowaState = enTowaState::enFollowSayaka;
		m_isWalk = false;
		m_vOldPosition = m_vPosition;
		BitFlagOFF(BitFlag::isMovement);
	}
	else if (m_TowaState == enTowaState::enCanMoveAlone) {
		pObj->BitFlagOFF(BitFlag::isNowTowaControl);
		m_pSkinMesh->ChangeAnimSet(TowaAnim::Wait);
		m_NowAnimNo = TowaAnim::Wait;
		m_TowaState = enTowaState::enStop;
	}

}

void CTowa::TowaStateAction(shared_ptr<CObjectBase> pObj)
{
	if (pObj->GetObjectNo() != enObjectNo::Sayaka) return;
	if (m_pSkinMesh == nullptr) return;

	switch (m_TowaState)
	{
	case enTowaState::enFollowSayaka:
		FollowSayaka(pObj);
		break;
	case enTowaState::enStop:
		m_OldAnimNo = TowaAnim::Wait;
		m_isWalk = false;
		//m_pTowaMesh->SetPosition(m_vOldPosition);
		//m_pTowaMesh->SetRotation(pObj->getPosition() - m_vPosition);
		break;
	case enTowaState::enCanMoveAlone:
		ControlTowa();
	}
}

void CTowa::ControlTowa()
{
	if( CSpriteResource::GetSprite( "Fade" )->GetAlpha() >= 0.1f ){
		m_OldAnimNo = TowaAnim::Wait;
		m_isWalk = false;
		return;
	}
	Move();
}

void CTowa::ChangeStateButton(shared_ptr<CObjectBase> pObj)
{
	if (pObj->GetObjectNo() != enObjectNo::Sayaka) return;

	if (m_TowaState != enTowaState::enBeforehelp) {
		pObj->BitFlagON(BitFlag::isTowaAfterHelping);
	}
	if( GetBitFlag(BitFlag::isTowaAfterHelping) == true ){
		m_TowaState = enTowaState::enFollowSayaka;
		BitFlagOFF( BitFlag::isTowaAfterHelping );
	}

	if (CXInput::IsPress(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		if (m_TowaState == enTowaState::enCanMoveAlone) return;
		if (m_TowaState != enTowaState::enBeforehelp) {
			ChangeSayakaMoveState(pObj);
		}
	}
	if (CXInput::IsPress(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		if (m_TowaState != enTowaState::enBeforehelp) {
			ChangeTowaMoveState(pObj);
		}
	}
	//if (GetBitFlag(BitFlag::isMovement) == false) {
	//	if (CXInput::IsPress(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
	//		ChangeTowaState( pObj );
	//	}
	//}
	//else {
	//	float fDistance = GetTwoDistance(m_vPosition, pObj->GetPosition());
	//	//�R���g���[����LB�L�[�������ꂽ�Ƃ��̏���.
	//	if (CXInput::IsPress(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
	//		if( fDistance < 1.0f ){
	//			ChangeTowaState( pObj );
	//		}
	//	}
	//	
	//}
}

void CTowa::FollowSayaka(shared_ptr<CObjectBase> pObj)
{
	if (pObj->GetObjectNo() != enObjectNo::Sayaka) return;

	float fDistance = GetTwoDistance( m_vPosition, pObj->GetPosition() );
	float fRadian = GetTwoRadian( m_vPosition, pObj->GetPosition() );

	if( fDistance > 1.5f ){
		m_vRotation.y = fRadian;
		m_vPosition += AxisZProc( m_vRotation.y ) * FOLLOW_SPEED;
		m_vRotation.y += D3DX_PI;
	}
	float dis = GetTwoDistance( m_vPosition, m_vOldPosition );
	if( fDistance < 1.48f ){
		m_isWalk = false;
	} else if( fDistance < 1.4f ) {
		m_isWalk = true;
		if( 0.05f > dis ){
			m_isWalk = false;
		} else {
			m_isWalk = true;
		}
	} else {
		m_isWalk = true;
	}


	if( m_onGround == true ) m_vOldPosition = m_vPosition;
}

void CTowa::Move()
{
	CPlayerBase::Move();
}

void CTowa::AnimetionControll()
{
	if (m_pSkinMesh == nullptr) return;
	if (m_isWalk == true) {
		m_OldAnimNo = TowaAnim::Walk;
	}
	else {
		m_OldAnimNo = TowaAnim::Wait;
	}

	if (m_isGoal == true) {
		m_OldAnimNo = TowaAnim::Happy;
	}

	if( m_isGamePause == true ){
		m_dAnimeSpeed = 0.0;
	} else{
		m_dAnimeSpeed = ANIM_SPEED.at( m_OldAnimNo );
	}

	switch (m_OldAnimNo) {
	case TowaAnim::Wait:
		if (m_NowAnimNo != m_OldAnimNo) {
			m_NowAnimNo = TowaAnim::Wait;
			m_dAnimeSpeed = ANIM_SPEED.at(m_OldAnimNo);;
			m_pSkinMesh->ChangeAnimSet(static_cast<int>(m_NowAnimNo));
		}
		break;
	case TowaAnim::Walk:
		if (m_NowAnimNo != m_OldAnimNo) {
			m_NowAnimNo = TowaAnim::Walk;
			m_dAnimeSpeed = ANIM_SPEED.at(m_OldAnimNo);;
			m_pSkinMesh->ChangeAnimSet(static_cast<int>(m_NowAnimNo));
		}
		break;
	case TowaAnim::Happy:
		if (m_NowAnimNo != m_OldAnimNo) {
			m_NowAnimNo = TowaAnim::Happy;
			m_dAnimeSpeed = ANIM_SPEED.at(m_OldAnimNo);;
			m_pSkinMesh->ChangeAnimSet(static_cast<int>(m_NowAnimNo));
		}
		break;
	default:
		break;
	}
}

void CTowa::Respawn()
{
	if (m_onGround == true) {
		m_onGround = false;
		return;
	}
	m_vPosition.y -= 0.1f;
	if (m_vPosition.y <= -4.0f) {
		m_vPosition = m_vOldPos;
		m_vOldPosition = m_vOldPos;
		m_onGround = false;
	}
	m_onGround = false;
}

void CTowa::GroundHit(shared_ptr<CObjectBase> pObj)
{
	// �ΏۃI�u�W�F�N�g�������Ȃ���(�����鏰)�̏ꍇ�͏I��.
	if (pObj->GetObjectNo() == enObjectNo::SlipThroughFloor)return;
	if (pObj->GetObjectNo() == enObjectNo::GhostA)return;
	if (pObj->GetObjectNo() == enObjectNo::GhostB)return;
	if (pObj->GetObjectNo() == enObjectNo::BigGhost)return;
	if (pObj->GetObjectNo() == enObjectNo::Event)return;
	if (pObj->GetObjectNo() == enObjectNo::Goal)return;
	if (pObj->GetObjectNo() == enObjectNo::Sayaka)return;


	float fDistance;
	D3DXVECTOR3 vIntersect;
	D3DXVECTOR3 vPos = m_vPosition;
	// ���������C�̐ݒ�.

	vPos.y += 0.5f;
	m_pCollision->RaySetRay(vPos);
	auto GroundHit = [&]()
	{
		// �Ώۂ̃I�u�W�F�N�g�Ƃ̃��C����.
		if (m_pCollision->isRayCollision(pObj, &fDistance, &vIntersect) == true) {
			// �ΏۃI�u�W�F�N�g�̏�ɐݒ肷��.
			m_vPosition.y = vIntersect.y;
			m_vOldPosition.y = vIntersect.y;
			m_onGround = true;
			m_vOldPos = m_vPosition;
			if (pObj->GetObjectNo() == enObjectNo::UpDownFloor) {
				BitFlagON(BitFlag::isGoal);
			}
		}
	};
	auto GroundHits = [&]()
	{
		// �Ώۂ̃I�u�W�F�N�g�Ƃ̃��C����.
		if (m_pCollision->isRayCollision(pObj, &fDistance, &vIntersect) == true) {
			m_onGround = true;
		}
	};

	// �^���ւ̃��C.
	m_pCollision->RaySetAxis(0.0f, -1.0f, 0.0f);
	GroundHit();

	// �O�㍶�E�ւ̃��C.
	m_pCollision->RaySetAxis(0.1f, -1.0f, 0.1f);
	GroundHits();
	m_pCollision->RaySetAxis(0.1f, -1.0f, -0.1f);
	GroundHits();
	m_pCollision->RaySetAxis(-0.1f, -1.0f, 0.1f);
	GroundHits();
	m_pCollision->RaySetAxis(-0.1f, -1.0f, -0.1f);
	GroundHits();
}

void CTowa::LookCameraPos()
{
	float fRadian = GetTwoRadian(m_vPosition, m_pCamera->GetCameraPos());
	m_vRotation.y = fRadian + D3DX_PI;
}

void CTowa::WallJudge(shared_ptr<CObjectBase> pObj)
{
	FLOAT fDistance[4] = {};			// ����.
	D3DXVECTOR3 vIntersect[4] = {};	// �������W.
	float fDis = 0.0f, fYaw = 0.0f;			// �����Ɖ�](�ꎞ�i�[�p).

	// ���C�̈ʒu��ݒ�.
	D3DXVECTOR3 vPos = m_vPosition;
	vPos.y += 0.5;
	m_pCollision->RaySetRay(vPos);


	// ���C�̌����ɂ�蓖����ǂ܂ł̋��������߂�.
	// ���x�N�g���O.
	m_pCollision->RaySetAxis(0.0f, 0.0f, 1.0f);
	m_pCollision->isRayCollision(pObj, &fDistance[0], &vIntersect[0]);
	// ���x�N�g�����.
	m_pCollision->RaySetAxis(0.0f, 0.0f, -1.0f);
	m_pCollision->isRayCollision(pObj, &fDistance[1], &vIntersect[1]);
	// ���x�N�g���E.
	m_pCollision->RaySetAxis(1.0f, 0.0f, 0.0f);
	m_pCollision->isRayCollision(pObj, &fDistance[2], &vIntersect[2]);
	// ���x�N�g����.
	m_pCollision->RaySetAxis(-1.0f, 0.0f, 0.0f);
	m_pCollision->isRayCollision(pObj, &fDistance[3], &vIntersect[3]);

	fYaw = static_cast<float>(fabs(m_vRotation.y));	// fabs:��Βl(float��).
	dirCheck(&m_vRotation.y);		// 0�`2�΂̊ԂɎ��߂�.

	//--------------------------------------------------.
	// �O���ǂɐڋ�.
	//--------------------------------------------------.
	fDis = fDistance[0];
	if (0.01f < fDis && fDis < 0.8f) {
		// ���v����.
		if (m_vRotation.y < 0.0f) {
			// �E����.
			if (0.785f <= fYaw && fYaw < 2.355f) {
				m_vPosition.x += 0.8f - fDis;
			}
			// �O����.
			else if (2.355f <= fYaw && fYaw < 3.925f) {
				m_vPosition.z += 0.8f - fDis;
			}
			// ������.
			else if (3.925f <= fYaw && fYaw < 5.495f) {
				m_vPosition.x -= 0.8f - fDis;
			}
			// ������.
			else {
				m_vPosition.z -= 0.8f - fDis;
			}
			// �����v����.
		}
		else {
			// �E����.
			if (0.785f <= fYaw && fYaw < 2.355f) {
				m_vPosition.x -= 0.8f - fDis;
			}
			// �O����.
			else if (2.355f <= fYaw && fYaw < 3.925f) {
				m_vPosition.z += 0.8f - fDis;
			}
			// ������.
			else if (3.925f <= fYaw && fYaw < 5.495f) {
				m_vPosition.x += 0.8f - fDis;
			}
			// ������.
			else {
				m_vPosition.z -= 0.8f - fDis;
			}
		}
	}

	//--------------------------------------------------.
	// ��낪�ǂɐڋ�.
	//--------------------------------------------------.
	fDis = fDistance[1];
	if (0.01f < fDis && fDis < 0.8f) {
		// ���v����.
		if (m_vRotation.y < 0.0f) {
			// �E����.
			if (0.785f <= fYaw && fYaw < 2.355f) {
				m_vPosition.x -= 0.8f - fDis;
			}
			// �O����.
			else if (2.355f <= fYaw && fYaw < 3.925f) {
				m_vPosition.z -= 0.8f - fDis;
			}
			// ������.
			else if (3.925f <= fYaw && fYaw < 5.495f) {
				m_vPosition.x += 0.8f - fDis;
			}
			// ������.
			else {
				m_vPosition.z += 0.8f - fDis;
			}
			// �����v����.
		}
		else {
			// �E����.
			if (0.785f <= fYaw && fYaw < 2.355f) {
				m_vPosition.x += 0.8f - fDis;
			}
			// �O����.
			else if (2.355f <= fYaw && fYaw < 3.925f) {
				m_vPosition.z -= 0.8f - fDis;
			}
			// ������.
			else if (3.925f <= fYaw && fYaw < 5.495f) {
				m_vPosition.x -= 0.8f - fDis;
			}
			// ������.
			else {
				m_vPosition.z += 0.8f - fDis;
			}
		}
	}

	//--------------------------------------------------.
	// �E���ǂɐڋ�.
	//--------------------------------------------------.
	fDis = fDistance[2];
	if (0.01f < fDis && fDis < 0.8f) {
		// ���v����.
		if (m_vRotation.y < 0.0f) {
			// �E����.
			if (0.785f <= fYaw && fYaw < 2.355f) {
				m_vPosition.z -= 0.8f - fDis;
			}
			// �O����.
			else if (2.355f <= fYaw && fYaw < 3.925f) {
				m_vPosition.x += 0.8f - fDis;
			}
			// ������.
			else if (3.925f <= fYaw && fYaw < 5.495f) {
				m_vPosition.z += 0.8f - fDis;
			}
			// ������.
			else {
				m_vPosition.x -= 0.8f - fDis;
			}
			// �����v����.
		}
		else {
			// �E����.
			if (0.785f <= fYaw && fYaw < 2.355f) {
				m_vPosition.z += 0.8f - fDis;
			}
			// �O����.
			else if (2.355f <= fYaw && fYaw < 3.925f) {
				m_vPosition.x += 0.8f - fDis;
			}
			// ������.
			else if (3.925f <= fYaw && fYaw < 5.495f) {
				m_vPosition.z -= 0.8f - fDis;
			}
			// ������.
			else {
				m_vPosition.x -= 0.8f - fDis;
			}
		}
	}

	//--------------------------------------------------.
	// �����ǂɐڋ�.
	//--------------------------------------------------.
	fDis = fDistance[3];
	if (0.01f < fDis && fDis < 0.8f) {
		// ���v����.
		if (m_vRotation.y < 0.0f) {
			// �E����.
			if (0.785f <= fYaw && fYaw < 2.355f) {
				m_vPosition.z += 0.8f - fDis;
			}
			// �O����.
			else if (2.355f <= fYaw && fYaw < 3.925f) {
				m_vPosition.x -= 0.8f - fDis;
			}
			// ������.
			else if (3.925f <= fYaw && fYaw < 5.495f) {
				m_vPosition.z -= 0.8f - fDis;
			}
			// ������.
			else {
				m_vPosition.x += 0.8f - fDis;
			}
			// �����v����.
		}
		else {
			// �E����.
			if (0.785f <= fYaw && fYaw < 2.355f) {
				m_vPosition.z -= 0.8f - fDis;
			}
			// �O����.
			else if (2.355f <= fYaw && fYaw < 3.925f) {
				m_vPosition.x -= 0.8f - fDis;
			}
			// ������.
			else if (3.925f <= fYaw && fYaw < 5.495f) {
				m_vPosition.z += 0.8f - fDis;
			}
			// ������.
			else {
				m_vPosition.x += 0.8f - fDis;
			}
		}
	}
}

void CTowa::dirCheck(float* pYaw)
{
	// 1���ȏア�Ă��邩?
	if (*pYaw > static_cast<float>(D3DX_PI * 2.0)) {
		*pYaw -= static_cast<float>(D3DX_PI * 2.0);		// 2��(360��)������.
	}

	// �ċN�֐�.
	if (*pYaw > static_cast<float>(D3DX_PI * 2.0)) {
		dirCheck(pYaw);
	}
}

void CTowa::TextureLoad(ID3D11Device* pDevice11)
{
	m_pTexture = make_shared<CDX9SkinMesh::CHANGE_TEXTURES>();

	strcpy_s(m_pTexture->szTargetTextureName, "sayaka_tex1.png");
	strcpy_s(m_pTexture->szTextureName, "Data\\Model\\Player\\Sayaka\\sayaka__smile_tex1.png");


	if (m_pTexture->szTargetTextureName != 0 &&
		FAILED(
			D3DX11CreateShaderResourceViewFromFileA(
				pDevice11, m_pTexture->szTextureName,
				nullptr, nullptr, &m_pTexture->pTexture, nullptr))) {
		MessageBox(NULL, "�e�N�X�`���ǂݍ��ݎ��s",
			"Error", MB_OK);
		return;
	}
}


LPD3DXMESH CTowa::GetMeshData()
{
	if (m_pSkinMesh == nullptr) return nullptr;
	return m_pSkinMesh->GetMesh();
}