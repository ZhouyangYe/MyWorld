$input v_color0

#include "../common/common.sh"

void main() {
	if (gl_FragCoord.z < 0.96f) {
		gl_FragDepth = gl_FragCoord.z - 0.001f;
	} else {
		gl_FragDepth = gl_FragCoord.z - 0.0001f;
	}
	gl_FragColor = v_color0;
}
