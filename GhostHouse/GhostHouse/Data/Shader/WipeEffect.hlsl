//��۰��ٕϐ�.
//ø����́Aڼ޽� t(n).
Texture2D		g_Texture : register( t0 );
//����ׂ́Aڼ޽� s(n).
SamplerState	g_samLinear		: register( s0 );
SamplerState	g_samLinear1	: register( s1 );

//�ݽ����ޯ̧.
cbuffer global : register( b0 )
{
	matrix	g_mW			: packoffset( c0 );	// ܰ��ލs��.
	float	g_fViewPortW	: packoffset( c4 );	// �r���[�|�[�g��.
	float	g_fViewPortH	: packoffset( c5 );	// �r���[�|�[�g����.
	float	g_fAlpha		: packoffset( c6 );	// �A���t�@�l.
	float2	g_vUV			: packoffset( c7 );	// UV���W.
	float	g_radius		: packoffset( c8 );
	float2	g_TargetPos		: packoffset( c9 );
};

//�\����.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD;
};

// ���_�V�F�[�_.
VS_OUTPUT VS_Main( float4 Pos : POSITION,
	float2 Tex:TEXCOORD )
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul( Pos, g_mW );

	// �X�N���[�����W�ɍ��킹��v�Z,
	output.Pos.x = ( output.Pos.x / g_fViewPortW ) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - ( output.Pos.y / g_fViewPortH ) * 2.0f;

	output.Tex = Tex;

	// UV���W�����炷.
	output.Tex.x += g_vUV.x;
	output.Tex.y += g_vUV.y;

	return output;
}

// �s�N�Z���V�F�[�_.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float4 color = g_Texture.Sample( g_samLinear, input.Tex );

	float a = g_TargetPos.x - input.Pos.x;
	float b = g_TargetPos.y - input.Pos.y;
	float c = sqrt( a * a + b * b );

	// �摜���~�`�͈͓̔��������A���t�@�����ɂ���.
	if( c <= g_radius ){
		color.a = 0.1f;
	}

	return color;
}