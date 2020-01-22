#ifndef C_TUTORIAL_TEXT_H
#define C_TUTORIAL_TEXT_H
#include "..\..\Resource\SpriteResouce\SpriteResouce.h"
#include "..\..\ObjectBase\ObjectBase.h"

class CTutorialText
{

public:
	CTutorialText();
	~CTutorialText();

	bool Load();
	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render(int ArrayNumber, shared_ptr<CObjectBase> pObj);

	void CharRender(float UV_X, float UV_Y);

	void SetFontPosition(float PositionX, float PositionY, float PositionZ);
	bool LoadAllImage();

	void TextDraw(int ArrayNumber, shared_ptr<CObjectBase> pObj);

	static void SetArrayNumber(int arrayNumber)
	{
		m_sArrayNumber = arrayNumber;
	}
private:

	bool LoadFontImage();

	bool LoadWindowImage();
	shared_ptr<CSpriteUI>	m_pTutorialFont;		//�`���[�g���A���e�L�X�g�p.
	shared_ptr<CSpriteUI>	m_pMessageWindow;		//�`���[�g���A�����b�Z�[�W�E�B���h�E.

	D3DXVECTOR3				m_vTextPosition;		//�`���[�g���A���e�L�X�g�|�W�V����.
	D3DXVECTOR3				m_vMessageWndPosition;	//�`���[�g���A�����b�Z�[�W�E�B���h�E�|�W�V����

	int		m_TextNumber;
	int		m_TextCount;
	int		m_OldTextArray;
	bool	m_bChange;
	int		m_Count;

	static int m_sArrayNumber;
};




#endif //#ifndef C_TUTORIAL_TEXT_H