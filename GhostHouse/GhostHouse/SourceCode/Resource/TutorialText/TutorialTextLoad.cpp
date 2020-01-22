#include "TutorialTextLoad.h"
#include <fstream>

CTutorialTextLoad::CTutorialTextLoad()
{

	InitHiraganaBox();
}
CTutorialTextLoad::~CTutorialTextLoad()
{
}

void CTutorialTextLoad::TextDataLoad()
{
	// �t�@�C�����J��.
	fstream filestream(GetInstance()->TEXT_FILENAME);
	if (!filestream.is_open()) return;

	int i = 0;
	// �t�@�C����ǂݍ���.
	while (!filestream.eof()) {
		string buffer;
		//Text�t�@�C������ǂݎ����1�s��string�ϐ��ubuffer]
		filestream >> buffer;
		//�v�f�𑝂₵�ď�����.
		//GetInstance()->TextBox.emplace_back();
		GetInstance()->m_vbAsteriskFlag.emplace_back(false);
		GetInstance()->TextBox.emplace_back(buffer);
		GetInstance()->For_searchTextBox.emplace_back(buffer);
		i++;
	}

	GetInstance()->m_HiragnaBoxSize = GetInstance()->HiraganaBox.size();

}

D3DXVECTOR2 CTutorialTextLoad::SearchCharPosition(size_t TextBoxNum, size_t TwoArray)
{
	if (TextBoxNum > GetInstance()->TextBox.size())return D3DXVECTOR2(7, 3);

	// 1�����擾.
	string Tmp;
	//���{���2�o�C�g�Ȃ̂ŁAsubstr��
	//(�J�n�ʒu�u�����������v�Łu���v�Ȃ�4, �o�C�g��(���{��Ȃ�2)���w�肵�Ď��o��.
	Tmp = GetInstance()->TextBox[TextBoxNum].substr(TwoArray, 2);
	int X = 0;
	int Y = 0;
	int NegativNumX = 1;
	int NegativNumY = 5;
	int Count = 0;
	if (Tmp == " " || Tmp == "�@") {
		return D3DXVECTOR2(0, 3);
	}
	// X = �Z�Z �̐��l�͍ő剡��.W 1600 �Ȃ� 16.
	for (size_t i = 0; i < GetInstance()->m_HiragnaBoxSize - 1; i += 2) {
		if (Tmp.c_str() == GetInstance()->HiraganaBox.substr(i, 2)) {
			X = 16 - NegativNumX;
			Y = 5 - NegativNumY;
			return D3DXVECTOR2((FLOAT)X, (FLOAT)Y);
		}
		else {
			Count++;
			// �u���v�̉��ɂ͉����Ȃ��̂ł�����Ƃ��}�b�N�X�ɂ���.
			if (GetInstance()->HiraganaBox.substr(i, 2) == "��") {
				Count = 5;
			}
			NegativNumY -= 1;
		}
		if (Count == 5) {
			NegativNumX += 1;
			NegativNumY = 5;
			Count = 0;
		}
	}
	return D3DXVECTOR2(7, 3);


}

bool CTutorialTextLoad::SearchFiristAsterisk(int ArrayNum)
{
	if (ArrayNum < 0) {
		return false;
	}
	// "��"�����������e�L�X�g�̔z��ԍ��ɔ����t���O�������Ă����炱����true��Ԃ�.
	if (GetInstance()->m_vbAsteriskFlag[ArrayNum] == true) {
		return true;
	}
	if (GetInstance()->TextBox[ArrayNum].find("��") != std::string::npos) {
		// �擪��"��"���폜.
		// ���̕����Əd�Ȃ�̂�2�����.
		GetInstance()->TextBox[ArrayNum].erase(GetInstance()->TextBox[ArrayNum].begin());
		GetInstance()->TextBox[ArrayNum].erase(GetInstance()->TextBox[ArrayNum].begin());
		GetInstance()->m_vbAsteriskFlag[ArrayNum] = true;
		return true;
	}
	return false;
}

int CTutorialTextLoad::SearchNowTextArea(int AreaNumber)
{
	if (AreaNumber < 0) {
		return false;
	}
	// "�Q�`�q�d�`"�ɃX�e�[�W�i���o�[����������.
	string AreaName = "�Q�`�q�d�`" + GetInstance()->ConversionHalfChar(AreaNumber);
	int ArrayofNumber = GetInstance()->TextBox.size();
	for (int i = 0; i < ArrayofNumber; i++) {
		// �e�L�X�g�� "�Q�`�q�d�`�Z"�����Ă��镨��T��.
		if (GetInstance()->For_searchTextBox[i].find(AreaName) != std::string::npos) {
			// �\���p��"�Q�`�q�d�`�Z�������Ă��Ȃ���Ώ���."
			if (GetInstance()->TextBox[i].find(AreaName) != std::string::npos) {
				GetInstance()->TextBox[i].erase(GetInstance()->TextBox[i].find(AreaName), GetInstance()->TextBox[i].back());
			}
			// ���������Ƃ���̃e�L�X�g�z��ԍ���Ԃ�.
			return i;
		}
	}

	return 0;
}

string CTutorialTextLoad::ConversionHalfChar(int Number)
{

	switch (Number)
	{
	case 0:
		return "�O";
	case 1:
		return "�P";
	case 2:
		return "�Q";
	case 3:
		return "�R";
	case 4:
		return "�S";
	case 5:
		return "�T";
	case 6:
		return "�U";
	default:
		break;
	}
	return "�|�P";
}

void CTutorialTextLoad::InitHiraganaBox()
{
	HiraganaBox = "����������"
		"����������"
		"����������"
		"�����Ă�"
		"�Ȃɂʂ˂�"
		"�͂Ђӂւ�"
		"�܂݂ނ߂�"
		"����H�I"
		"������"
		"�����B�A"
		"����������"
		"����������"
		"�����Âł�"
		"�΂тԂׂ�"
		"�ς҂Ղ؂�"
		"�[��";
}

