#include "Stage.h"
#include "..\..\XAudio2\PlaySEThread.h"
#include "..\..\UI\Fade\FadeUI.h"
#include "..\..\XInput\XInput.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Resource\TutorialText\TutorialTextLoad.h"

CStage::CStage()
	: m_isLightSet(true)
	, m_bOneSound(true)
	, m_bOneCall(true)
	, m_isPause(false)
	, m_isRender(false)
	, m_isStopCamera(true)
	, init_Map(-1)
	, m_MapSize(-1)
	, m_TextNum(0)
	, m_TextStartNum(0)
	, m_bStartGame(false)
	, m_vTextColPostion()
	, m_NowMapScene(MapScene::In_the_forest)
	, m_pBackGround(nullptr)
	, m_pPlayerManager(make_unique<CPlayerManager>())
	, m_pObjectManager(make_unique<CObjectManager>())
	, m_pText(make_unique<CTutorialText>())
	, m_pMap()
	, m_pMapList()
	, m_pPaper(nullptr)
{}

CStage::~CStage()
{
}

void CStage::Load(ID3D11Device * pDevice11, ID3D11DeviceContext * pContext11)
{
	isRender();

	// �w�i�̓ǂݍ���.
	if (m_pBackGround == nullptr) {
		m_pBackGround = CMeshResorce::GetStatic("haikei");
	}

	if (LoadMap() == true) return;
	m_pText->Load();

	ChangeNextMap();

	m_pObjectManager->Init(pDevice11, pContext11, m_pMap, m_pPaper);
	LoadMapObject(pDevice11, pContext11);
	PlayerInit(pDevice11, pContext11);
	GetNextTextColPosition();
}

void CStage::Updata()
{
	// �X�V�֐����������C�g�̈ʒu�����₩�ɃZ�b�g������������.
	// LightSet�𖳌��ɂ���.
	m_bStartGame = true;

	if (m_pPlayerManager->GetSayaka() == nullptr) return;
	if (m_bStartGame == true) {
		if (m_bOneCall == true) {
			for (size_t i = 0; i <= m_MapSize; i++) {
				m_OldTextNum.emplace_back();
				InitTextNum(i);
			}
			m_bOneCall = false;
			m_TextNum = m_OldTextNum[init_Map];
		}
	}
	ChangeText();
	if (CFadeUI::isActive() == false) {
		m_pPlayerManager->GetSayaka()->BitFlagOFF(BitFlag::isStopCamera);
		// LightSet�𖳌��ɂ���.
		m_isLightSet = false;
		CXInput::SetActiveFlag(true);
	}
	m_pPlayerManager->Update();	// �v���C���[�̍X�V.
}

void CStage::Render(D3DXMATRIX & mView, D3DXMATRIX & mProj, Light& stLight, stCAMERA& stCamera)
{
	if (m_pPlayerManager->GetSayaka() == nullptr) return;

	if (m_isStopCamera == true) {
		m_pPlayerManager->GetSayaka()->BitFlagON(BitFlag::isStopCamera);
		m_isStopCamera = false;
	}

	// ���C�g�̏�����.
	InitLightPosition(stLight);

	// �w�i�̕`��.
	BackGroundRender(mView, mProj, stLight, stCamera);

	// �I�u�W�F�N�g���X�N���[���O�����肷��֐�.
	auto isWindowRenderOut = [&](const D3DXVECTOR2& vec2) -> bool
	{
		if (-WINDOW_OUT_SIZE > vec2.x || vec2.x > WND_W + WINDOW_OUT_SIZE ||
			-WINDOW_OUT_SIZE > vec2.y || vec2.y > WND_H + WINDOW_OUT_SIZE) return true;
		return false;
	};

	// �e�I�u�W�F�N�g�̕`��.
	for (auto& m : m_pMap) {
		m_pPlayerManager->Collsiton(m);

		// �e�I�u�W�F�N�g��3D���W���X�N���[�����W�ɕϊ�.
		D3DXVECTOR2 vec = CDX9Mesh::WorldtoScreenPos(mView, mProj, m->GetPosition());
		if (isWindowRenderOut(vec) == true) continue;	// �X�N���[�����O�Ȃ�`�悵�Ȃ�.

		if (m_isPause == false) m->Update(m_pPlayerManager->GetSayaka());

		m->SetGamePause(m_isPause);

		m->Collision(m_pPlayerManager->GetSayaka());
		m->Collision(m_pPlayerManager->GetTowa());

		m->Render(mView, mProj, stLight, stCamera);		// �I�u�W�F�N�g�̕\��.
	}
	m_pPlayerManager->SetGamePause(m_isPause);
	m_pPlayerManager->Render(mView, mProj, stLight, stCamera);	// �v���C���[�\��.

	if (m_isPause == false && isSayakaDead() == false) {
		m_pPlayerManager->UIRender();											// �v���C���[��UI�̕\��.
		m_pObjectManager->UIRender(m_pPlayerManager->GetSayaka());			// �I�u�W�F�N�g��UI�̕\��.

		if (m_bStartGame == true) DispText(m_pPlayerManager->GetSayaka());	// �e�L�X�g�̕\��.
	}

	if (m_pPlayerManager->GetSayaka()->GetBitFlag(BitFlag::isCameraMoveUp) == true) {
		stCamera.vLook = m_pObjectManager->GetInvisibleFloorCenterPosition();
	}
	stCamera.InvisibleCenter = m_pObjectManager->GetInvisibleFloorCenterPosition();
	stCamera.InvisibleCollisionPos = m_pObjectManager->GetNearPosition(m_pPlayerManager->GetSayaka());
}

