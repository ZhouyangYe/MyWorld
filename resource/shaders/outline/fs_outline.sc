$input v_texcoord0

#include "../common/common.sh"

void main() {
	if (v_texcoord0.x > 0.01f && v_texcoord0.y > 0.01f && v_texcoord0.x < 0.99f && v_texcoord0.y < 0.99f)
		discard;

	gl_FragDepth = gl_FragCoord.z - 0.0001f;
	gl_FragColor = vec4(0, 0, 0, 1);
}
