#ifndef EACH_ITEM_H
#define EACH_ITEM_H

#include "..\..\EditHeader.h"

class CEachItem
{
public:
	CEachItem()
		: m_NowSelectItem	( edit::enSelect::bgin )
	{}

	~CEachItem()
	{}

	void Update();
	void Render();

	edit::SelectItem GetNowSelectitem() const { return m_NowSelectItem-1; };

private:
	void EachItemNameListRender();
	void SelectingCursorRender();

private:
	edit::SelectItem m_NowSelectItem;
};

#endif	// #ifndef EACH_ITEM_H.