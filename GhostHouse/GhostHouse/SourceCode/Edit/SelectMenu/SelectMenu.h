#ifndef SELECT_MENU_H
#define SELECT_MENU_H

#include "..\EditHeader.h"

class CFileManager;

class CSelectMenu
{
	typedef std::vector<std::string> ObjectName;
public:
	CSelectMenu();
	~CSelectMenu();

	// �`��֐�.
	void Render();

	// �I�u�W�F�N�g���̎擾.
	std::string GetObjectName() const { return m_SelectOjectName; }
	// ���j���[�̕\���ɖ߂�.
	void BackMenu(){ m_isChoice = true; }
	// ���X�e�[�W�̐ݒ�֐�.
	void SetTmpStage( std::vector<edit::RENDER_OBJRCT>* pobj );
	// ���݃I�u�W�F�N�g�I����Ԃ�.
	bool IsChoice() const { return m_isChoice; }

private:
	// �E�B���h�E�̐ݒ�.
	void SettingWindow();
	// �I�u�W�F�N�g�̃^�u�̕\��.
	void ObjectTabRender();
	// �I�u�W�F�N�g���X�g�̕\��.
	bool ObjectListRender( const std::string& objectListName, const int& index );
	// �I�u�W�F�N�g�̑I��.
	bool SelectObject( const std::string& objectName );
	// �I�u�W�F�N�g���̃��X�g���擾.
	void GetObjectListName();

private:
	// �e�I�u�W�F�N�g���̃��X�g.
	ObjectName	m_BackGroundNameList;
	ObjectName	m_GimmickNameList;
	ObjectName	m_PlayerNameList;
	ObjectName	m_GhostNameList;

	std::vector<ObjectName*> m_ObjectList;	// ��̃I�u�W�F�N�g����ێ����邽�߂̃��X�g.

	std::unique_ptr<CFileManager>	m_pFileManager;

	std::string m_SelectOjectName;	// �I���I�u�W�F�N�g��.
	bool m_isChoice;
};

#endif	// #ifndef SELECT_MENU_H.