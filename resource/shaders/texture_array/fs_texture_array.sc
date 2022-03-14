$input v_texcoord0

#include "../common/common.sh"

SAMPLER2DARRAY(s_texColor, 0);

void main() {
	if (v_texcoord0.x % 1.0 <= 0.01 || v_texcoord0.y % 1.0 <= 0.01 || v_texcoord0.x % 1.0 >= 0.99 || v_texcoord0.y % 1.0 >= 0.99)
	{
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		gl_FragColor = vec4(texture2DArray(s_texColor, v_texcoord0).rgb, 1.0);
	}
}
