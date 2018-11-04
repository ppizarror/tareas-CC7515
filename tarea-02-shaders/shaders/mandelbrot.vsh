/* The fragment shader for the Mandelbrot set plotter */

// Varyings get declared in both vertex and fragment shaders:
varying float c_r;
varying float c_i;

// uniforms are defined in the JavaScript:
uniform int max_iterations;

// Defining the lower and upper ends of the colour scale:
#define r_min 0.0
#define r_max 1.0

#define g_min 0.0
#define g_max 1.0

#define b_min 0.35
#define b_max 0.0

void main() {
  float r;
  float g;
  float b;
  float t;
  float w_r;
  float w_i;
  float u;
  float v;
  
  // Initial value in the w --> w^2 + c:
  w_r = 0.0;
  w_i = 0.0;
  
  // Colour will be black if it stays converged:
  r = 0.0;
  g = 0.0;
  b = 0.0;
  
  // The looping variable can't be compared to a non-constant,
  // so to allow the user to change the maximum number of
  // iterations, I break manually.
  for (int i = 0; i < 65536; i++) {
    // The fractal calculation:
    u = w_r;
    v = w_i;
    
    w_r = u*u - v*v + c_r;
    w_i = 2.0*u*v + c_i;
    
    if (w_r*w_r + w_i*w_i > 4.0) {
      // Diverged: make a pretty colour.
      t = log(float(i + 1)) / log(float(max_iterations + 1));
      
      r = t*r_max + (1.0 - t)*r_min;
      g = t*g_max + (1.0 - t)*g_min;
      b = t*b_max + (1.0 - t)*b_min;
      
      break;
    }
    
    if (i >= max_iterations) {
      break;
    }
  }
  
  // Set the colour:
  gl_FragColor = vec4(r, g, b, 1.0);
}