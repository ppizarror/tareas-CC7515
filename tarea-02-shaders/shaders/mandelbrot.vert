/*
MANDELBROT
VERTEX SHADER

Ejecuta mandelbrot, C = c_r + i*c_i se pasa por cada (x,y) del plano complejo.
@since 0.1.0
*/

// (x,y) de cada valor del plano
attribute float vertex_z_r;
attribute float vertex_z_i;

varying float c_r;
varying float c_i;

void main() {

    // El valor complejo corresponde a (x,y)
	c_r = vertex_z_r;
	c_i = vertex_z_i;

	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);

}