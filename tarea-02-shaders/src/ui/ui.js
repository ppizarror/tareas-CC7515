/**
 UI
 Crea la interfaz gráfica.

 @author Pablo Pizarro R. @ppizarror.com
 @license MIT
 */
"use strict";

/**
 * Inicia la interfaz gráfica.
 *
 * @class
 * @constructor
 * @since 0.1.0
 */
function BuildUI() {

    /**
     * Contenedor del menú
     * @type {null | JQuery<HTMLElement> | HTMLElement}
     * @private
     * @since 0.1.0
     */
    this._menuContainer = null;

    /**
     * Puntero a objeto shaderViewer
     * @type {null | ShaderViewer}
     * @private
     */
    this._shaderViewer = null;

    /**
     * Puntero al objeto
     * @type {BuildUI}
     * @since 0.1.0
     */
    let self = this;

    /**
     * Inicia el visualizador, toma por parámetro el nombre de los paneles izquierdo
     * (menú) y derecho (canvas).
     * @function
     * @param {string} leftPanel - ID del panel izquierdo
     * @param {string} rightPanel - ID del panel derecho
     * @since 0.1.0
     */
    this.init = function (leftPanel, rightPanel) {

        // Guarda las referencias
        self._menuContainer = $(leftPanel);

        // Inicia los elementos
        this._drawMenu();
        this._drawCanvas(rightPanel);

    };

    /**
     * Dibuja el menú (panel izquierdo)
     * @function
     * @private
     * @since 0.1.0
     */
    this._drawMenu = function () {
        self._menuContainer.append('<input type="text" />');
    };

    /**
     * Dibuja el canvas e inicia el viewer
     * @param {string} container - Contenedor del visualizador
     * @private
     * @since 0.1.0
     */
    this._drawCanvas = function (container) {
        self._shaderViewer = new ShaderViewer();
        this._shaderViewer.init(container);
    }

}