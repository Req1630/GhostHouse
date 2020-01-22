#ifndef CRAY_H
#define CRAY_H

#include "..\..\Main\Global.h"

// ���C�\���N���X.
class CRay
{
public:
	// ���C�\����.
	struct RAY
	{
		D3DXVECTOR3 Point[2];	// �n�_, �I�_.
		D3DXVECTOR3 Direction;	// ����.
		D3DXVECTOR3 Pos;		// �ʒu.
		FLOAT		fYaw;		// Y����]�l.
		RAY()
			: Point		()
			, Direction	()
			, Pos		()
			, fYaw		( 0.0F )
		{}
	};

	CRay();
	~CRay();

	// �������֐�.
	HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );

	//����ލ쐬.
	HRESULT InitShader();

	// ���C������.
	HRESULT InitModel();

	// �`��.
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj );


	// ���C�\����.
	RAY		m_Ray;

private:
	//======================================
	//	�\����.
	//======================================
	//�ݽ����ޯ̧�̱��ؑ��̒�`.
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX	mWVP;		//ܰ���,�ޭ�,��ۼު���݂̍����ϊ��s��.	
		D3DXVECTOR4	vColor;		//�װ(RGBA�̌^�ɍ��킹��).
	};
	//���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//���_���W.
	};

	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;

	//�����ق̎�ނ��Ƃɗp��.
	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pConstantBuffer;	//�ݽ����ޯ̧.

	//�����ق��Ƃɗp��.
	ID3D11Buffer*			m_pVertexBuffer;	//���_�ޯ̧.
};

#endif // #ifndef CRAY_H.