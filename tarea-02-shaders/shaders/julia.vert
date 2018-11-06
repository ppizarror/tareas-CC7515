/*
JULIA
VERTEX SHADER

Julia polinomial, pasa coordenadas del plano cartesiano.
Shader común para todas las instancias de los shader julia.

@author Pablo Pizarro R. @ppizarror.com
@license MIT
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