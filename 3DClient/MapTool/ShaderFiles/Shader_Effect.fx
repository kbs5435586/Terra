matrix		g_matWorld, g_matView, g_matProj;
vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightSpecular;
vector		g_vLightAmbient;
vector		g_vMtrlSpecular;
vector		g_vMtrlAmbient;
float		g_fPower;
vector		g_vCamPosition;
int			g_iPick;



float2		fDistortion1;
float2		fDistortion2;
float2		fDistortion3;
float		fDistortionScale;
float		fDistortionBias;

float		fFrameTime;
float3		vScrollSpeed;
float3		vScale;


texture		g_DiffuseTexture;
sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_FireAlpha;
sampler FireAlphaSampler = sampler_state
{
	texture = g_FireAlpha;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_Fire;
sampler FireSampler = sampler_state
{
	texture = g_Fire;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture		g_FireNoise;
sampler FireNoiseSampler = sampler_state
{
	texture = g_FireNoise;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VS_IN
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD;
};

struct VS_OUT
{
	vector	vPosition : POSITION;
	vector	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	vector	vProjPos : TEXCOORD1;

	float2 vTexCoord1	: TEXCOORD2;
	float2 vTexCoord2	: TEXCOORD3;
	float2 vTexCoord3	: TEXCOORD4;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;


	Out.vTexCoord1 = (Out.vTexUV * vScale.x);
	Out.vTexCoord1.y = Out.vTexCoord1.y + (fFrameTime * vScrollSpeed.x);

	Out.vTexCoord2 = (Out.vTexUV * vScale.y);
	Out.vTexCoord2.y = Out.vTexCoord2.y + (fFrameTime * vScrollSpeed.y);

	Out.vTexCoord3 = (Out.vTexUV * vScale.z);
	Out.vTexCoord3.y = Out.vTexCoord3.y + (fFrameTime * vScrollSpeed.z);
	return Out;
}

struct PS_IN
{
	vector	vPosition : POSITION;
	vector	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	vector	vProjPos : TEXCOORD1;

	float2 vTexCoord1	: TEXCOORD2;
	float2 vTexCoord2	: TEXCOORD3;
	float2 vTexCoord3	: TEXCOORD4;
};


struct PS_OUT
{
	vector vEffect0			: COLOR0;
	vector vEffect1			: COLOR1;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	float4	vNoise1;
	float4	vNoise2;
	float4	vNoise3;


	float4	vFinalNoise;

	float	fPertrub;

	float2	vNoiseCoord;
	float4	vFireColor;
	float4	vAlphaColor;

	vNoise1 =tex2D(FireNoiseSampler, In.vTexCoord1);
	vNoise2 =tex2D(FireNoiseSampler, In.vTexCoord2);
	vNoise3 =tex2D(FireNoiseSampler, In.vTexCoord3);

	vNoise1.xy = vNoise1.xy * fDistortion1.xy;
	vNoise2.xy = vNoise2.xy * fDistortion2.xy;
	vNoise3.xy = vNoise3.xy * fDistortion3.xy;

	vFinalNoise = vNoise1 + vNoise2 + vNoise3;
	fPertrub = ((1.0f - In.vTexUV.y) * fDistortionScale) + fDistortionBias;

	vNoiseCoord.xy = (vFinalNoise.xy * fPertrub) + In.vTexUV.xy;
	vFireColor = tex2D(FireSampler, vNoiseCoord.xy);

	vAlphaColor = tex2D(FireAlphaSampler, vNoiseCoord.xy);
	vFireColor.a = vAlphaColor;


	Out.vEffect0 = tex2D(DiffuseSampler, In.vTexUV);
	Out.vEffect1 = vFireColor;


	return Out;
}


technique	DefaultDevice
{
	pass NoneAlpha
	{
		AlphaBlendEnable = false;
		SrcBlend = one;
		DestBlend = one;

		CullMode = none;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	//pass CrossBuffer
	//{
	//	AlphaTestEnable = true;
	//	AlphaFunc = Greater;
	//	AlphaRef = 0x1f;

	//	CullMode = none;

	//	VertexShader = compile vs_3_0 VS_MAIN_PHONG();
	//	PixelShader = compile ps_3_0 PS_MAIN_PHONG_CROSS();
	//}
}
