varying float c_r;
varying float c_i;

uniform int max_iterations;

// Define el rango de colores
#define r_min 0.0
#define r_max 0.5

#define g_min 0.5
#define g_max 0.0

#define b_min 1.0
#define b_max 0.35

// Inicio del shader
void main() {
	float r;
	float g;
	float b;
	float t;
	float w_r;
	float w_i;
	float u;
	float v;

	w_r = 0.0;
	w_i = 0.0;

	// Si converge es negro
	r = 0.0;
	g = 0.0;
	b = 0.0;

	for (int i = 0; i < 65536; i++) {
		u = w_r;
		v = w_i;

		w_r = u*u - v*v + c_r;
		w_i = 2.0*u*v + c_i;

		if (w_r*w_r + w_i*w_i > 4.0) {
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

	gl_FragColor = vec4(r, g, b, 1.0);
}