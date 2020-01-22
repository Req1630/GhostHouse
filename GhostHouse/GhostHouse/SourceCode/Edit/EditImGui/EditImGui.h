#ifndef EDIT_IMGUI_H
#define EDIT_IMGUI_H

#include "..\EditHeader.h"

class CFileManager;

class CEditImGui
{
	const float IMGUI_WND_W = 350.0f;
	const float IMGUI_WND_H = 300.0f;
	const float IMGUI_WND_X = 850.0f;
	const float IMGUI_WND_Y = 350.0f;
public:
	CEditImGui();
	~CEditImGui();

	void Render( bool& isEditEnd );
	void SetObjectCount( const int& size ){ m_ObjectCount = size; }

private:
	void ManipulationDescriptionRender( bool& isEditEnd );
	void EditEndPopRender( bool& isEditEnd );

private:
	int m_ObjectCount;

	std::vector<std::string>			m_pStageNameList;

	bool m_isEndPopRender;
};

#endif	// #ifndef EDIT_IMGUI_H.