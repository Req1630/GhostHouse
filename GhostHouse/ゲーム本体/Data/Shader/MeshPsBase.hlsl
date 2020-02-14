//��۰��ٕϐ�.

//ø����� ڼ޽� t(n).
Texture2D		g_Texture	: register(t0);
//����ׂ� ڼ޽� s(n).
SamplerState	g_samLinear	: register(s0);

//�ݽ����ޯ̧.
cbuffer global
{
	matrix	g_mW;	//ܰ��ލs��.
	matrix	g_mWVP;	//ܰ���,�ޭ�,��ۼު���݂̍����s��.
	float4	g_vLightDir;	//ײĂ̕����޸��.
	float4	g_vDiffuse;		//�ި̭��ސF(�g�U���ːF).
	float4	g_vSpecular;	//��߷�אF(���ʔ��ːF).
	float4	g_vCamPos;		//��׈ʒu(���_�ʒu).
};

//���_����ނ̏o�����Ұ�.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float4	PosLocal	: POSITION;
	float4	Normal		: NORMAL;
	float2	Tex			: TEXCOORD;
};

//-------------------------------------------------
//	���_(�ްï��)�����.
//-------------------------------------------------
VS_OUTPUT VS_Main(float4 Pos : POSITION,
	float4 Norm : NORMAL,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, g_mWVP);
	output.PosLocal = Pos;
	output.Normal = Norm;
	output.Tex = Tex;

	return output;
}

//*************************************************
//	׽�ײ��(���ؑ��Őݒ肷�鍀�ځB�����ł͐G��Ȃ�).
//*************************************************

//-------------------------------------------------
//	�߸�ټ����.
//-------------------------------------------------
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	float3 PosWorld = mul(input.PosLocal, g_mW);
	float3 EyeVector = g_vCamPos - PosWorld;

	float3 Normal = normalize(mul(input.Normal, (float3x3)g_mW));
	float3 LightDir = normalize(g_vLightDir);
	float3 ViewDir = normalize(EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL*Normal - LightDir);
	float4 Specular = pow(saturate(dot(Reflect, ViewDir)), 1);

	float4 Color =
		g_Texture.Sample(g_samLinear, input.Tex) / 2.0f +
		(g_vDiffuse * NL + Specular * g_vSpecular) / 2.0f;

	return Color;
}