#ifndef TITLE_UI_H
#define TITLE_UI_H

#include "..\..\Resource\SpriteResouce\SpriteResouce.h"

class CTitleUI
{
public:
	enum enTitleMenuState
	{
		None,

		Start,	// �Q�[���X�^�[�g.
		End,	// �Q�[���I��.
		Edit,

		Max,
	};

private:
	float MAX_CURSOR_POSITION_Y;		// �J�[�\���̍ő�l.
	float MIN_CURSOR_POSITION_Y;		// �J�[�\���̍ŏ��l.
	float CURSOR_MOVE_SPEED;			// �J�[�\���̈ړ��l.
	const int STICK_IDLING	= 30000;	// �X�e�B�b�N�̗V��.


public:
	CTitleUI();
	~CTitleUI();

	bool Load();

	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render();

	// �J�[�\��, ��ւ̑���.
	void GiveCursorUP();
	// �J�[�\��, ���ւ̑���.
	void GiveCursorDOWN();
	// ���݂̃��j���[��Ԏ擾�֐�
	enTitleMenuState GetNowMenuState(){ return m_enTitleMenuState; }

private:
	// ------ �e�摜�̃��[�h�֐� ------.
	// ���ׂẲ摜.
	bool LoadAllImage();	
	// �^�C�g���摜. 
	bool LoadTitltImage();	
	// "�X�^�[�g"�e�L�X�g�摜.
	bool LoadStartImage();
	// "�I��"�e�L�X�g�摜.
	bool LoadEndImage();
	// �I���J�[�\���̉摜.
	bool LoadCursorImage();

private:
	// �^�C�g�����j���[.
	enTitleMenuState	m_enTitleMenuState;

	// �^�C�g���摜.
	shared_ptr<CSpriteUI>	m_pTitleImage;
	// �Q�[���X�^�[�g�摜.
	shared_ptr<CSpriteUI>	m_pStartImage;
	// �Q�[���I���摜.
	shared_ptr<CSpriteUI>	m_pEndImage;
	// �J�[�\���摜.
	shared_ptr<CSpriteUI>	m_pCursor;

	// �J�[�\�����W.
	D3DXVECTOR3	m_vCursorPosition[2];
	bool m_bPushDownKey;
	bool m_bPushUpKey;
};

#endif	// #ifndef TITLE_UI_H.