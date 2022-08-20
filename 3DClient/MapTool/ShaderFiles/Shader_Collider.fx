
matrix	g_matWorld, g_matView, g_matProj;
vector	g_vColor;

vector VS_MAIN(float3 vPosition : POSITION) : POSITION
{
	vector		vOut = (vector)0;

matrix		matWV, matWVP;

matWV = mul(g_matWorld, g_matView);
matWVP = mul(matWV, g_matProj);

vOut = mul(vector(vPosition, 1.f), matWVP);

return vOut;
}

vector PS_MAIN(vector vPosition : POSITION) : COLOR
{
	vector		vColor = (vector)0;

vColor = g_vColor;

return vColor;
}

technique	DefaultDevice
{
	pass Render_Collider
	{
		Fillmode = Wireframe;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}


