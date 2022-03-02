$input v_texcoord0, v_color0

#include "../common/common.sh"

SAMPLER2DARRAY(s_texColor, 0);

void main() {
	gl_FragColor = vec4(texture2DArray(s_texColor, v_texcoord0).rgb, v_color0.w);
}
