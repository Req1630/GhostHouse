//��۰��ٕϐ�.

//ø����́Aڼ޽� t(n).
Texture2D		g_Texture : register(t0);
//����ׂ́Aڼ޽� s(n).
SamplerState	g_samLinear : register(s0);

//�ݽ����ޯ̧.
cbuffer global
{
	matrix	g_mW;		//ܰ��ލs��.
	matrix	g_mWVP;		//ܰ���,�ޭ�,��ۼު���݂̍����s��.
	float4	g_vLightDir;//ײĂ̕���(�޸��).
	float4	g_vColor;	//�ި̭��ސF(�g�U���ːF). 
	float4	g_vCamPos;	//��׈ʒu(���_�ʒu).
};

//�\����.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float4	Color		: COLOR;
	float3	Light		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	EyeVector	: TEXCOORD2;
	float2	Tex			: TEXCOORD3;
};

//-------------------------------------------------
//	���_(�ްï��)�����.
//-------------------------------------------------
VS_OUTPUT VS(float4 Pos		: POSITION,	//�ʒu.
			 float4 Normal	: NORMAL,	//�@��.
			 float2 Tex		: TEXCOORD )//ø���.
{
	VS_OUTPUT output = (VS_OUTPUT)0;	//0�ŏ�����.

	//mul(A,B) : A��B�̊|���Z.
	output.Pos = mul(Pos, g_mWVP);

	//����.
	//����؂ɖ@�����|����.
	output.Normal = mul(Normal, (float3x3)g_mW);
	output.Light = g_vLightDir;
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vCamPos - PosWorld;

	output.Tex = Tex;

	return output;
}

//*************************************************
//	׽�ײ��(�����Ȃ�).
//*************************************************

//-------------------------------------------------
//	�߸�ټ����.
//-------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{

	//����.
	float3 Normal	= normalize(input.Normal);
	float3 LightDir	= normalize(input.Light);
	float3 ViewDir	= normalize(input.EyeVector);
	float4 NL		= saturate(dot(Normal, LightDir));

	//Reflect:����.
	// �ʂ̔��˒l�����߂�.
	float3 Reflect	= normalize(2 * NL * Normal - LightDir);
	float4 Specular = 2 * pow(saturate(dot(Reflect, ViewDir)), 2);

	float4 Color = g_Texture.Sample(g_samLinear, input.Tex);//�F��Ԃ�.
//	Color = Color * NL + Specular;
//	Color = (Color * NL + Specular)*g_vColor;
	Color = Color + Color * Specular;

	return Color;
}

//normalize()	: ���K������.
//saturate()	: 0�`1�ɸ���߂���(�����I��1�ɔ[�߂�).
//dot( A, B )	: A��B�̓��ς����߂�.
//pow( A, B )	: A��B���Ԃ�.

//Specular(��߷��)	: ���ːF.
//Ambient(���޴��)	: ���F.
//Diffuse(�ި̭���)	: �g�U���ːF.
//Emissive(�Я���)	: ���Ȕ����F.

