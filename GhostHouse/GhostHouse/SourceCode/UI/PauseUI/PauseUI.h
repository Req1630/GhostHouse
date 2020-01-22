#ifndef PAUSE_H
#define PAUSE_H

#include "..\..\Resource\SpriteResouce\SpriteResouce.h"

class CPauseUI
{
public:
	enum enPauseMenuState
	{
		None,

		ToGame,		// �Q�[����.
		ToTitle,	// �^�C�g����.

		Max,
	};

private:
	float MAX_CURSOR_POSITION_Y;		// �J�[�\���̍ő�l.
	float MIN_CURSOR_POSITION_Y;		// �J�[�\���̍ŏ��l.
	float CURSOR_MOVE_SPEED;			// �J�[�\���̈ړ��l.
	const int STICK_IDLING = 30000;		// �X�e�B�b�N�̗V��.


public:
	CPauseUI();
	~CPauseUI()
	{
	};

	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render();

	// �J�[�\��, ��ւ̑���.
	void GiveCursorUP();
	// �J�[�\��, ���ւ̑���.
	void GiveCursorDOWN();
	// ���݂̃��j���[��Ԏ擾�֐�
	enPauseMenuState GetNowMenuState()
	{
		return m_enPauseMenuState;
	}

private:
	// ------ �e�摜�̃��[�h�֐� ------.
	// ���ׂẲ摜.
	bool LoadAllImage();
	// �^�C�g���摜. 
	bool LoadPauseTitltImage();
	// "�^�C�g����"�e�L�X�g�摜.
	bool LoadToTilteImage();
	// "�Q�[����"�e�L�X�g�摜.
	bool LoadToGameImage();
	// �I���J�[�\���̉摜.
	bool LoadCursorImage();

private:
	// �|�[�Y���j���[.
	enPauseMenuState	m_enPauseMenuState;

	// �|�[�Y�^�C�g���摜.
	shared_ptr<CSpriteUI>	m_pPauseTitleImage;
	// �^�C�g���֖߂�摜.
	shared_ptr<CSpriteUI>	m_pToTitleImage;
	// �Q�[���֖߂�摜.
	shared_ptr<CSpriteUI>	m_pToGameImage;
	// �J�[�\���摜.
	shared_ptr<CSpriteUI>	m_pCursor;

	// �J�[�\�����W.
	D3DXVECTOR3	m_vCursorPosition[2];
	bool m_bPushDownKey;
	bool m_bPushUpKey;
};

#endif	// #ifndef PAUSE_H.