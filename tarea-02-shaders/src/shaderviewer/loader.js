/**
 SHADERVIEWER LOADER
 Carga un shader desde un archivo.

 @author Pablo Pizarro R. @ppizarror.com
 @license MIT
 */
"use strict";

/**
 * Contiene la información de los shaders disponibles
 * @global
 * @const
 * @since 0.1.6
 */
let shader_lib = {
    "julia-z2": {
        "files": {
            "frag": "shaders/julia-z2.frag",
            "vert": "shaders/julia.vert",
        },
        "julia": {
            "re": 0.279,
            "im": 0.000
        },
        "name": "Julia f(z) = z^2 + C",
    },
    "julia-z3": {
        "files": {
            "frag": "shaders/julia-z3.frag",
            "vert": "shaders/julia.vert",
        },
        "julia": {
            "re": 0.400,
            "im": 0.000
        },
        "name": "Julia f(z) = z^3 + C",
    },
    "julia-z4": {
        "files": {
            "frag": "shaders/julia-z4.frag",
            "vert": "shaders/julia.vert",
        },
        "julia": {
            "re": 0.484,
            "im": 0.000
        },
        "name": "Julia f(z) = z^4 + C",
    },
    "julia-exp": {
        "files": {
            "frag": "shaders/julia-exp.frag",
            "vert": "shaders/julia.vert",
        },
        "julia": {
            "re": -0.650,
            "im": 0.000
        },
        "name": "Julia f(z) = exp(z) + C",
    },
    "julia-expz3": {
        "files": {
            "frag": "shaders/julia-exp-z3.frag",
            "vert": "shaders/julia.vert",
        },
        "julia": {
            "re": -0.59,
            "im": 0.000
        },
        "name": "Julia f(z) = exp(z^3) + C",
    },
    "julia-zexp": {
        "files": {
            "frag": "shaders/julia-z-exp.frag",
            "vert": "shaders/julia.vert",
        },
        "julia": {
            "re": 0.040,
            "im": 0.000
        },
        "name": "Julia f(z) = z*exp(z) + C",
    },
    "julia-z2-z-lnz": {
        "files": {
            "frag": "shaders/julia-z2-z-ln-z.frag",
            "vert": "shaders/julia.vert",
        },
        "julia": {
            "re": 0.268,
            "im": 0.060
        },
        "name": "Julia f(z) = (z^2+z)/ln(z) + C",
    },
    "mandelbrot": {
        "files": {
            "frag": "shaders/mandelbrot.frag",
            "vert": "shaders/mandelbrot.vert",
        },
        "name": "Mandelbrot",
    }
};

/**
 * Carga un shader desde los archivos y luego llama a callback.
 *
 * @param {string} vertex - Archivo vertex shader
 * @param {string} fragment - Archivo fragment shader
 * @param {function=} callback - Función a llamar para pasar [vertex,shader]
 * @since 0.1.3
 */
function load_shader(vertex, fragment, callback) {

    /**
     * Almacena las respuestas de cada caso
     * @type {{fragment: string, vertex: string}}
     */
    let data = {
        fragment: '',
        vertex: '',
    };

    // noinspection JSUnresolvedFunction
    /**
     * Se crea la consulta para el vertex shader
     */
    let $loadVertex = $.ajax({
        crossOrigin: cfg_ajax_cors,
        timeout: cfg_href_ajax_timeout,
        type: 'get',
        url: vertex,
    });

    /**
     * Respuesta correcta desde el servidor
     */
    $loadVertex.done(function (response) {
        data.vertex = response;

        // noinspection JSUnresolvedFunction
        /**
         * Se crea la consulta para el fragment shader
         */
        let $loadFragment = $.ajax({
            crossOrigin: cfg_ajax_cors,
            timeout: cfg_href_ajax_timeout,
            type: 'get',
            url: fragment,
        });

        /**
         * Respuesta correcta desde el servidor
         */
        $loadFragment.done(function (response) {
            data.fragment = response;
            if (isFunction(callback)) {
                callback(data);
            }
        });

        /**
         * Respuesta fallida desde el servidor
         */
        $loadFragment.fail(function () {
            app_dialog.error(lang.error_loading_shader, lang.error_loading_shader_info.format(fragment));
            loadingHandler(false);
        });

    });

    /**
     * Respuesta fallida desde el servidor
     */
    $loadVertex.fail(function () {
        app_dialog.error(lang.error_loading_shader, lang.error_loading_shader_info.format(vertex));
        loadingHandler(false);
    });

}