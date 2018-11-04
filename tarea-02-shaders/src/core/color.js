/**
 COLOR
 Funciones utilitarias asociadas a la creación y manejo de colores.

 @author Pablo Pizarro R. @ppizarror.com
 @license MIT
 */
"use strict";

/**
 * Retorna un color aleatorio.
 *
 * @function
 * @returns {string} - Código de color
 * @since 0.1.2
 */
function getRandomColor() {
    let letters = '0123456789ABCDEF';
    let color = '#';
    for (let i = 0; i < 6; i += 1) {
        color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
}

/**
 * Modifica un color añadiendo blanco.
 *
 * @function
 * @param {string} color - Color
 * @param {number} percent - Porcentaje de b/n
 * @returns {string} - Color corregido por el shading
 * @since 0.1.2
 */
function shadeColor2(color, percent) {
    let f = parseInt(color.slice(1), 16);
    let t = percent < 0 ? 0 : 255;
    let p = percent < 0 ? percent * -1 : percent;
    /* eslint no-bitwise: "off"*/
    let R = f >> 16;
    let G = f >> 8 & 0x00FF;
    let B = f & 0x0000FF;
    /* eslint no-mixed-operators: "off"*/
    return '#' + (0x1000000 + (Math.round((t - R) * p) + R) * 0x10000 + (Math.round((t - G) * p) + G) * 0x100 + (Math.round((t - B) * p) + B)).toString(16).slice(1);
}

/**
 * Retorna true/false si el string es un color.
 *
 * @function
 * @param {object} color - Color
 * @returns boolean - String es un color
 * @since 0.1.2
 */
function isColor(color) {
    return typeof color === 'string' && color.length === 7 && color[0] === '#';
}

/**
 * Convierte un color en hex a un color tipo rgb/rgba dependiendo de la opacidad.
 *
 * @function
 * @param {string} hex - Color en hexadecimal
 * @param {number=} opacity - Opacidad
 * @returns {string}
 * @since 0.1.2
 */
function convertHexColorRGBA(hex, opacity) {
    hex = hex.replace('#', '');
    let r = parseInt(hex.substring(0, 2), 16);
    let g = parseInt(hex.substring(2, 4), 16);
    let b = parseInt(hex.substring(4, 6), 16);
    if (notNullUndf(opacity) && !isNaN(opacity)) { // Se definió la opacidad
        return 'rgba(' + r + ',' + g + ',' + b + ',' + opacity + ')';
    }
    return 'rgb(' + r + ',' + g + ',' + b + ')';
}

/**
 * Aplica transparencia a un color.
 *
 * @function
 * @param {string} color - Color en RGB
 * @param {number=} opacity - Opacidad del color
 * @returns {string} - Color con transparencia
 * @since 0.1.2
 */
function transparentizeColor(color, opacity) {
    let alpha = opacity === undefined ? 0.5 : 1 - opacity;
    let $color = new Color(color).alpha(alpha);
    return $color.rgbString();
}