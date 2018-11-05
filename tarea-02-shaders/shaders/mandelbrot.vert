attribute float vertex_z_r;
attribute float vertex_z_i;

varying float c_r;
varying float c_i;

void main() {
	c_r = vertex_z_r;
	c_i = vertex_z_i;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}