void CStage::ContinueStageReset()
{
	int nextMap = static_cast<int>(m_NowMapScene);
	CFadeUI::Init();
	CFadeUI::FadeINStart();
	m_pMap.clear();
	m_pMapList.clear();
	m_pPlayerManager->ResetPlayer();
	m_isRender = false;
}

bool CStage::isRender()
{
	if (m_isRender == true) return true;

	for (auto& m : m_pMap) if (m->GetMeshData() == nullptr) return false;

	if (m_pMap.size() != 0) m_isRender = true;

	return true;
}

bool CStage::isGaol()
{
	// ���₩�����Ȃ���ΏI��.
	if (m_pPlayerManager->GetSayaka() == nullptr) return false;
	return m_pPlayerManager->GetSayaka()->GetBitFlag(BitFlag::isGoal);
}

bool CStage::isSayakaDead()
{
	if (m_pPlayerManager->GetSayaka() == nullptr) return false;
	return m_pPlayerManager->GetSayaka()->GetBitFlag(BitFlag::isDead);
}

bool CStage::isSayakaLifePointZero()
{
	if (m_pPlayerManager->GetSayaka() == nullptr) return false;
	return m_pPlayerManager->GetSayaka()->GetLifePoint() == 0 ? true : false;
}

bool CStage::LoadMap()
{
	if (m_pMap.size() == 0) {
		init_Map = CMapLoad::GetStageNum();
		// �O�V�[���őI�������X�e�[�W�}�b�v�̍쐬.
		m_pMapList.emplace_back();
		CMapLoad::FileRead(m_pMapList[0], "01_Forest");
		m_pMapList.emplace_back();
		CMapLoad::FileRead(m_pMapList[1], "02_House");
		m_pMapList.emplace_back();
		CMapLoad::FileRead(m_pMapList[2], "03_1Floor");
		m_pMapList.emplace_back();
		CMapLoad::FileRead(m_pMapList[3], "04_2Floor");
		m_pMapList.emplace_back();
		CMapLoad::FileRead(m_pMapList[4], "05_B1FloorArea1");
		m_pMapList.emplace_back();
		CMapLoad::FileRead(m_pMapList[5], "06_B1FloorArea2");
		m_pMapList.emplace_back();
		CMapLoad::FileRead(m_pMapList[6], "07_1FoorEnd");
		if (init_Map >= static_cast<int>(m_pMapList.size())) {
			_ASSERT_EXPR(false, L"�}�b�v�̐�������܂���");
		}
		m_MapSize = static_cast<int>(m_pMapList.size());
		m_pMap = m_pMapList[init_Map];
		m_NowMapScene = static_cast<MapScene>(init_Map);
		m_isStopCamera = true;
		m_isLightSet = true;
		m_pPlayerManager->SetStageNumber( init_Map );
		if (m_bStartGame == true) {
			m_TextNum = m_OldTextNum[init_Map];
		}
		return true;
	}
	return false;
}

void CStage::LoadMapObject(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	if (m_isRender == true) return;

	// �e�I�u�W�F�N�g�̓ǂݍ���.
	for (size_t i = 0; i < m_pMap.size(); i++) {
		if (m_pMap[i]->GetMeshData() != nullptr) continue;
		m_pMap[i]->Load(pDevice11, pContext11);
	}
}

void CStage::PlayerInit(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11)
{
	if (m_isRender == true) return;

	auto ObjSwap = [&](shared_ptr<CObjectBase>& m, enObjectNo no, int index)
	{
		shared_ptr<CObjectBase> obj = m;

		if (obj->GetObjectNo() == enObjectNo::Sayaka) {
			m_pPlayerManager->SetSayaka(m);
			m_pMap.erase(m_pMap.begin() + index);
			return;
		}
		if (obj->GetObjectNo() == enObjectNo::Towa) {
			m_pPlayerManager->SetTowa(m);
			m_pMap.erase(m_pMap.begin() + index);
			return;
		}
		return;
	};

	for (size_t i = 0; i < m_pMap.size(); i++) {
		ObjSwap(m_pMap[i], enObjectNo::Sayaka, i);
		if( i >= m_pMap.size() ) continue;
		ObjSwap(m_pMap[i], enObjectNo::Towa, i);
	}

	m_pPlayerManager->Load(pDevice11, pContext11);
	m_pPlayerManager->OneGroundHit(m_pMap);
}

