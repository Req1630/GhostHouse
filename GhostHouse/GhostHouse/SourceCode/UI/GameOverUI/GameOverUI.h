#ifndef GAME_OVER_UI_H
#define GAME_OVER_UI_H

#include "..\..\Resource\SpriteResouce\SpriteResouce.h"

class CGameOverUI
{
public:
	enum enGameOverMenuState
	{
		None,

		Continue,	// �Q�[����.
		End,		// �^�C�g����.

		Max,
	};
private:
	float MAX_CURSOR_POSITION_Y;		// �J�[�\���̍ő�l.
	float MIN_CURSOR_POSITION_Y;		// �J�[�\���̍ŏ��l.
	float CURSOR_MOVE_SPEED;			// �J�[�\���̈ړ��l.
	const int STICK_IDLING = 30000;		// �X�e�B�b�N�̗V��.

public:
	CGameOverUI();
	~CGameOverUI()
	{
	};

	// �X�V�֐�.
	void Upadate();
	// �`��֐�.
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera );

	// �J�[�\��, ��ւ̑���.
	void GiveCursorUP();
	// �J�[�\��, ���ւ̑���.
	void GiveCursorDOWN();
	// ���݂̃��j���[��Ԏ擾�֐�
	enGameOverMenuState GetNowMenuState()
	{
		return m_enGameOverMenuState;
	}

private:
	// ----- �e�摜�̃��[�h�֐� ------.
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
	enGameOverMenuState	m_enGameOverMenuState;

	// �|�[�Y�^�C�g���摜.
	shared_ptr<CSpriteUI>	m_pGameOverTitleImage;
	// �^�C�g���֖߂�摜.
	shared_ptr<CSpriteUI>	m_pContinueImage;
	// �Q�[���֖߂�摜.
	shared_ptr<CSpriteUI>	m_pEndImage;
	// �J�[�\���摜.
	shared_ptr<CSpriteUI>	m_pCursor;

	// �J�[�\�����W.
	D3DXVECTOR3	m_vCursorPosition[2];
	bool m_bPushDownKey;
	bool m_bPushUpKey;
};

#endif	// #ifndef GAME_OVER_UI_H.