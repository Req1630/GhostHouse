#ifndef FADE_UI_H
#define FADE_UI_H

#include "..\..\Resource\SpriteResouce\SpriteResouce.h"
#include "..\..\SingletonBase\SingletonBase.h"

class CFadeUI : CSingletonBase<CFadeUI>
{
	const float MAX_ALPHA	= 1.0f;

public:
	CFadeUI();
	~CFadeUI();

	static void Init( bool isrender = true );
	static void Render();

	static void FadeINStart();
	static void FadeOUTStart();

	static void InitAlpha(){ GetInstance()->m_fAlpha = 0.0f; }

	static bool isActive(){ return GetInstance()->m_isRender; }
	static bool isEnd(){ return GetInstance()->m_isEnd; }
private:
	void Load();
	void Update();

	shared_ptr<CSpriteUI>	m_pFadeSprite;

	bool	m_bFadeIN;
	bool	m_bFadeOUT;
	float	m_fAlpha;

	bool	m_isRender;
	bool	m_isEnd;

};

#endif	// #ifndef FADE_UI_H.