void CStage::ChangeNextMap()
{
	if (m_pPlayerManager->GetSayaka() == nullptr) return;
	if (m_pPlayerManager->GetSayaka()->GetBitFlag(BitFlag::isNextScene) == false) return;

	CFadeUI::Init();
	CFadeUI::FadeINStart();

	int nextMap = 0;
	switch (m_NowMapScene) {
	case MapScene::In_the_forest:
		m_NowMapScene = MapScene::In_front_house;
		//			m_NowMapScene = MapScene::In_the_house_F1_end;
		nextMap = static_cast<int>(m_NowMapScene);
		break;
	case MapScene::In_front_house:
		m_NowMapScene = MapScene::In_the_house_F1;
		nextMap = static_cast<int>(m_NowMapScene);
		break;
	case MapScene::In_the_house_F1:
		m_NowMapScene = MapScene::In_the_house_F2;
		nextMap = static_cast<int>(m_NowMapScene);
		break;
	case MapScene::In_the_house_F2:
		m_NowMapScene = MapScene::In_the_house_B1_Area1;
		nextMap = static_cast<int>(m_NowMapScene);
		break;
	case MapScene::In_the_house_B1_Area1:
		m_NowMapScene = MapScene::In_the_house_B1_Area2;
		nextMap = static_cast<int>(m_NowMapScene);
		break;
	case MapScene::In_the_house_B1_Area2:
		m_NowMapScene = MapScene::In_the_house_F1_end;
		nextMap = static_cast<int>(m_NowMapScene);
		break;
	default:
		break;
	}
	m_TextNum = m_OldTextNum[nextMap];

	if (static_cast<size_t>(nextMap) >= m_pMapList.size()) {
		m_pPlayerManager->GetSayaka()->BitFlagOFF(BitFlag::isNextScene);
		return;
	}


	m_isRender = false;
	// �g�p�}�b�v���擾.
	m_pMap = m_pMapList[nextMap];
	CMapLoad::SetStageNum(nextMap);
	// �V�[���؂�ւ����p�̃v���C���[���W����ւ�.
	m_pPlayerManager->ChangeSceneInit(m_pMap);
	m_isStopCamera = true;
	m_isLightSet = true;
	// �I�u�W�F�N�g�̏������p�̃t���O�����낷.
	m_pObjectManager->SetInitFlagUnload();
}

void CStage::InitLightPosition(Light& stLight)
{
	if (m_isLightSet == false) return;

	stLight.pos = D3DXVECTOR3(0.0, 0.0, 0.0f);
	stLight.pos.y = -10.0f;
	stLight.cone = 10.0f;
	stLight.range = 10.0f;
	stLight.att = D3DXVECTOR3(0.4f, 0.02f, 0.0f);
	stLight.ambient = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 0.7f);
	stLight.diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXMATRIX mRot, mYaw, mPitch, mRoll;
	D3DXMatrixRotationX(&mPitch, D3DXToRadian(90));	// �P����X����].
	D3DXMatrixRotationZ(&mRoll, 0.0f);	// �P����Z����].
	D3DXMatrixRotationY(&mYaw, 0.0f);		// �P����Y����].
	mRot = mYaw * mPitch * mRoll;
	D3DXMatrixTranspose(&mRot, &mRot);
	stLight.Rot = mRot;
}

void CStage::BackGroundRender(D3DXMATRIX& mView, D3DXMATRIX& mProj, Light& stLight, stCAMERA& stCamera)
{
	if (m_pBackGround == nullptr) return;

	D3DXVECTOR3 vPos = m_pPlayerManager->GetSayaka()->GetPosition();
	vPos.y = 1.0f;
	m_pBackGround->SetPosition(vPos);
	m_pBackGround->SetScale(3.0f);
	m_pBackGround->Render(mView, mProj, stLight, stCamera);
}

void CStage::GetNextTextColPosition()
{
	m_vTextColPostion.clear();
	m_vTextColNum.clear();
	for (size_t i = 0; i < m_pMap.size(); i++) {
		if (m_pMap[i]->GetObjectNo() == enObjectNo::TextEvent) {
			m_vTextColPostion.emplace_back(
				D3DXVECTOR3(m_pMap[i]->GetPosition().x, m_pMap[i]->GetPosition().y, m_pMap[i]->GetPosition().z));
			m_vTextColNum.emplace_back(i);
		}
	}
}

void CStage::DispText(shared_ptr<CObjectBase> pObj)
{
	if (m_pText == nullptr) return;
	m_pText->Render(m_TextNum, pObj);
}

