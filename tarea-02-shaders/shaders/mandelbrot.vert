attribute float vertex_z_r;
attribute float vertex_z_i;

// varyings get passed to the fragment shader.
varying float c_r;
varying float c_i;

void main() {
	// Say what the varying values are.
	c_r = vertex_z_r;
	c_i = vertex_z_i;

	// Have to say what the position is:
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}