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

	// �`��֐�.
	void Render( bool& isEditEnd );
	// �I�u�W�F�N�g�̃J�E���g���ݒ�.
	void SetObjectCount( const int& size ){ m_ObjectCount = size; }

private:
	// ��������`��֐�.
	void ManipulationDescriptionRender( bool& isEditEnd );
	// �G�f�B�b�g�I���E�B���h�E�`��֐�.
	void EditEndPopRender( bool& isEditEnd );

private:
	int m_ObjectCount;	// �I�u�W�F�N�g��.

	std::vector<std::string> m_pStageNameList;	// �X�e�[�W�����X�g.

	bool m_isEndPopRender;	// �I���E�B���h�E�\���t���O.
};

#endif	// #ifndef EDIT_IMGUI_H.