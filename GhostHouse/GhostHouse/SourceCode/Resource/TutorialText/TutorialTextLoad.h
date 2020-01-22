#ifndef C_TUTORIAL_TEXTLOAD_H
#define C_TUTORIAL_TEXTLOAD_H

#include "..\..\SingletonBase\SingletonBase.h"
#include "..\..\Main\Global.h"

using namespace std;

class CTutorialTextLoad : public CSingletonBase<CTutorialTextLoad>
{
private:
	const char* TEXT_FILENAME = "Data\\Tutorial.txt";	// �e�L�X�g�p�X.
public:
	CTutorialTextLoad();
	~CTutorialTextLoad();
	// �f�[�^���[�h.
	static void TextDataLoad();
	// �e�L�X�g�̎q�z��̐���Ԃ��֐�.
	static int GetTextArraySize(int i)
	{
		return GetInstance()->TextBox[i].size();
	}
	// �e�L�X�g�̔z�񐔂�Ԃ��֐�.
	static int GetAllTextSize()
	{
		return GetInstance()->TextBox.size();
	}
	// �e�L�X�g�ɍ��킹���A�t�H���g��UV���W������.
	static D3DXVECTOR2 SearchCharPosition(size_t TextBoxNum, size_t TwoArray);
	// �A�X�^���X�N������.
	static bool SearchFiristAsterisk(int ArrayNum);
	// �G���A���ς��^�C�~���O�̃e�L�X�g������(�K��).
	static int SearchNowTextArea(int AreaNumber);
private:
	// ���p������S�p������.
	string ConversionHalfChar(int Number);
	// �Ђ炪�Ȃ����Ă�����.
	void InitHiraganaBox();
	// �񎟌��z��.
	vector<string> TextBox;
	// �K�������p.
	vector<string> For_searchTextBox;
	//�Ђ炪�Ȃ����Ă����ϐ�.
	string HiraganaBox;
	size_t	m_HiragnaBoxSize;
	vector<bool>	m_vbAsteriskFlag;

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CTutorialTextLoad(const CTutorialTextLoad &) = delete;
	CTutorialTextLoad& operator = (const CTutorialTextLoad &) = delete;
	CTutorialTextLoad(CTutorialTextLoad &&) = delete;
	CTutorialTextLoad& operator = (CTutorialTextLoad &&) = delete;
};



#endif // !C_TUTORIAL_TEXTLOAD_H
