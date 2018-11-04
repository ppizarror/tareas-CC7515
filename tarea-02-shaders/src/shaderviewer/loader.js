/**
 SHADERVIEWER LOADER
 Carga un shader desde un archivo.

 @author Pablo Pizarro R. @ppizarror.com
 @license MIT
 */
"use strict";

/**
 * Carga un shader desde los archivos y luego llama a callback.
 *
 * @param {string} vertex - Archivo vertex shader.
 * @param {string} fragment - Archivo fragment shader.
 * @param {function=} callback - Función a llamar para pasar [vertex,shader].
 */
function load_shader(vertex, fragment, callback) {

    // noinspection JSUnresolvedFunction
    /**
     * Se crea la consulta
     */
    let $getMessage = $.ajax({
        crossOrigin: cfg_ajax_cors,
        timeout: cfg_href_ajax_timeout,
        type: 'get',
        url: vertex,
    });

    /**
     * Respuesta correcta desde el servidor
     */
    $getMessage.done(function (response) {
        console.log(response);
    });

    // noinspection JSCheckFunctionSignatures
    /**
     * Respuesta fallida desde el servidor
     */
    $getMessage.fail(function (response, textStatus, jqXHR) {
        app_dialog.error('sad', 'uwu');
    });

}