#ifndef STAGE_H
#define STAGE_H

#include "..\..\Resource\MapLoad\MapLoad.h"
#include "..\Gimmick\Signboard\Signboard.h"
#include "..\..\DebugMode\DebugText\DebugText.h"
#include "..\..\Object\Character\Player\Manager\PlayerManager.h"
#include "..\..\UI\Tutorial\TutorialText.h"
#include "..\..\Object\ObjectManager\ObjectManager.h"

class CStage
{
public:
	enum class MapScene
	{
		None = -1,

		In_the_forest = 0,		// �X�̒�.
		In_front_house,			// �Ƃ̑O.
		In_the_house_F1,		// �Ƃ̒�.
		In_the_house_F2,		// �Ƃ̒�.
		In_the_house_B1_Area1,	// �Ƃ̒n��.
		In_the_house_B1_Area2,	// �Ƃ̒n���G���A2.
		In_the_house_F1_end,	// �Ƃ̒�.

		Max,
	};

	const float WINDOW_OUT_SIZE = 200.0f;
public:
	CStage();
	~CStage();

	// �ǂݍ��݊֐�.
	void Load(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	// �X�V�֐�.
	void Updata();
	// �`��֐�.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj, Light& stLight, stCAMERA& stCamera);

	// �R���e�j���[�̍ۃX�e�[�W�̃��Z�b�g.
	void ContinueStageReset();

	// �`��\��.
	bool isRender();
	// �S�[������֐�.
	bool isGaol();
	// �T���J���ꔻ��֐�.
	bool isSayakaDead();
	// �T���J��HP��0�Ȃ�true.
	bool isSayakaLifePointZero();

	// �|�[�Y���Ă��邩�ǂ����ݒ�.
	void SetPause(const bool& isPause) { m_isPause = isPause; }

	D3DXVECTOR3 GetPlayerPosition()
	{
		if (m_pPlayerManager->GetSayaka() == nullptr) return { 0.0f, 0.0f, 0.0f };
		return m_pPlayerManager->GetSayaka()->GetPosition();
	}

	void SetCameraOFF()
	{
		if (m_pPlayerManager->GetSayaka() == nullptr) return;
		m_pPlayerManager->GetSayaka()->BitFlagOFF(BitFlag::isStopCamera);
	}
private:
	// �}�b�v�̓ǂݍ���.
	bool LoadMap();
	// �}�b�v�I�u�W�F�N�g�̓ǂݍ���.
	void LoadMapObject(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	// �v���C���[�̍쐬.
	void PlayerInit(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	// ���̃}�b�v�ɕύX.
	void ChangeNextMap();

	// �^�C�g�����̃��C�g���W�̏�����.
	void InitLightPosition(Light& stLight);

	// �w�i�̕`��.
	void BackGroundRender(D3DXMATRIX& mView, D3DXMATRIX& mProj, Light& stLight, stCAMERA& stCamera);

	// �e�L�X�g�����ɐi�߂铖���蔻��̌�,�ʒu�����.
	void GetNextTextColPosition();
	// �e�L�X�g��\��.
	void DispText(shared_ptr<CObjectBase> pObj);
	// �e�L�X�g�����̕��ɕύX.
	void ChangeText();

	// �O�������W�̓�_�ԋ����𑪂�.
	float GetTwoDistanceXYZ(const D3DXVECTOR3 & vMyPos, const D3DXVECTOR3 & vtargetPos);


	void InitTextNum(int Num);
private:
	int init_Map;
	int m_MapSize;
	bool m_isLightSet;
	bool m_bOneSound;
	bool m_bOneCall;
	bool m_bStartGame;
	bool m_isPause;
	bool m_isRender;
	bool m_isStopCamera;

	size_t		m_TextStartNum;
	size_t		m_TextNum;
	vector<int>	m_OldTextNum;
	vector<int>	m_vTextColNum;

	MapScene m_NowMapScene;

	shared_ptr<CDX9Mesh>			m_pBackGround;
	unique_ptr<CPlayerManager>		m_pPlayerManager;
	unique_ptr<CObjectManager>		m_pObjectManager;
	vector<shared_ptr<CObjectBase>> m_pMap;
	vector<vector<shared_ptr<CObjectBase>>>	m_pMapList;		// �}�b�v.
	vector<D3DXVECTOR3>				m_vTextColPostion;

	unique_ptr<CMultiTexture>		m_pPaper;
	unique_ptr<CTutorialText>		m_pText;
};

#endif	// #ifndef STAGE_H.