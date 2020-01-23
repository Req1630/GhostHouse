#ifndef FADE_UI_H
#define FADE_UI_H

#include "..\..\Resource\SpriteResouce\SpriteResouce.h"
#include "..\..\SingletonBase\SingletonBase.h"

class CFadeUI : CSingletonBase<CFadeUI>
{
	const float MIN_ALPHA	= 0.0f;
	const float MAX_ALPHA	= 1.0f;
	const float FADE_VALUE	= 0.02f;

public:
	CFadeUI();
	~CFadeUI();

	// �������p�֐�.
	static void Init( bool isrender = true );
	// �`��֐�.
	static void Render();

	// �t�F�[�h�C���J�n�֐�.
	static void FadeINStart();
	// �t�F�[�h�A�E�g�J�n�֐�.
	static void FadeOUTStart();

	// �A���t�@�l�̏������֐�.
	static void InitAlpha(){ GetInstance()->m_ImageAlpha = 0.0f; }

	// �t�F�[�h����.
	static bool isActive(){ return GetInstance()->m_isRender; }
	// �t�F�[�h���I��������.
	static bool isEnd(){ return GetInstance()->m_isFadeEnd; }

private:
	// �Ǎ��֐�(Render�ɓ���).
	void Load();
	// �X�V�֐�(Render�ɓ���).
	void Update();

private:
	shared_ptr<CSpriteUI>	m_pFadeSprite;	// �t�F�[�h�p�̉摜.

	bool	m_bFadeIN;		// �t�F�[�h�C�����Ă��邩�ǂ���.
	bool	m_bFadeOUT;		// �t�F�[�h�A�E�g���Ă��邩�ǂ���.
	float	m_ImageAlpha;	// �摜�̃A���t�@�l.
	bool	m_isRender;		// �`�悵�Ă��邩.
	bool	m_isFadeEnd;	// �t�F�[�h���I�����Ă��邩.

};

#endif	// #ifndef FADE_UI_H.