/*
JULIA-POL2
VERTEX SHADER

Julia polinomial orden 2. Cumple ecuación z_n = z_n-1^2 + C.
@since 0.3.0
*/

attribute float vertex_z_r;
attribute float vertex_z_i;

varying float c_r;
varying float c_i;

void main() {
	c_r = vertex_z_r;
	c_i = vertex_z_i;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}