void CStage::ChangeText()
{
	if (m_pPlayerManager->GetSayaka() == nullptr) return;
	if (m_pPlayerManager->GetSayaka()->GetBitFlag(BitFlag::isNextText) == false) return;

	if (CTutorialTextLoad::SearchFiristAsterisk(m_TextNum) == true) {
		m_pPlayerManager->GetSayaka()->BitFlagOFF(BitFlag::isNextText);
		m_TextNum += 1;
		return;
	}
	//�e�L�X�g�𑡂铖���蔻�肪1�����Ȃ画�ʂ͕s�v.
	if (m_vTextColPostion.size() == 1) {
		m_pMap[m_vTextColNum[0]]->SetPosition(
			D3DXVECTOR3(m_pMap[m_vTextColNum[0]]->GetPosition().x, -100.0f, m_pMap[m_vTextColNum[0]]->GetPosition().z));
		m_pPlayerManager->GetSayaka()->BitFlagOFF(BitFlag::isNextText);
		m_TextNum += 1;
		return;
	}
	//2�Ȃ�\�[�g���K�v�Ȃ�.+
	else if (m_vTextColPostion.size() == 2) {
		float distance1 = GetTwoDistanceXYZ(m_pPlayerManager->GetSayaka()->GetPosition(), m_pMap[m_vTextColNum[0]]->GetPosition());
		float distance2 = GetTwoDistanceXYZ(m_pPlayerManager->GetSayaka()->GetPosition(), m_pMap[m_vTextColNum[1]]->GetPosition());
		if (distance1 > distance2) {
			m_pMap[m_vTextColNum[1]]->SetPosition(
				D3DXVECTOR3(m_pMap[m_vTextColNum[1]]->GetPosition().x, -100.0f, m_pMap[m_vTextColNum[1]]->GetPosition().z));
			m_pPlayerManager->GetSayaka()->BitFlagOFF(BitFlag::isNextText);
			m_TextNum += 1;
			return;
		}
		m_pMap[m_vTextColNum[0]]->SetPosition(
			D3DXVECTOR3(m_pMap[m_vTextColNum[0]]->GetPosition().x, -100.0f, m_pMap[m_vTextColNum[0]]->GetPosition().z));
		m_pPlayerManager->GetSayaka()->BitFlagOFF(BitFlag::isNextText);
		m_TextNum += 1;
		return;
	}

	vector<float>	Distance;
	bool			ChangeFlag = false;
	D3DXVECTOR3 SayakaPos = m_pPlayerManager->GetSayaka()->GetPosition();
	for (size_t i = 0; i < m_vTextColPostion.size(); i++) {
		Distance.emplace_back(GetTwoDistanceXYZ(SayakaPos, m_vTextColPostion[i]));
	}
	float tmp = 0.0f;
	for (size_t j = 0; j < m_vTextColPostion.size(); j++) {
		ChangeFlag = false;
		for (size_t i = 0; i < m_vTextColPostion.size() - 1 - j; i++) {
			if (Distance[i] > Distance[i + 1]) {
				//����ւ�����.
				tmp = Distance[i];
				Distance[i] = Distance[i + 1];
				Distance[i + 1] = tmp;
				ChangeFlag = true;	//����ւ�����.
			}
		}
		if (ChangeFlag == false) break;	//����ւ��������Ȃ������̂ŏI��������.
	}
	size_t ColNum = 0;
	for (size_t i = 0; i < m_vTextColPostion.size(); i++) {
		if (Distance[0] == GetTwoDistanceXYZ(SayakaPos, m_vTextColPostion[i])) {
			ColNum = i;
			break;
		}
	}
	size_t Num = 0;

	for (auto m : m_pMap) {
		if (m->GetObjectNo() == enObjectNo::TextEvent) {
			if (Num == ColNum) {
				m->SetPosition(
					D3DXVECTOR3(m->GetPosition().x, -100.0f, m->GetPosition().z));
				break;
			}
			Num++;
		}
	}
	m_TextNum += 1;
	m_pPlayerManager->GetSayaka()->BitFlagOFF(BitFlag::isNextText);
}

float CStage::GetTwoDistanceXYZ(const D3DXVECTOR3 & vMyPos, const D3DXVECTOR3 & vtargetPos)
{
	float distance = sqrtf((vtargetPos.x - vMyPos.x) * (vtargetPos.x - vMyPos.x)
		+ (vtargetPos.y - vMyPos.y) * (vtargetPos.y - vMyPos.y)
		+ (vtargetPos.z - vMyPos.z) * (vtargetPos.z - vMyPos.z));

	return distance;
}

void CStage::InitTextNum(int Num) {
	m_OldTextNum[Num] = CTutorialTextLoad::SearchNowTextArea(Num);
}