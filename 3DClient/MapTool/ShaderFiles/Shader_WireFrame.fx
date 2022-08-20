
matrix		g_matWorld, g_matView, g_matProj;

int			g_iPick;

struct VS_IN
{
	float3	vPosition : POSITION;
};

struct VS_OUT
{
	vector	vPosition : POSITION;
};


VS_OUT VS_MAIN_GROUD(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	return Out;
}


struct PS_IN
{
	vector	vPosition : POSITION;
};


struct PS_OUT
{
	vector	vColor : COLOR;
};

PS_OUT PS_MAIN_GROUD(VS_OUT In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (!g_iPick)
	{
		Out.vColor = vector(0.f, 1.f, 0.f, 1.f);
	}
	else
	{
		Out.vColor = vector(1.f, 0.f, 0.f, 1.f);
	}

	return Out;
}


technique	DefaultDevice
{
	pass Groud // 정점단에서 빛 연산.
	{
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 0x0f;

		CullMode = none;
		Fillmode = Wireframe;


		VertexShader = compile vs_3_0 VS_MAIN_GROUD();
		PixelShader = compile ps_3_0 PS_MAIN_GROUD();
	}

}
