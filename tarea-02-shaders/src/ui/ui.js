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

        /**
         * Inicia el panel de carga
         */
        loadingHandler(true);

        /**
         * Guarda las referencias
         */
        self._menuContainer = $(leftPanel);

        /**
         * Inicia los elementos
         */
        this._drawCanvas(rightPanel);
        this._drawMenu();

        /**
         * Dibuja el shader elegido
         */
        setTimeout(this._shaderViewer.loadSelectedShader, 1000);

    };

    /**
     * Dibuja el menú (panel izquierdo)
     * @function
     * @private
     * @since 0.1.0
     */
    this._drawMenu = function () {

        /**
         * Crea título
         */
        let c;
        c = this._drawMenuInput();
        c.title.text(lang.menu_title);
        c.title.addClass('viewer-menu-main-title');
        // noinspection HtmlUnknownTarget
        c.content.append('<div class="viewer-menu-title-image"><img src="src/res/android-chrome-512x512.png" alt="" /></div>');

        /**
         * Dibuja el selector de shaders
         */
        c = this._drawMenuInput();
        let $selectorid = generateID();

        c.title.text(lang.menu_shader);
        c.content.append('<select id="{0}" class="common-selector noselect"></select>'.format($selectorid));
        let $shaders = Object.keys(shader_lib);
        let $selector = $('#' + $selectorid);
        for (let i = 0; i < $shaders.length; i += 1) {
            $selector.append('<option value="{0}">{1}</option>'.format($shaders[i], shader_lib[$shaders[i]].name));
        }

        // Define el selector
        self._shaderViewer.setShaderSelector($selector);

        // El cambio genera un redibujo del shader actual
        $selector.on('change', function () {
            self._shaderViewer.loadSelectedShader();
        });

    };

    /**
     * Crea un input en el menú lateral
     * @function
     * @private
     * @since 0.1.6
     */
    this._drawMenuInput = function () {
        let $titleid = generateID();
        let $contentid = generateID();
        self._menuContainer.append('<div class="viewer-menu-input-container"><div class="viewer-menu-input-title" id="{0}"></div><div class="viewer-menu-input-content" id="{1}"></div></div>'.format($titleid, $contentid));
        return {
            content: $('#' + $contentid),
            title: $('#' + $titleid),
        };
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