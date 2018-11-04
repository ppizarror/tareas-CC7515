/* The vertex shader for the Mandelbrot set plotter */

// The real and imaginary components of the
// complex number this vertex represents; must
// be set in the JavaScript.
attribute float vertex_z_r;
attribute float vertex_z_i;

// varyings get passed to the fragment shader; these
// will be the same as the attributes but the names
// needs to be different:
varying float c_r;
varying float c_i;

void main() {
  // Say what the varying values are.
  c_r = vertex_z_r;
  c_i = vertex_z_i;
  
  // Have to say what the position is:
  gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}