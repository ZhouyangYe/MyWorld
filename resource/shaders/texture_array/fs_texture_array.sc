$input v_texcoord0

#include "../common/common.sh"

SAMPLER2DARRAY(s_texColor, 0);

void main() {
	if (v_texcoord0.z < 0)
	{
		if (
			(v_texcoord0.x <= 0.01 && v_texcoord0.y <= 0.1) ||
			(v_texcoord0.y <= 0.01 && v_texcoord0.x <= 0.1) ||
			(v_texcoord0.x >= 0.99 && v_texcoord0.y <= 0.1) ||
			(v_texcoord0.y <= 0.01 && v_texcoord0.x >= 0.9) ||
			(v_texcoord0.x >= 0.99 && v_texcoord0.y >= 0.9) ||
			(v_texcoord0.y >= 0.99 && v_texcoord0.x >= 0.9) ||
			(v_texcoord0.x <= 0.01 && v_texcoord0.y >= 0.9) ||
			(v_texcoord0.y >= 0.99 && v_texcoord0.x <= 0.1)
		)
		{
			gl_FragColor = vec4(0.8, 0.8, 0.8, 1.0);
		}
		else
		{
			discard;
		}
	}
	else
	{
		vec4 texColor = texture2DArray(s_texColor, v_texcoord0);
		if (texColor.a < 0.1)
			discard;
		gl_FragColor = vec4(texColor.rgb, 1.0);
	}
}
