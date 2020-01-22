#include "TutorialText.h"
#include "..\..\Resource\TutorialText\TutorialTextLoad.h"

int CTutorialText::m_sArrayNumber = 0;

CTutorialText::CTutorialText()
	: m_pTutorialFont(nullptr)
	, m_pMessageWindow(nullptr)
	, m_TextCount(0)
	, m_OldTextArray(-1)
	, m_bChange(false)
	, m_Count(0)
{
}

CTutorialText::~CTutorialText()
{
}

bool CTutorialText::Load()
{
	// �摜���ǂݍ��߂Ă�����true��Ԃ�.
	if (LoadAllImage() == true) return true;
	return false;
}

void CTutorialText::Update()
{
}

void CTutorialText::Render(int ArrayNumber, shared_ptr<CObjectBase> pObj)
{
	if (m_pMessageWindow == nullptr) return;
	m_pMessageWindow->Render();
	TextDraw(ArrayNumber, pObj);
}

void CTutorialText::CharRender(float UV_X, float UV_Y)
{
	if (m_pMessageWindow == nullptr) return;
	m_pTutorialFont->Render(UV_X, UV_Y);

}

void CTutorialText::SetFontPosition(float PositionX, float PositionY, float PositionZ)
{
	if (m_pTutorialFont == nullptr) return;
	m_pTutorialFont->SetPosition(PositionX, PositionY, PositionZ);
}

bool CTutorialText::LoadAllImage()
{
	if (LoadFontImage() == false) return false;
	if (LoadWindowImage() == false) return false;

	return true;
}

void CTutorialText::TextDraw(int ArrayNumber, shared_ptr<CObjectBase> pObj)
{
	if (LoadAllImage() == false)return;
	// �e�L�X�g�̍ő吔�Ǝw��z��i���o�[���d�Ȃ�ƁA�z�񂪃I�[�o�[����̂�(�Y������0�X�^�[�g�Ȃ̂�).
	// ���̏ꍇ��l�����炷.
	if (CTutorialTextLoad::GetAllTextSize() == ArrayNumber) {
		ArrayNumber = CTutorialTextLoad::GetTextArraySize(ArrayNumber) - 1;
	}

	int xx = 0;
	//�e�L�X�g���؂�ւ�����當������p�ϐ���������.
	if (m_OldTextArray != ArrayNumber) {
		//���̐擪��"��"�������ꍇ�A��莞�Ԍo�߂Ŏ����Ŏ��̃e�L�X�g�ɍs��.
		if (CTutorialTextLoad::SearchFiristAsterisk(ArrayNumber) == true) {
			m_bChange = true;
			m_Count = 0;
		}
		m_TextNumber = 0;
		m_OldTextArray = ArrayNumber;
	}
	if (m_TextNumber < CTutorialTextLoad::GetTextArraySize(ArrayNumber)) {
		//5�t���[�����ɕ�������p�ϐ��C���N�������g.
		if (m_TextCount < 3) {
			m_TextCount++;
		}
		else {
			m_TextCount = 0;
		}
		//5�t���[�����ɕ�������.
		if (m_TextCount % 2 == 0) {
			m_TextNumber++;
			m_TextCount = 0;
		}
	}
	else {
		//�����Ŏ��̃e�L�X�g�ɑ���.
		if (m_bChange == true) {
			m_Count++;
			if (m_Count >= 35) {
				m_bChange = false;
				pObj->BitFlagON(BitFlag::isNextText);
				return;
			}
		}
	}

	for (int x = 0; x < m_TextNumber; x += 2) {
		D3DXVECTOR2 Tmp = CTutorialTextLoad::SearchCharPosition(ArrayNumber, x);
		SetFontPosition(60.0f + (60.0f * (float)xx), 660.0f, 0.7f);
		CharRender(Tmp.x, Tmp.y);
		xx++;
	}
}

bool CTutorialText::LoadFontImage()
{
	// "�X�e�[�W�Z���N�g"�̃^�C�g���摜�̓ǂݍ���.
	if (m_pTutorialFont != nullptr) return true;
	m_pTutorialFont = CSpriteResource::GetSprite("font");
	return false;
}

bool CTutorialText::LoadWindowImage()
{
	// "�X�e�[�W�Z���N�g"�̃^�C�g���摜�̓ǂݍ���.
	if (m_pMessageWindow != nullptr) return true;
	m_pMessageWindow = CSpriteResource::GetSprite("Window");
	return false;
}
