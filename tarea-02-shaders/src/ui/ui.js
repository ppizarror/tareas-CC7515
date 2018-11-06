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

        /**
         * Dibuja recuadro información del plano
         */
        c = this._drawMenuInput();
        c.title.text(lang.shader_complex_title);
        let $complexinfo = generateID();
        c.content.append('<div class="viewer-menu-complex-info" id="{0}"></div>'.format($complexinfo));
        self._shaderViewer.setComplexInfoWindow($('#' + $complexinfo));

        /**
         * Crea varyings
         */
        c = this._drawMenuInput();
        c.title.text(lang.shader_data_color);
        let $slider;

        let $r_min = generateID();
        c.content.append('<p><b>R min</b> <input type="text" value="" data-slider-min="0" data-slider-max="1" data-slider-step="0.01" data-slider-value="{1}" id="{0}"/></p>'.format($r_min, self._shaderViewer.getShaderColor('r_min')));
        $slider = $('#' + $r_min);
        $slider.slider({});
        $slider.on('change', function () {
            // noinspection JSCheckFunctionSignatures
            self._shaderViewer.updateShaderColor('r_min', parseFloat($('#' + $r_min).val()));
        });

        let $r_max = generateID();
        c.content.append('<p><b>R max</b> <input type="text" value="" data-slider-min="0" data-slider-max="1" data-slider-step="0.01" data-slider-value="{1}" id="{0}"/></p>'.format($r_max, self._shaderViewer.getShaderColor('r_max')));
        $slider = $('#' + $r_max);
        $slider.slider({});
        $slider.on('change', function () {
            // noinspection JSCheckFunctionSignatures
            self._shaderViewer.updateShaderColor('r_max', parseFloat($('#' + $r_max).val()));
        });

        let $g_min = generateID();
        c.content.append('<p><b>G min</b> <input type="text" value="" data-slider-min="0" data-slider-max="1" data-slider-step="0.01" data-slider-value="{1}" id="{0}"/></p>'.format($g_min, self._shaderViewer.getShaderColor('g_min')));
        $slider = $('#' + $g_min);
        $slider.slider({});
        $slider.on('change', function () {
            // noinspection JSCheckFunctionSignatures
            self._shaderViewer.updateShaderColor('g_min', parseFloat($('#' + $g_min).val()));
        });

        let $g_max = generateID();
        c.content.append('<p><b>G max</b> <input type="text" value="" data-slider-min="0" data-slider-max="1" data-slider-step="0.01" data-slider-value="{1}" id="{0}"/></p>'.format($g_max, self._shaderViewer.getShaderColor('g_max')));
        $slider = $('#' + $g_max);
        $slider.slider({});
        $slider.on('change', function () {
            // noinspection JSCheckFunctionSignatures
            self._shaderViewer.updateShaderColor('g_max', parseFloat($('#' + $g_max).val()));
        });

        let $b_min = generateID();
        c.content.append('<p><b>B min</b> <input type="text" value="" data-slider-min="0" data-slider-max="1" data-slider-step="0.01" data-slider-value="{1}" id="{0}"/></p>'.format($b_min, self._shaderViewer.getShaderColor('b_min')));
        $slider = $('#' + $b_min);
        $slider.slider({});
        $slider.on('change', function () {
            // noinspection JSCheckFunctionSignatures
            self._shaderViewer.updateShaderColor('b_min', parseFloat($('#' + $b_min).val()));
        });

        let $b_max = generateID();
        c.content.append('<p><b>B max</b> <input type="text" value="" data-slider-min="0" data-slider-max="1" data-slider-step="0.01" data-slider-value="{1}" id="{0}"/></p>'.format($b_max, self._shaderViewer.getShaderColor('b_max')));
        $slider = $('#' + $b_max);
        $slider.slider({});
        $slider.on('change', function () {
            // noinspection JSCheckFunctionSignatures
            self._shaderViewer.updateShaderColor('b_max', parseFloat($('#' + $b_max).val()));
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