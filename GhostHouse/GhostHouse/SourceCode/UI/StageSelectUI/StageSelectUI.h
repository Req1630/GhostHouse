#ifndef STAGE_SELECT_UI_H
#define STAGE_SELECT_UI_H

#include "..\..\Resource\SpriteResouce\SpriteResouce.h"
#include "..\..\Resource\MapLoad\MapLoad.h"
#include "..\..\DebugMode\DebugText\DebugText.h"


class CStageSelectUI
{
private:
	// ���摜�����T�C�Y�l.
	float ARROW_INIT_SIZE = 1.0f;
	// ���T�C�Y�ύX�J�E���g�l.
	float ARROW_SIZE_CHNGE_COUNT = 3.0f;
	// ���T�C�Y�ύX�J�E���g�ő�l.
	float ARROW_SIZE_CHNGE_COUNT_MAX = 60.0f;
	// ���ŏ��T�C�Y�l.
	float ARROW_SIZE_MINIMUM_VALUE = 0.8f;

public:
	CStageSelectUI();
	~CStageSelectUI();

	bool Load();

	// �`��֐�.
	void Render();

	// �X�e�[�W��I���֐�.
	void MoveUpStage();
	// �X�e�[�W���I���֐�.
	void MoveDownStage();
	// �X�e�[�W������.
	bool SetStageName();

private:
	// ���摜�T�C�Y�ύX�֐�.
	void ArrowSizeChange();
	// �X�e�[�W�ύX�֐�.
	void StageChange();

	/****************************
	*	�� �摜�n.
	*/
	// ����摜�`��.
	void UpArrowRender();
	// �����摜�`��.
	void DownArrowRender();

	// ���ׂẲ摜�̓ǂݍ���.
	bool LoadAllImage();
	// �X�e�[�W�Z���N�g�^�C�g���摜�ǂݍ���.
	bool LoadStageSelectTitle();

	// ����摜�ǂݍ��݊֐�.
	bool LoadUpArrow();
	// �����摜�ǂݍ��݊֐�.
	bool LoadDownArrow();

	// �X�e�[�W���g�摜�ǂݍ��݊֐�.
	bool LoadStageNameFrame();

private:
	// ���݂̃X�e�[�W�ԍ�.
	int m_iNowStageNumber;

	// �X�e�[�W��ړ��t���O.
	bool m_bMoveUpStageFlag;
	// �X�e�[�W���ړ��t���O.
	bool m_bMoveDownStageFlag;

	// ���摜�T�C�Y.
	float m_fArrowSize;
	// �T�C�Y�̃J�E���g.
	float m_fSizeCount;

	bool	m_bPlaySE;

	/****************************
	*	�� �摜�n.
	*/

	// �X�e�[�W�Z���N�g�^�C�g���摜.
	shared_ptr<CSpriteUI>	m_pStageSelectTitle;
	// ����摜.
	shared_ptr<CSpriteUI>	m_pUpArrows;
	// �����摜.
	shared_ptr<CSpriteUI>	m_pDownArrows;
	// �X�e�[�W���g�摜.
	shared_ptr<CSpriteUI>	m_pStageNameFrame;
};

#endif	// #ifndef STAGE_SELECT_UI_H.