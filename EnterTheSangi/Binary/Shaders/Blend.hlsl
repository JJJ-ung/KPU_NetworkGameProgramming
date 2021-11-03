
// µ«ª¡Ó≈∏∞Ÿ
texture		g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	texture = g_DiffuseTexture;
};

struct PS_IN
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	Out.vColor = vDiffuse;

	return Out;
}


technique	Default_Device
{
	pass Default_Rendering
	{
		AlphaTestEnable = true;
		Alpharef = 0;
		AlphaFunc = Greater;

		ZWriteEnable = false;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}

