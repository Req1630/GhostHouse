#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "..\Gimmick\Signboard\Signboard.h"

#include <vector>
#include <memory>

class CObjectManager
{
public:
	CObjectManager();
	~CObjectManager();

	void Init(
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11, 
		std::vector<std::shared_ptr<CObjectBase>>& vMap,
		std::unique_ptr<CMultiTexture>& pPaper );
	void UIRender( std::shared_ptr<CObjectBase> pObj );

	D3DXVECTOR3 GetNearPosition( std::shared_ptr<CObjectBase> pObj ) const;
	D3DXVECTOR3 GetInvisibleFloorCenterPosition() const { return m_vInvisibleFloorCenterPosition; }
	void SetInitFlagUnload(){ m_isInit = false; }

private:
	// �I�u�W�F�N�g�̕��ёւ�.
	void Sort();
	// �Ŕ̍쐬.
	bool InitSignboard( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, std::unique_ptr<CMultiTexture>& pPaper );
	// �����Ȃ����̒��S���W���擾����.
	void InvisibleFloorCenterPositionSearch();
	// �����Ȃ����̓����蔻��̍��W���擾.
	void InvisibleCollisionPositionSearch();
	// �����Ȃ����̃��[�g�摜�̍쐬.
	void InitSignboardSpraite( std::shared_ptr<CObjectBase> pObj, std::vector<CMultiTexture::SPRITE_STATE>& ss );
	// �����Ȃ����̃��[�g�摜�̍��W����.
	void InitSignboardRoutePosition( std::vector<CMultiTexture::SPRITE_STATE>& ss );

private:
	std::vector<std::shared_ptr<CObjectBase>> m_Map;
	std::unique_ptr<CMultiTexture> m_pPaper;
	bool m_isInit;

	D3DXVECTOR3 m_vInvisibleFloorCenterPosition;
	D3DXVECTOR3 m_vMaxPositin;
	D3DXVECTOR3 m_vMinPositin;
};

#endif	// #ifndef OBJECT_MANAGER_H.