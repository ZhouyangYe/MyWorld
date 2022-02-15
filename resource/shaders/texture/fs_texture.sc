$input v_texcoord0

#include "common/common.sh"

SAMPLER2D(s_texColor, 0);

void main() {
	gl_FragColor = vec4(texture2D(s_texColor, v_texcoord0).rgb, 1.0);
}
