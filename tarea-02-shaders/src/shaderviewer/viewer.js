/**
 SHADERVIEWER VIEWER
 Administra el visualizador con Three.js.

 @author Pablo Pizarro R. @ppizarror.com
 @license MIT
 */
"use strict";

/**
 * Crea el visualizador de shaders.
 *
 * @class
 * @constructor
 * @since 0.1.0
 */
function ShaderViewer() {

    /**
     * ------------------------------------------------------------------------
     * Variables del plugin, relacionadas con el DOM
     * ------------------------------------------------------------------------
     */

    /**
     * ID del canvas
     * @type {string}
     * @protected
     */
    this.id = generateID();

    /**
     * DIV que contiene el canvas
     * @type {JQuery | jQuery | HTMLElement | null}
     * @protected
     */
    this.canvasParent = null;


    /**
     * Puntero al objeto
     * @type {ShaderViewer}
     */
    let self = this;


    /**
     * ------------------------------------------------------------------------
     * Eventos
     * ------------------------------------------------------------------------
     */

    // noinspection JSUnusedGlobalSymbols
    /**
     * Indica si se mantiene un botón presionado
     * @type {boolean}
     * @private
     */
    this._hasKeyPressed = false;

    // noinspection JSUnusedGlobalSymbols
    /**
     * Indica si el mouse está sobre el canvas
     * @type {boolean}
     * @private
     */
    this._hasMouseOver = false;

    /**
     * Indica si el mouse se mantiene presionado
     * @type {boolean}
     * @private
     */
    this._hasMousePressed = false;

    // noinspection JSUnusedGlobalSymbols
    /**
     * ID del evento de mousemove en la ventana
     * @type {string}
     * @private
     */
    this._windowMouseMoveEvent = '';

    /**
     * Contiene identificadores de los eventos
     * @private
     */
    this._eventID = {
        contextmenu: 'contextmenu.canvas',
        keydown: 'keydown.canvas',
        keyup: 'keyup.canvas',
        mousedown: 'mousedown.canvas',
        mouseout: 'mouseout.canvas',
        mouseover: 'mouseover.canvas',
        mouseup: 'mouseup.canvas',
        mousewheel: 'mousewheel.canvas',
        wheel: 'wheel.canvas',
    };


    /**
     * ------------------------------------------------------------------------
     * Objetos de Three.js
     * ------------------------------------------------------------------------
     */

    /**
     * Indica que la animación está activa
     * @type {boolean}
     * @private
     */
    this._animateThread = false;

    /**
     * Three.js helpers
     * @protected
     */
    this.threejs_helpers = {

        /**
         * Helpers, estado
         */
        axis: false,                     // Muestra los ejes en el plano
        cameratarget: false,             // Muestra el objetivo de la cámara
        fpsmeter: false,                 // Indicador de FPS
        grid: true,                      // Muestra grilla en plano
        gui: true,                       // Muestra una gui por defecto (modificable)
        light: false,                    // Muestra el objeto de la luz
        lighthelper: false,              // Muestra helper de la luz
        planes: false,                   // Dibuja los planos
        shadowlight: false,              // Muestra ubicación de la sombra
        worldlimits: false,              // Límites del mundo

        /**
         * Parámetros de los helpers
         */
        axissize: 0.40,                 // Tamaño de los ejes en comparación al tamaño del mundo (menor)
        cameratargetcolor: 0X0000FF,    // Color del target
        cameratargetsize: 0.02,         // Tamaño del punto del objetivo de la cámara (% tamaño mundo)
        griddist: 0.03,                 // % Cubierto por líneas
        guistartclosed: true,           // La gui si se autoinicia aparece cerrada
        guicloseafterpopup: false,      // La gui se cierra al abrir un popup
        lightsize: 0.05,                // Tamaño de la luz en comparación al tamaño del mundo (menor)
        planecolorx: 0X0000FF,          // Color plano x
        planecolory: 0XFF0000,          // Color plano y
        planecolorz: 0X00FF00,          // Color plano z
        planeopacity: 0.5,              // Opacidad del plano
        walkcameracolor: 0xFF00FF,      // Color del walkingcamera
        walkcamerasize: 0.02,           // Tamaño del punto de la posición del walkcamera (% tamaño mundo)
        worldlimitscolor: 0X444444,     // Colores límite del mundo

    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * ID de la GUI
     * @type {object | string}
     * @protected
     */
    this.guiID = 'viewer-gui';

    /**
     * Contiene funciones de actualización de los helpers, se extiende en @drawHelpers
     * @type {Array}
     * @private
     */
    this._helpersUpdate = [];

    /**
     * Instancias de cada helper, son únicos, se eliminan o añaden en @drawHelpers
     * @private
     */
    this._helperInstances = {
        axis: null,
        cameratarget: null,
        fpsmeter: null,
        grid: null,
        light: null,
        lighthelper: null,
        planes: null,
        shadowlight: null,
        walkcamerapos: null,
        worldlimits: null,
    };

    /**
     * Nombres globales, usados para identificar determinados elementos preestablecidos
     * @protected
     */
    this.globals = {
        contour: '__CONTOUR',
        helper: '__HELPER',
        plane: '__PLANE',
    };

    /**
     * Propiedades de los objetos del mundo
     * @protected
     */
    this.objects_props = {

        /**
         * Plano de la escena
         */
        plane: {
            color: 0x222222,        // Color del plano
            dithering: false,       // Aplica dithering al material
            obj: null,              // Almacena el objeto
        },

        /**
         * Luz ambiental
         */
        ambientlight: {
            color: 0X202020,
            intensity: 0.500,
        },

        /**
         * Luz puntual
         */
        light: {
            angle: 1.600,
            color: 0XFFFFFF,
            decay: 1.600,
            distance: 0.793,
            intensity: 1.500,
            penumbra: 0.580,
            planeshadow: false,
            pos: {  // Porcentaje del tamaño de cada eje
                x: 0.000,
                y: 0.000,
                z: 1.000,
            },
        },

        /**
         * Niebla
         */
        fog: {
            color: 0x131313,                    // Color de la niebla
            density: 0.00024,                   // Densidad, cuán rápido crece
            enabled: true,                      // Indica si está activa o no la niebla
        },

        /**
         * Cámara
         */
        camera: {
            angle: 56,                          // Ángulo de la cámara (FOV)
            autorotate: false,                  // Rotar automáticamente en torno al objetivo
            far: 9.000,                         // Plano lejano de la cámara (% diagonal larga)
            light: {                            // Luz pegada a la cámara
                color: 0X181818,
                decay: 1.500,
                distance: 0.483,
                intensity: 0.600,
            },
            maxdistance: 2.500,                 // Distancia máxima (% diagonal larga)
            maxpolarangle: 0.904,               // Máximo ángulo que puede alcanzar la cámara (Por pi/2)
            near: 0.100,                        // Plano cercano de la cámara
            nopan: true,                        // Activa/desactiva el PAN del mouse
            posx: 0.000,                        // Posición inicial en x (% dimensión del mundo)
            posy: -1.600,                       // Posición inicial en y (% dimensión del mundo)
            posz: 1.500,                        // Posición inicial en z
            rotationx: -1.600,                  // Rotación inicial con respecto al eje x (Por pi/2)
            rotationy: -1.600,                  // Rotación inicial con respecto al eje y (Por pi/2)
            rotationz: -0.600,                  // Rotación inicial con respecto al eje z (Por pi/2)
            target: {                           // Target de la cámara, posición inicial con respecto a la dimensión
                x: 0.000,
                y: 0.000,
                z: 0.000,
            },
            targetMoveCamera: true,             // Al mover el target también se mueve la cámara
            targetMoveCameraFlipByPos: true,    // Invierte el sentido según la posición de la cámara
            targetSpeed: {                      // Velocidad de cambio del target como % de cada eje
                angular: 0.05,                  // Velocidad angular [rad]
                x: 0.010,
                y: 0.010,
                z: 0.005,
            },
            zoom: 1.000,                        // Factor de zoom
        },

    };

    /**
     * Límites del mundo, modificar
     */
    this.worldsize = {
        x: 1,
        y: 1,
        z: 1,
    };

    /**
     * Lista con meshes colisionables
     * @type {Array}
     * @private
     */
    this._collaidableMeshes = [];


    /**
     * ------------------------------------------------------------------------
     * Métodos del visualizador
     * ------------------------------------------------------------------------
     */

    /**
     * Inicia el visualizador
     * @function
     * @param {string} parentElement - Contenedor del visualizador
     */
    this.init = function (parentElement) {
        self.id = parentElement;
        self.canvasParent = $(parentElement);
        self._initThree();
        self._initWorldObjects();
        self._initEvents();
        self._animateFrame();
        self.initShaderObject();
    };

    /**
     * Reajusta el canvas al cambiar el tamaño.
     *
     * @function
     * @protected
     * @param {boolean} type - Indica tipo de carga, si es true se añade evento, si es false se borra
     */
    this.threeResize = function (type) {

        /**
         * Nombre del evento
         * @type {string}
         */
        let $ev = 'resize.shaderviewer' + this.id;

        /**
         * Activa el evento
         */
        if (type) {
            let $f = function (e) {

                /**
                 * Previene otros cambios, útil por ThreeControls
                 */
                if (notNullUndf(e)) e.preventDefault();

                /**
                 * Se obtiene el ancho y el alto del DIV
                 */
                let $w = Math.ceil(getElementWidth(self.maindiv));
                let $h = Math.ceil(getElementHeight(self.maindiv));

                /**
                 * Actualiza el aspecto del canvas
                 */
                self.objects_props.camera.aspect = $w / $h;

                /**
                 * Actualiza Three.js
                 */
                self._three_camera.aspect = $w / $h;
                self._three_camera.updateProjectionMatrix();
                self._renderer.setSize($w, $h); // Actualiza el render
                self._renderer.setPixelRatio(window.devicePixelRatio);

                /**
                 * Redibuja
                 */
                self._animateFrame();

            };
            app_dom.window.on($ev, $f);
            $f();
        }

        /**
         * Desactiva el evento
         */
        else {
            app_dom.window.off($ev);
        }

    };

    /**
     * Actualiza controles y renderiza.
     *
     * @function
     * @private
     */
    this._animateFrame = function () {

        /**
         * Actualiza los controles
         */
        this._controls.update();

        /**
         * Renderiza un cuadro
         */
        this._render();

    };

    /**
     * Inicia Three.js.
     *
     * @function
     * @private
     */
    this._initThree = function () {

        /**
         * --------------------------------------------------------------------
         * Calcula límites del mapa
         * --------------------------------------------------------------------
         */
        this.worldsize.diagl = Math.sqrt(Math.pow(2 * this.worldsize.x, 2) + Math.pow(2 * this.worldsize.y, 2) + Math.pow(this.worldsize.z, 2));
        this.worldsize.diagx = Math.sqrt(Math.pow(2 * this.worldsize.x, 2) + Math.pow(this.worldsize.z, 2));
        this.worldsize.diagy = Math.sqrt(Math.pow(2 * this.worldsize.y, 2) + Math.pow(this.worldsize.z, 2));

        /**
         * --------------------------------------------------------------------
         * Setea cámara
         * --------------------------------------------------------------------
         */

        // Restricciones de la cámara
        this.objects_props.camera.far *= this.worldsize.diagl;
        this.objects_props.camera.maxdistance *= this.worldsize.diagl;
        this.objects_props.camera.maxpolarangle *= Math.PI / 2;

        // Posición inicial de la cámara
        this.objects_props.camera.posx *= this.worldsize.x;
        this.objects_props.camera.posy *= this.worldsize.y;
        this.objects_props.camera.posz *= this.worldsize.z;

        // Rotaciones iniciales
        this.objects_props.camera.rotationx *= Math.PI / 2;
        this.objects_props.camera.rotationy *= Math.PI / 2;
        this.objects_props.camera.rotationz *= Math.PI / 2;

        // Target de la cámara
        this.objects_props.camera.target.x *= this.worldsize.x;
        this.objects_props.camera.target.y *= this.worldsize.y;
        this.objects_props.camera.target.z *= this.worldsize.z;

        // Velocidad de movimiento del target en actualización de controles
        this.objects_props.camera.targetSpeed.x *= this.worldsize.x;
        this.objects_props.camera.targetSpeed.y *= this.worldsize.y;
        this.objects_props.camera.targetSpeed.z *= this.worldsize.z;
        this.objects_props.camera.targetSpeed.xy = this._dist2(this.objects_props.camera.targetSpeed.x, this.objects_props.camera.targetSpeed.y);

        // Suma el target a la cámara para mantener distancias
        this.objects_props.camera.posx += this.objects_props.camera.target.x;
        this.objects_props.camera.posy += this.objects_props.camera.target.y;
        this.objects_props.camera.posz += this.objects_props.camera.target.z;

        // Posición inicial de la cámara
        self.objects_props.camera.initialTarget = {
            x: this.objects_props.camera.target.x,
            y: this.objects_props.camera.target.y,
            z: this.objects_props.camera.target.z
        };

        /**
         * --------------------------------------------------------------------
         * Setea propiedades de las luces
         * --------------------------------------------------------------------
         */
        this.objects_props.light.pos.x *= this.worldsize.x;
        this.objects_props.light.pos.y *= this.worldsize.y;
        this.objects_props.light.pos.z *= this.worldsize.z;
        this.objects_props.light.distance *= this.worldsize.diagl;
        this.objects_props.camera.light.distance *= this.worldsize.diagl;

        /**
         * --------------------------------------------------------------------
         * Inicia el render de Three.js
         * --------------------------------------------------------------------
         */
        this._renderer = new THREE.WebGLRenderer({

            // Activa las transparencias, obligatorio
            alpha: true,

            // Antialias, deshabilitado en dispositivo móvil para mejorar rendimiento
            antialias: true,

            // Tiene un búffer de profundidad de 16 bits
            depth: true,

            // Preferencia de WebGL, puede ser "high-performance", "low-power" ó "default"
            powerPreference: "high-performance",

            // Búffer de profundidad logarítmico, usado cuando hay mucha diferencia en la escena
            logarithmicDepthBuffer: false,

            // Los colores ya tienen incorporado las transparencias
            premultipliedAlpha: true,

            // Para capturas, si molesta deshabilitar
            preserveDrawingBuffer: false,

            // El búffer de dibujo tiene un stencil de 8 bits
            stencil: true,

        });

        /**
         * --------------------------------------------------------------------
         * Crea la escena
         * --------------------------------------------------------------------
         */
        this._scene = new THREE.Scene();
        this._scene.name = 'VIEWER-3D-SCENE';

        /**
         * --------------------------------------------------------------------
         * Setea la iluminación puntual
         * --------------------------------------------------------------------
         */
        this._light = new THREE.SpotLight();

        // Cambia las propiedades
        this._light.castShadow = true;
        this._light.color.setHex(self.objects_props.light.color);
        this._light.decay = self.objects_props.light.decay;
        this._light.distance = this.worldsize.diagl;
        this._light.intensity = self.objects_props.light.intensity;
        this._light.penumbra = self.objects_props.light.penumbra;
        this._light.angle = self.objects_props.light.angle;
        // noinspection JSSuspiciousNameCombination
        this._light.position.x = this.objects_props.light.pos.y;
        this._light.position.y = this.objects_props.light.pos.z;
        this._light.position.z = this.objects_props.light.pos.x;
        this._light.shadow.mapSize.height = 512;
        this._light.shadow.mapSize.width = 512;
        this._scene.add(this._light);

        /**
         * --------------------------------------------------------------------
         * Setea la niebla
         * --------------------------------------------------------------------
         */
        this._fog = new THREE.FogExp2(this.objects_props.fog.color, this.objects_props.fog.density);
        if (this.objects_props.fog.enabled) {
            this._scene.fog = this._fog;
        }

        /**
         * --------------------------------------------------------------------
         * Iluminación ambiental
         * --------------------------------------------------------------------
         */
        this._ambientlight = new THREE.AmbientLight();
        this._ambientlight.color.setHex(this.objects_props.ambientlight.color);
        this._ambientlight.intensity = this.objects_props.ambientlight.intensity;
        this._scene.add(this._ambientlight);

        /**
         * --------------------------------------------------------------------
         * Modifica el render para las luces
         * --------------------------------------------------------------------
         */
        this._renderer.shadowMap.enabled = true;
        this._renderer.shadowMap.type = THREE.PCFSoftShadowMap;
        this._renderer.gammaInput = true;
        this._renderer.gammaOutput = true;

        /**
         * --------------------------------------------------------------------
         * Crea la cámara
         * --------------------------------------------------------------------
         * @private
         */
        this._three_camera = new THREE.PerspectiveCamera(
            self.objects_props.camera.angle,
            self.objects_props.camera.aspect,
            self.objects_props.camera.near,
            self.objects_props.camera.far,
        );
        this._three_camera.zoom = this.objects_props.camera.zoom;

        // noinspection JSUnusedGlobalSymbols
        this._cameralight = new THREE.PointLight();
        this._cameralight.color.setHex(this.objects_props.camera.light.color);
        this._cameralight.decay = this.objects_props.camera.light.decay;
        this._cameralight.distance = this.objects_props.camera.light.distance;
        this._cameralight.intensity = this.objects_props.camera.light.intensity;
        this._three_camera.add(this._cameralight);

        /**
         * --------------------------------------------------------------------
         * Añade el render al div
         * --------------------------------------------------------------------
         * @private
         */
        this.maindiv = $(self.id);
        this.maindiv.append(this._renderer.domElement);
        this.canvasParent.attr('tabindex', '1');

        /**
         * --------------------------------------------------------------------
         * Añade la cámara al escenario
         * --------------------------------------------------------------------
         */
        this._scene.add(this._three_camera);

        /**
         * --------------------------------------------------------------------
         * Crea los controles
         * --------------------------------------------------------------------
         */
        this._controls = new THREE.OrbitControls(this._three_camera, this._renderer.domElement);
        this._controls.addEventListener('change', this._render);

        // El pan sólo está disponible en móvil, en escritorio usar teclado
        this._controls.enablePan = true;

        // Autorotar (?) esta característica funciona sólo con requestAnimateFrame()
        this._controls.autoRotate = this.objects_props.camera.autorotate;

        // Define límites de la camara
        this._controls.maxPolarAngle = this.objects_props.camera.maxpolarangle;
        this._controls.maxDistance = this.objects_props.camera.maxdistance;

        /**
         * --------------------------------------------------------------------
         * Setea posición inicial de la cámara
         * --------------------------------------------------------------------
         */
        this._setInitialCameraPosition();

        // noinspection JSUnusedGlobalSymbols
        /**
         * --------------------------------------------------------------------
         * Crea el raycaster
         * --------------------------------------------------------------------
         */
        this._raycaster = new THREE.Raycaster();

        /**
         * --------------------------------------------------------------------
         * Inicia la GUI
         * --------------------------------------------------------------------
         */
        if (this.threejs_helpers.gui) {
            this.threejs_helpers.gui = false;
            this._toggleGUI();
        }

    };

    /**
     * Define la posición inicial de la cámara.
     *
     * @function
     * @private
     */
    this._setInitialCameraPosition = function () {

        /**
         * Setea posición inicial
         */
        this._three_camera.position.x = this.objects_props.camera.posy;
        this._three_camera.position.y = this.objects_props.camera.posz;
        this._three_camera.position.z = this.objects_props.camera.posx;

        /**
         * Setea ángulo inicial
         */
        this._three_camera.rotation.x = this.objects_props.camera.rotationy;
        this._three_camera.rotation.y = this.objects_props.camera.rotationz;
        this._three_camera.rotation.z = this.objects_props.camera.rotationx;

        /**
         * Target inicial
         */
        this.objects_props.camera.target.x = self.objects_props.camera.initialTarget.x;
        this.objects_props.camera.target.y = self.objects_props.camera.initialTarget.y;
        this.objects_props.camera.target.z = self.objects_props.camera.initialTarget.z;

        /**
         * Actualiza la cámara
         */
        this._setCameraTarget();

    };

    /**
     * Setea el target de la cámara.
     *
     * @function
     * @private
     */
    this._setCameraTarget = function () {
        // noinspection JSSuspiciousNameCombination
        self._controls.target.set(self.objects_props.camera.target.y, self.objects_props.camera.target.z, self.objects_props.camera.target.x);
        self._controls.update();
    };

    /**
     * Verifica que el target de la cámara se encuentre entre los límites del mundo,
     * desde la versión 2.00 se verifica si colisiona en modo walkingCamera.
     *
     * @function
     * @private
     * @param {string} axis - Eje actualizado
     * @param {number} min - Valor mínimo de la posición
     * @param {number} max - Valor máximo de la posición
     * @returns {boolean} - Indica si la cámara se puede mover
     */
    this._checkCameraTargetLimits = function (axis, min, max) {
        let $pos = self.objects_props.camera.target[axis];
        if (min <= $pos && $pos <= max) {
            return true;
        }
        self.objects_props.camera.target[axis] = Math.min(max, Math.max($pos, min));
        return false;
    };

    /**
     * Calcula la distancia entre dos puntos.
     *
     * @function
     * @private
     * @param {number} a - Número
     * @param {number} b - Número
     * @returns {number}
     */
    this._dist2 = function (a, b) {
        return Math.sqrt(Math.pow(a, 2) + Math.pow(b, 2));
    };

    /**
     * Verifica que el target de la cámara se encuentre entre los límites del mundo,
     * desde la versión 2.00 se verifica si colisiona en modo walkingCamera.
     *
     * @function
     * @private
     * @param {string} axis - Eje actualizado
     * @param {number} min - Valor mínimo de la posición
     * @param {number} max - Valor máximo de la posición
     * @returns {boolean} - Indica si la cámara se puede mover
     */
    this._checkCameraTargetLimits = function (axis, min, max) {
        let $pos = self.objects_props.camera.target[axis];
        if (min <= $pos && $pos <= max) {
            return true;
        }
        self.objects_props.camera.target[axis] = Math.min(max, Math.max($pos, min));
        return false;
    };

    /**
     * Chequea que el target de la cámara no colisione.
     *
     * @function
     * @private
     * @param {string} axis - Eje a comprobar
     * @param {number} val - valor A sumar
     * @returns {boolean} - Indica si no se colisiona (true) o no (false)
     */
    this._checkCameraTargetCollision = function (axis, val) {

        /**
         * Suma al eje
         */
        self.objects_props.camera.target[axis] += val;
        return true;

    };

    /**
     * Incrementa el target de la cámara en un valor.
     *
     * @function
     * @private
     * @param {string} dir - Dirección
     * @param {number} val - Incremento de la dirección
     * @param {boolean=} flipSignPos - Cambia el sentido del incremento según la posición de la cámara
     * @param {boolean=} setTarget - Establece el target de la cámara
     */
    this._updateCameraTarget = function (dir, val, flipSignPos, setTarget) {

        let $factor = 1.0;
        switch (dir) {
            case 'x':
                if (flipSignPos) { // Se actualiza el factor dependiendo de la ubicación de la cámara
                    $factor = Math.sign(self._three_camera.position.z);
                }
                val *= $factor;

                // Actualiza el target y la cámara
                if (self._checkCameraTargetCollision(dir, val) && self._checkCameraTargetLimits(dir, -self.worldsize.x, self.worldsize.x) && self.objects_props.camera.targetMoveCamera) {
                    self._three_camera.position.z += val;
                }
                break;
            case 'y':
                if (flipSignPos) { // Se actualiza el factor dependiendo de la ubicación de la cámara
                    $factor = Math.sign(self._three_camera.position.x);
                }
                val *= $factor;

                // Actualiza el target y la cámara
                if (self._checkCameraTargetCollision(dir, val) && self._checkCameraTargetLimits(dir, -self.worldsize.y, self.worldsize.y) && self.objects_props.camera.targetMoveCamera) {
                    self._three_camera.position.x += val;
                }
                break;
            case 'z':
                if (flipSignPos) { // Se actualiza el factor dependiendo de la ubicación de la cámara
                    // noinspection JSSuspiciousNameCombination
                    $factor = Math.sign(self._three_camera.position.y);
                }
                val *= $factor;

                // Actualiza el target y la cámara
                if (self._checkCameraTargetCollision(dir, val) && self._checkCameraTargetLimits(dir, 0.00001, self.worldsize.z) && self.objects_props.camera.targetMoveCamera) {
                    self._three_camera.position.y += val;
                }
                break;
            default: // Se ignoran otras situaciones
                break;
        }
        if (setTarget) self._setCameraTarget();

    };

    /**
     * Renderiza el contenido de Three.js.
     *
     * @function
     * @private
     */
    this._render = function () {

        /**
         * Renderiza
         */
        self._renderer.render(self._scene, self._three_camera);

        /**
         * Actualiza los helpers
         */
        for (let i = 0; i < self._helpersUpdate.length; i += 1) {
            self._helpersUpdate[i].update();
        }
        if (notNullUndf(self._gui)) {
            self._guiCameraParams.posx = roundNumber(self._three_camera.position.z, 3);
            self._guiCameraParams.posy = roundNumber(self._three_camera.position.x, 3);
            self._guiCameraParams.posz = roundNumber(self._three_camera.position.y, 3);
            self._guiCameraParams.rotationx = roundNumber(self._three_camera.rotation.z, 3);
            self._guiCameraParams.rotationy = roundNumber(self._three_camera.rotation.x, 3);
            self._guiCameraParams.rotationz = roundNumber(self._three_camera.rotation.y, 3);
        }

    };

    /**
     * Actualiza controles y renderiza.
     *
     * @function
     * @private
     */
    this._animateFrame = function () {

        /**
         * Actualiza los controles
         */
        this._controls.update();

        /**
         * Renderiza un cuadro
         */
        this._render();

    };

    /**
     * Thread de animación, dibuja mediante {@link requestAnimationFrame}.
     *
     * @function
     * @private
     */
    this._animationThread = function () {
        if (!self._animateThread) return;
        requestAnimationFrame(self.initAnimate);
        self._animateFrame();
    };

    /**
     * Inicia el thread de actualizaciones.
     *
     * @function
     * @protected
     */
    this.initAnimate = function () {
        self._animateThread = true;
        self._animationThread();
    };

    /**
     * Añade objetos a la escena.
     *
     * @function
     * @private
     */
    this._initWorldObjects = function () {

        /**
         * Añade los helpers
         */
        this._drawHelpers();

        /**
         * Establece el target de la cámara si es que se cambió en initObjectModel
         */
        this._setCameraTarget();

        /**
         * Guarda valores iniciales
         */
        this._saveInitialStatus();

    };

    /**
     * Guarda algunas variables iniciales antes de renderizar, utiliza valores desde initObjectModel.
     *
     * @function
     * @private
     */
    this._saveInitialStatus = function () {
        self.objects_props.camera.initialTarget.x = self.objects_props.camera.target.x;
        self.objects_props.camera.initialTarget.y = self.objects_props.camera.target.y;
        self.objects_props.camera.initialTarget.z = self.objects_props.camera.target.z;
    };

    /**
     * Mueve la cámara en dirección paralela al rayo entre la posición de la cámara y el target.
     *
     * @function
     * @private
     * @param {number} direction - Dirección de avance (1, -1)
     */
    this._moveParallel = function (direction) {

        /**
         * Calcula el ángulo de avance
         * @type {number}
         */
        let $ang = Math.atan2(self._three_camera.position.x - self.objects_props.camera.target.y, self._three_camera.position.z - self.objects_props.camera.target.x);

        /**
         * Calcula las componentes de avance en x/y
         */
        let $dx, $dy;
        $dx = self.objects_props.camera.targetSpeed.x * Math.cos($ang) * direction;
        $dy = self.objects_props.camera.targetSpeed.y * Math.sin($ang) * direction;

        /**
         * Suma las componentes
         */
        self._updateCameraTarget('x', $dx, false, false);
        self._updateCameraTarget('y', $dy, false, true);

    };

    /**
     * Mueve la cámara en dirección ortogonal al rayo entre la posición de la cámara y el target.
     *
     * @function
     * @private
     * @param {number} direction - Dirección de avance (1, -1)
     */
    this._moveOrtho = function (direction) {

        /**
         * Calcula el ángulo de avance
         * @type {number}
         */
        let $ang = Math.atan2(self._three_camera.position.x - self.objects_props.camera.target.y, self._three_camera.position.z - self.objects_props.camera.target.x);
        $ang += Math.PI / 2;

        /**
         * Calcula las componentes de avance en x/y
         */
        let $dx, $dy;
        $dx = self.objects_props.camera.targetSpeed.x * Math.cos($ang) * direction;
        $dy = self.objects_props.camera.targetSpeed.y * Math.sin($ang) * direction;

        /**
         * Suma las componentes, sólo en el segundo cambio se actualiza
         */
        self._updateCameraTarget('x', $dx, false, false);
        self._updateCameraTarget('y', $dy, false, true);

    };

    /**
     * Mueve la cámara de manera vertical en el eje +-Z.
     *
     * @function
     * @private
     * @param {number} direction - Dirección de avance
     */
    this._moveVertical = function (direction) {

        /**
         * Mueve la cámara
         */
        self._updateCameraTarget('z', self.objects_props.camera.targetSpeed.z * direction, false, true);

    };

    /**
     * Rota el objetivo de la cámara en torno a la posición de la cámara.
     *
     * @function
     * @private
     * @param {number} direction - Dirección de avance
     */
    this._rotateTarget = function (direction) {

        /**
         * Obtiene ángulo
         * @type {number}
         */
        let $ang = Math.PI + Math.atan2(self._three_camera.position.x - self.objects_props.camera.target.y, self._three_camera.position.z - self.objects_props.camera.target.x);

        /**
         * Suma velocidad angular
         */
        $ang += direction * self.objects_props.camera.targetSpeed.angular;

        /**
         * Obtiene radio de giro
         */
        let $r = self._dist2(self._three_camera.position.x - self.objects_props.camera.target.y, self._three_camera.position.z - self.objects_props.camera.target.x);

        /**
         * Obtiene nuevas posiciones
         */
        self.objects_props.camera.target.x = self._three_camera.position.z + $r * Math.cos($ang);
        self.objects_props.camera.target.y = self._three_camera.position.x + $r * Math.sin($ang);

        /**
         *  Verifica límites
         */
        self._checkCameraTargetLimits('x', -self.worldsize.x, self.worldsize.x);
        self._checkCameraTargetLimits('y', -self.worldsize.y, self.worldsize.y);

        /**
         * Actualiza la cámara y renderiza
         */
        self._setCameraTarget();
        self._render();

    };

    /**
     * Mueve la cámara al frente.
     *
     * @function
     * @private
     */
    this._moveForward = function () {
        this._moveParallel(-1);
    };

    /**
     * Mueve la cámara hacia atrás.
     *
     * @function
     * @private
     */
    this._moveBackward = function () {
        this._moveParallel(1);
    };

    /**
     * Mueve la cámara a la izquierda.
     *
     * @function
     * @private
     */
    this._moveLeft = function () {
        this._moveOrtho(-1);
    };

    /**
     * Mueve la cámara a la derecha.
     *
     * @function
     * @private
     */
    this._moveRight = function () {
        this._moveOrtho(1);
    };

    /**
     * Mueve la cámara en la coordenada +Z.
     *
     * @function
     * @private
     */
    this._moveUp = function () {
        this._moveVertical(1);
    };

    /**
     * Mueve la cámara en la coordenada -Z.
     *
     * @function
     * @private
     */
    this._moveDown = function () {
        this._moveVertical(-1);
    };

    /**
     * Rota la cámara hacia la izquierda.
     *
     * @function
     * @private
     */
    this._rotateLeft = function () {
        this._rotateTarget(1);
    };

    /**
     * Rota la cámara hacia la derecha.
     *
     * @function
     * @private
     */
    this._rotateRight = function () {
        this._rotateTarget(-1);
    };

    /**
     * Forza el foco en la aplicación.
     *
     * @function
     * @private
     */
    this._forceFocus = function () {
        self.canvasParent.trigger('focus'); // Atrapa focus
    };

    /**
     * Inicia los eventos, se puede reemplazar por otra función.
     *
     * @function
     * @protected
     */
    this._initEvents = function () {

        /**
         * Si el div del mapa no ha sido definido se termina la función
         */
        if (isNullUndf(this.canvasParent)) return;
        console.log('k');

        /**
         * Evento scroll renderiza (orbitControls) en canvas
         */
        this.canvasParent.on(self._eventID.mousewheel, function (e) {
            stopWheelEvent(e);
            e.preventDefault();
            self._animateFrame();
        });

        /**
         * Click izquierdo y derecho desactivan eventos originales en el canvas
         */
        this.canvasParent.on(self._eventID.mousedown, function (e) {
            e.preventDefault();
            self._forceFocus();
            self._animateFrame();
            self._hasMouseOver = true;
            self._hasMousePressed = true;
        });
        this.canvasParent.on(self._eventID.contextmenu, function (e) {
            e.preventDefault();
        });
        this.canvasParent.on(self._eventID.mouseup, function (e) {
            e.preventDefault();
            self._hasMousePressed = false;
        });

        /**
         * Mouse sobre el canvas
         */
        this.canvasParent.on(self._eventID.mouseover, function () {
            self._hasMouseOver = true;
        });
        this.canvasParent.on(self._eventID.mouseout, function () {
            self._hasMouseOver = false;
        });

        /**
         * Presión de botones sobre el canvas al tener el foco activo
         */
        this.canvasParent.on(self._eventID.keydown, function (e) {
            e.preventDefault(); // Cancela todos los botones por defecto
            e.stopPropagation();

            // Setea como key presionado
            self._hasKeyPressed = true;

            // Si se mantiene presionado el mouse retorna
            if (self._hasMousePressed) return;

            switch (e.which) {
                case 87: // [W] Avanza la cámara
                    self._moveForward();
                    break;
                case 83: // [S] Retrocede la cámara
                    self._moveBackward();
                    break;
                case 65: // [A] Mueve la cámara a la izquierda
                    self._moveLeft();
                    break;
                case 68: // [D] Mueve la cámara a la derecha
                    self._moveRight();
                    break;
                case 38: // [FLECHA ARRIBA] Avanza la cámara
                    self._moveForward();
                    break;
                case 40: // [FLECHA ABAJO] Retrocede la cámara
                    self._moveBackward();
                    break;
                case 37: // [FLECHA IZQUIERDA] Rota el target hacia la izquierda
                    self._rotateLeft();
                    break;
                case 39: // [->] Rota el target hacia la derecha
                    self._rotateRight();
                    break;
                case 81: // [Q] Avanza el target de la cámara en el eje -Z
                    self._moveDown();
                    break;
                case 69: // [E] Avanza el target de la cámara en el eje +Z
                    self._moveUp();
                    break;
                default: // Se ignoran otros input
                    break;
            }
        });
        this.canvasParent.on(self._eventID.keyup, function () {
            self._hasKeyPressed = false;
        });

        /**
         * Ajuste automático de la pantalla
         */
        this.threeResize(true);

    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * Detiene el thread de actualización.
     *
     * @function
     * @protected
     */
    this.stopAnimate = function () {
        self._animateThread = false;
    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * Resetea la cámara.
     *
     * @function
     * @private
     */
    this._resetCamera = function () {
        self._setInitialCameraPosition();
        self._animateFrame();
    };

    /**
     * Activa/Desactiva un helper.
     *
     * @function
     * @private
     */
    this._toggleHelper = function () {
        self._drawHelpers();
        self._animateFrame();
        self._forceFocus();
    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * Oculta/Muestra los ejes.
     *
     * @function
     * @private
     */
    this._toggleAxis = function () {
        self.threejs_helpers.axis = !self.threejs_helpers.axis;
        self._toggleHelper();
    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * Oculta/Muestra la grilla.
     *
     * @function
     * @private
     */
    this._toggleGrid = function () {
        self.threejs_helpers.grid = !self.threejs_helpers.grid;
        self._toggleHelper();
    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * Oculta/Muestra los límites del mundo.
     *
     * @function
     * @private
     */
    this._toggleWorldLimits = function () {
        self.threejs_helpers.worldlimits = !self.threejs_helpers.worldlimits;
        self._toggleHelper();
    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * Oculta/Muestra el objetivo de la cámara.
     *
     * @function
     * @private
     */
    this._toggleCameraTarget = function () {
        self.threejs_helpers.cameratarget = !self.threejs_helpers.cameratarget;
        self._toggleHelper();
    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * Oculta/Muestra los planos.
     *
     * @function
     * @private
     */
    this._togglePlanes = function () {
        self.threejs_helpers.planes = !self.threejs_helpers.planes;
        self._toggleHelper();
    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * Oculta/Muestra el lighthelper.
     *
     * @function
     * @private
     */
    this._toggleLightSystemHelper = function () {
        self.threejs_helpers.lighthelper = !self.threejs_helpers.lighthelper;
        self._toggleHelper();
    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * Oculta/Muestra el helper de las sombras.
     *
     * @function
     * @private
     */
    this._toggleShadowHelper = function () {
        self.threejs_helpers.shadowlight = !self.threejs_helpers.shadowlight;
        self._toggleHelper();
    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * Oculta/Muestra la luz.
     *
     * @function
     * @private
     */
    this._toggleLightHelper = function () {
        self.threejs_helpers.light = !self.threejs_helpers.light;
        self._toggleHelper();
    };

    /**
     * Oculta/Muestra la GUI.
     *
     * @function
     * @private
     */
    this._toggleGUI = function () {
        self.threejs_helpers.gui = !self.threejs_helpers.gui;
        if (self.threejs_helpers.gui) {
            self._buildGUI();
        } else {
            self._destroyGUI();
        }
    };

    /**
     * Construye una interfaz gráfica para el render.
     *
     * @function
     * @private
     */
    this._buildGUI = function () {

        /**
         * Crea una GUI
         */
        this._gui = new dat.GUI({autoPlace: false});

        /**
         * --------------------------------------------------------------------
         * Carpeta luz ambiental
         * --------------------------------------------------------------------
         */
        this._ambientLightParam = {
            color: self._ambientlight.color.getHex(),
            intensity: self._ambientlight.intensity
        };
        let ambientlightfolder = this._gui.addFolder(lang.viewer_3d_gui_ambientlight);
        ambientlightfolder.addColor(this._ambientLightParam, 'color').onChange(function (val) {
            self._ambientlight.color.setHex(val);
            self._render();
        });
        ambientlightfolder.add(this._ambientLightParam, 'intensity', 0, 5).onChange(function (val) {
            self._ambientlight.intensity = val;
            self._render();
        });

        /**
         * --------------------------------------------------------------------
         * Carpeta luz de la cámara
         * --------------------------------------------------------------------
         */
        this._cameraLightParam = {
            color: self._cameralight.color.getHex(),
            intensity: self._cameralight.intensity,
            distance: self._cameralight.distance,
            decay: self._cameralight.decay
        };
        let cameralightfolder = this._gui.addFolder(lang.viewer_3d_gui_cameralight);
        cameralightfolder.addColor(this._cameraLightParam, 'color').onChange(function (val) {
            self._cameralight.color.setHex(val);
            self._render();
        });
        cameralightfolder.add(this._cameraLightParam, 'intensity', 0, 5).onChange(function (val) {
            self._cameralight.intensity = val;
            self._render();
        });
        cameralightfolder.add(this._cameraLightParam, 'distance', 0, 3 * self.worldsize.diagl).onChange(function (val) {
            self._cameralight.distance = val;
            self._render();
        });
        cameralightfolder.add(this._cameraLightParam, 'decay', 1, 2).onChange(function (val) {
            self._cameralight.decay = val;
            self._render();
        });

        /**
         * --------------------------------------------------------------------
         * Carpeta luz puntual
         * --------------------------------------------------------------------
         */
        this._guiLightParam = {
            color: self._light.color.getHex(),
            intensity: self._light.intensity,
            distance: self._light.distance,
            angle: self._light.angle,
            penumbra: self._light.penumbra,
            decay: self._light.decay,
            posx: self._light.position.z,
            posy: self._light.position.x,
            posz: self._light.position.y,
            planeshadow: self.objects_props.light.planeshadow,
            radius: self._distOriginxyz(self._light.position.x, self._light.position.y, self._light.position.z),
            rotatexy: (Math.atan2(this._light.position.z, this._light.position.x) * 180 / Math.PI + 360) % 360,
            rotatexz: 0.0,
            rotateyz: 0.0,
            mapsizeh: self._light.shadow.mapSize.height,
            mapsizew: self._light.shadow.mapSize.width
        };
        let lightfolder = this._gui.addFolder(lang.viewer_3d_gui_light);
        lightfolder.addColor(this._guiLightParam, 'color').onChange(function (val) {
            self._light.color.setHex(val);
            self._render();
        });
        lightfolder.add(this._guiLightParam, 'intensity', 0, 10).onChange(function (val) {
            self._light.intensity = val;
            self._render();
        });
        lightfolder.add(this._guiLightParam, 'distance', 0.01, 3 * self.worldsize.diagl).onChange(function (val) {
            self._light.distance = val;
            self._render();
        });
        lightfolder.add(this._guiLightParam, 'angle', 0.01, Math.PI / 2).onChange(function (val) {
            self._light.angle = val;
            self._render();
        });
        lightfolder.add(this._guiLightParam, 'penumbra', 0, 1).onChange(function (val) {
            self._light.penumbra = val;
            self._render();
        });
        lightfolder.add(this._guiLightParam, 'decay', 1, 2).onChange(function (val) {
            self._light.decay = val;
            self._render();
        });
        lightfolder.add(this._guiLightParam, 'posx', -self.worldsize.diagl, self.worldsize.diagl).onChange(function (val) {
            self._light.position.z = val;
            self._render();
            // noinspection JSSuspiciousNameCombination
            self._guiLightParam.rotatexy = (180 * Math.atan2(self._light.position.x, self._light.position.z) / Math.PI + 360) % 360;
        }).listen();
        lightfolder.add(this._guiLightParam, 'posy', -self.worldsize.diagl, self.worldsize.diagl).onChange(function (val) {
            self._light.position.x = val;
            self._render();
        }).listen();
        lightfolder.add(this._guiLightParam, 'posz', 0, self.worldsize.diagl).onChange(function (val) {
            self._light.position.y = val;
            self._render();
        }).listen();
        lightfolder.add(this._guiLightParam, 'radius', 0.01, self.worldsize.diagl).onChange(function (r) {

            // Radio actual
            let $ract = self._distOriginxyz(self._light.position.x, self._light.position.y, self._light.position.z);

            // Calcula los ángulos actuales
            let angxy, angxyz;
            // noinspection JSSuspiciousNameCombination
            angxy = Math.atan2(self._light.position.x, self._light.position.z);
            angxyz = Math.asin(self._light.position.y / $ract);

            // Calcula distancia proyección radio plano xy radio nuevo
            let $e = r * Math.cos(angxyz);

            // Calcula las nuevas posiciones
            let $x, $y, $z;
            $x = $e * Math.cos(angxy);
            $y = $e * Math.sin(angxy);
            $z = $e * Math.tan(angxyz);

            // noinspection JSSuspiciousNameCombination
            self._light.position.x = $y; // Actualiza las posiciones
            self._light.position.y = $z;
            self._light.position.z = $x;
            self._render();

            // Actualiza entradas en el menú
            self._guiLightParam.posx = self._light.position.x;
            self._guiLightParam.posy = self._light.position.z;
            self._guiLightParam.posz = self._light.position.y;

        }).listen();
        lightfolder.add(this._guiLightParam, 'rotatexy', 0, 360).onChange(function (val) {

            // Calcula el ángulo en radianes
            val = (val) * Math.PI / 180;

            // Posiciones actuales
            let posx, posy;
            posx = self._light.position.x;
            posy = self._light.position.z;

            // noinspection JSSuspiciousNameCombination
            let r = Math.sqrt(Math.pow(posx, 2) + Math.pow(posy, 2)); // Radio de giro

            // Actualiza la posición
            self._light.position.z = r * Math.cos(val);
            self._light.position.x = r * Math.sin(val);
            self._render();

            // Actualiza entradas en el menú
            self._guiLightParam.posx = self._light.position.z;
            self._guiLightParam.posy = self._light.position.x;

        }).listen();

        /**
         * --------------------------------------------------------------------
         * Carpeta niebla
         * --------------------------------------------------------------------
         */
        let fogfolder = this._gui.addFolder(lang.viewer_3d_gui_fog);
        this._guiFogParams = {
            color: self._fog.color.getHex(),
            density: self._fog.density,
            enabled: self.objects_props.fog.enabled,
        };
        fogfolder.addColor(this._guiFogParams, 'color').onChange(function (val) {
            self._fog.color.setHex(val);
            self._render();
        });
        fogfolder.add(this._guiFogParams, 'density', 0.00001, 0.00025 * 10).onChange(function (val) {
            self._fog.density = val;
            self._render();
        });
        fogfolder.add(this._guiFogParams, 'enabled').onChange(function (val) {
            if (val) {
                self._scene.fog = self._fog;
            } else {
                self._scene.fog = null;
            }
            self._animateFrame();
        });

        /**
         * --------------------------------------------------------------------
         * Carpeta sombra
         * --------------------------------------------------------------------
         */
        let shadowfolder = this._gui.addFolder(lang.viewer_3d_gui_shadow);
        shadowfolder.add(this._guiLightParam, 'mapsizew', 512, 2 * 1024).onChange(function (val) {
            self._light.shadow.mapSize.width = val;
            self._render();
        });
        shadowfolder.add(this._guiLightParam, 'mapsizeh', 512, 2 * 1024).onChange(function (val) {
            self._light.shadow.mapSize.height = val;
            self._render();
        });
        shadowfolder.add(this._guiLightParam, 'planeshadow').onChange(function (val) {
            self.objects_props.plane.obj.receiveShadow = val;
            self._animateFrame();
        });

        /**
         * --------------------------------------------------------------------
         * Carpeta cámara
         * --------------------------------------------------------------------
         */
        let camerafolder = this._gui.addFolder(lang.viewer_3d_gui_camera);
        this._guiCameraParams = {
            fov: self._three_camera.fov,
            far: self._three_camera.far,
            zoom: self._three_camera.zoom,
            maxdistance: self._controls.maxDistance,
            maxpolarangle: self._controls.maxPolarAngle,
            posx: self._three_camera.position.z,
            posy: self._three_camera.position.x,
            posz: self._three_camera.position.y,
            rotationx: Number(self._three_camera.rotation.z) + 0.01,
            rotationy: Number(self._three_camera.rotation.x) + 0.01,
            rotationz: Number(self._three_camera.rotation.y) + 0.01
        };
        camerafolder.add(this._guiCameraParams, 'fov', 1, 179).onChange(function (val) {
            self._three_camera.fov = val;
            self._three_camera.updateProjectionMatrix();
            self._animateFrame();
        });
        camerafolder.add(this._guiCameraParams, 'far', 100, 10000).onChange(function (val) {
            self._three_camera.far = val;
            self._three_camera.updateProjectionMatrix();
            self._animateFrame();
        });
        camerafolder.add(this._guiCameraParams, 'zoom', 0.1, 10).onChange(function (val) {
            self._three_camera.zoom = val;
            self._three_camera.updateProjectionMatrix();
            self._animateFrame();
        });
        camerafolder.add(this._guiCameraParams, 'maxdistance', 100, 5 * self.worldsize.diagl).onChange(function (val) {
            self._controls.maxDistance = val;
            self._animateFrame();
        });
        camerafolder.add(this._guiCameraParams, 'maxpolarangle', 0, Math.PI).onChange(function (val) {
            self._controls.maxPolarAngle = val;
            self._animateFrame();
        });
        camerafolder.add(this._guiCameraParams, 'posx', 1, 2 * self.worldsize.diagl).onChange(function (val) {
            self._three_camera.position.z = val;
            self._animateFrame();
        }).listen();
        camerafolder.add(this._guiCameraParams, 'posy', 1, 2 * self.worldsize.diagl).onChange(function (val) {
            self._three_camera.position.x = val;
            self._animateFrame();
        }).listen();
        camerafolder.add(this._guiCameraParams, 'posz', 1, 2 * self.worldsize.diagl).onChange(function (val) {
            self._three_camera.position.y = val;
            self._animateFrame();
        }).listen();
        camerafolder.add(this._guiCameraParams, 'rotationx', -Math.PI, Math.PI).onChange(function (val) {
            self._three_camera.rotation.z = val;
            self._animateFrame();
        }).listen();
        camerafolder.add(this._guiCameraParams, 'rotationy', -Math.PI, Math.PI).onChange(function (val) {
            self._three_camera.rotation.x = val;
            self._animateFrame();
        }).listen();
        camerafolder.add(this._guiCameraParams, 'rotationz', -Math.PI / 2, Math.PI / 2).onChange(function (val) {
            self._three_camera.rotation.y = val;
            self._animateFrame();
        }).listen();

        /**
         * Añade la GUI al div
         */
        $('#' + this.guiID).append(this._gui.domElement);

        /**
         * Inicia la GUI cerrada
         */
        if (this.threejs_helpers.guistartclosed) {
            this._gui.close();
        }

    };

    /**
     * Destruye la GUI.
     *
     * @function
     * @private
     */
    this._destroyGUI = function () {
        if (notNullUndf(self._gui)) {
            self._gui.destroy();
            self._gui = null;
            $('#' + self.guiID).empty();
        }
    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * Añade visualizador de FPS.
     *
     * @function
     * @private
     */
    this._toggleFPSMeter = function () {

        /**
         * Si no se ha activado retorna
         */
        if (!self.threejs_helpers.fpsmeter) return;

        /**
         * Si no se ha creado el FPS
         */
        if (isNullUndf(self._helperInstances.fpsmeter)) {
            let stats = new Stats();
            self.canvasParent.append(stats.dom);
            requestAnimationFrame(function loop() {
                stats.update();
                requestAnimationFrame(loop);
            });
            self._helperInstances.fpsmeter = stats;
        }

        /**
         * Si se creó se elimina del DOM (toggle)
         */
        else {
            self._helperInstances.fpsmeter.dom.remove();
            self._helperInstances.fpsmeter = null;
        }

    };

    /**
     * Dibuja los helpers de Three.js según configuración.
     *
     * @function
     * @private
     */
    this._drawHelpers = function () {

        /**
         * Variable local helper
         */
        let helper;

        /**
         * --------------------------------------------------------------------
         * Dibuja ejes
         * --------------------------------------------------------------------
         */
        if (this.threejs_helpers.axis) {
            if (isNullUndf(this._helperInstances.axis)) {
                let $helpersize = Math.min(self.worldsize.x, self.worldsize.y, self.worldsize.z) * self.threejs_helpers.axissize;
                helper = new THREE.AxesHelper($helpersize);
                self.addMeshToScene(helper, this.globals.helper, false);
                // noinspection JSValidateTypes
                this._helperInstances.axis = helper; // Añade la instancia
            }
        } else { // Se elimina el helper si es que se instanció
            if (notNullUndf(this._helperInstances.axis)) {
                self._scene.remove(this._helperInstances.axis);
            }
            this._helperInstances.axis = null;
        }


        /**
         * --------------------------------------------------------------------
         * Dibuja planos x, y, z
         * --------------------------------------------------------------------
         */
        if (this.threejs_helpers.planes) {
            if (isNullUndf(this._helperInstances.planes)) {
                let $planes = [];

                // Colores de los planos
                let materialx = new THREE.MeshBasicMaterial({
                    color: self.threejs_helpers.planecolorx,
                    opacity: self.threejs_helpers.planeopacity,
                });
                let materialy = new THREE.MeshBasicMaterial({
                    color: self.threejs_helpers.planecolory,
                    opacity: self.threejs_helpers.planeopacity,
                });
                let materialz = new THREE.MeshBasicMaterial({
                    color: self.threejs_helpers.planecolorz,
                    opacity: self.threejs_helpers.planeopacity,
                });
                materialx.wireframe = true;
                materialx.aoMapIntensity = self.threejs_helpers.planeopacity;
                materialy.wireframe = true;
                materialy.aoMapIntensity = self.threejs_helpers.planeopacity;
                materialz.wireframe = true;
                materialz.aoMapIntensity = self.threejs_helpers.planeopacity;

                // Geometrías de los plannos
                let geometry, plane;

                // Plano x
                geometry = new THREE.Geometry();
                geometry.vertices.push(
                    this._newThreePoint(this.worldsize.x, 0, 0),
                    this._newThreePoint(-this.worldsize.x, 0, 0),
                    this._newThreePoint(-this.worldsize.x, 0, this.worldsize.z),
                    this._newThreePoint(this.worldsize.x, 0, this.worldsize.z)
                );
                geometry.faces.push(new THREE.Face3(0, 1, 2));
                geometry.faces.push(new THREE.Face3(0, 2, 3));
                plane = new THREE.Mesh(geometry, materialx);
                plane.position.y = 0;
                self.addMeshToScene(plane, this.globals.helper, false);
                $planes.push(plane);

                // Plano y
                geometry = new THREE.Geometry();
                geometry.vertices.push(
                    this._newThreePoint(0, -this.worldsize.y, 0),
                    this._newThreePoint(0, this.worldsize.y, 0),
                    this._newThreePoint(0, this.worldsize.y, this.worldsize.z),
                    this._newThreePoint(0, -this.worldsize.y, this.worldsize.z)
                );
                geometry.faces.push(new THREE.Face3(0, 1, 2));
                geometry.faces.push(new THREE.Face3(0, 2, 3));
                plane = new THREE.Mesh(geometry, materialy);
                plane.position.y = 0;
                self.addMeshToScene(plane, this.globals.helper, false);
                $planes.push(plane);

                // Plano z
                geometry = new THREE.Geometry();
                geometry.vertices.push(
                    this._newThreePoint(this.worldsize.x, this.worldsize.y, 0),
                    this._newThreePoint(-this.worldsize.x, this.worldsize.y, 0),
                    this._newThreePoint(-this.worldsize.x, -this.worldsize.y, 0),
                    this._newThreePoint(this.worldsize.x, -this.worldsize.y, 0)
                );
                geometry.faces.push(new THREE.Face3(0, 1, 2));
                geometry.faces.push(new THREE.Face3(0, 2, 3));
                plane = new THREE.Mesh(geometry, materialz);
                plane.position.y = 0;
                self.addMeshToScene(plane, this.globals.helper, false);
                $planes.push(plane);

                // noinspection JSValidateTypes
                this._helperInstances.planes = $planes; // Añade instancia
            }
        } else { // Se elimina el helper si es que se instanció
            if (notNullUndf(this._helperInstances.planes)) {
                let $planes = this._helperInstances.planes;
                for (let i = 0; i < $planes.length; i += 1) {
                    this._scene.remove($planes[i]);
                }
            }
            this._helperInstances.planes = null;
        }

        /**
         * --------------------------------------------------------------------
         * Dibuja grilla en el plano
         * --------------------------------------------------------------------
         */
        if (this.threejs_helpers.grid) {
            if (isNullUndf(this._helperInstances.grid)) {
                let $mapsize = 2 * Math.max(this.worldsize.x, this.worldsize.y);
                let $griddist = Math.floor(2 / this.threejs_helpers.griddist);
                helper = new THREE.GridHelper($mapsize, $griddist);
                helper.position.y = 0;
                helper.material.opacity = 0.1;
                helper.material.transparent = true;
                self.addMeshToScene(helper, this.globals.helper, false);
                // noinspection JSValidateTypes
                this._helperInstances.grid = helper; // Añade la instancia
            }
        } else { // Se elimina el helper si es que se instanció
            if (notNullUndf(this._helperInstances.grid)) {
                this._scene.remove(this._helperInstances.grid);
            }
            this._helperInstances.grid = null;
        }

        /**
         * --------------------------------------------------------------------
         * Dibuja helper de la luz
         * --------------------------------------------------------------------
         */
        if (this.threejs_helpers.lighthelper) {
            if (isNullUndf(this._helperInstances.lighthelper)) {
                helper = new THREE.SpotLightHelper(this._light);
                self.addMeshToScene(helper, this.globals.helper, false);
                this._helpersUpdate.push(helper);
                // noinspection JSValidateTypes
                this._helperInstances.lighthelper = { // Añade la instancia
                    update: this._helpersUpdate.length - 1,
                    obj: helper
                }
            }
        } else { // Se elimina el helper si es que se instanció
            if (notNullUndf(this._helperInstances.lighthelper)) {
                this._helpersUpdate.splice(this._helperInstances.lighthelper.update, 1);
                this._scene.remove(this._helperInstances.lighthelper.obj);
            }
            this._helperInstances.lighthelper = null;
        }

        /**
         * --------------------------------------------------------------------
         * Dibuja la luz
         * --------------------------------------------------------------------
         */
        if (this.threejs_helpers.light) {
            if (isNullUndf(this._helperInstances.light)) {
                let $lightsize = Math.min(self.worldsize.x, self.worldsize.y, self.worldsize.z) * self.threejs_helpers.lightsize;
                let sphereGeometry = new THREE.SphereGeometry($lightsize / 2, 16, 8);
                let wireframeMaterial = new THREE.MeshBasicMaterial(
                    {
                        color: self._light.color.getHex(),
                        transparent: true,
                        wireframe: true,
                    });
                let mesh = new THREE.Mesh(sphereGeometry, wireframeMaterial);
                let $update = function () {
                    mesh.position.x = self._light.position.x;
                    mesh.position.y = self._light.position.y;
                    mesh.position.z = self._light.position.z;
                    mesh.material.color = self._light.color;
                };
                $update();
                self.addMeshToScene(mesh, this.globals.helper, false);
                this._helpersUpdate.push({
                    update: $update
                });

                // noinspection JSValidateTypes
                this._helperInstances.light = { // Añade la instancia
                    update: this._helpersUpdate.length - 1,
                    obj: mesh
                }
            }
        } else { // Se elimina el helper si es que se instanció
            if (notNullUndf(this._helperInstances.light)) {
                this._helpersUpdate.splice(this._helperInstances.light.update, 1);
                this._scene.remove(this._helperInstances.light.obj);
            }
            this._helperInstances.light = null;
        }

        /**
         * --------------------------------------------------------------------
         * Dibuja sombra de cámara
         * --------------------------------------------------------------------
         */
        if (this.threejs_helpers.shadowlight) {
            if (isNullUndf(this._helperInstances.shadowlight)) {
                helper = new THREE.CameraHelper(this._light.shadow.camera);
                self.addMeshToScene(helper, this.globals.helper, false);
                this._helpersUpdate.push(helper);
                // noinspection JSValidateTypes
                this._helperInstances.shadowlight = helper; // Añade la instancia
            }
        } else { // Se elimina el helper si es que se instanció
            if (notNullUndf(this._helperInstances.shadowlight)) {
                this._scene.remove(this._helperInstances.shadowlight);
            }
            this._helperInstances.shadowlight = null;
        }

        /**
         * --------------------------------------------------------------------
         * Dibuja límites del mapa
         * --------------------------------------------------------------------
         */
        if (this.threejs_helpers.worldlimits) {
            if (isNullUndf(this._helperInstances.worldlimits)) {
                let material = new THREE.MeshBasicMaterial({
                    color: self.threejs_helpers.worldlimitscolor,
                    opacity: self.threejs_helpers.planeopacity
                });
                material.wireframe = true;
                material.aoMapIntensity = 0.5;
                let geometry = new THREE.Geometry();
                geometry.vertices.push(
                    // Plano +x
                    this._newThreePoint(this.worldsize.x, -this.worldsize.y, 0),
                    this._newThreePoint(this.worldsize.x, this.worldsize.y, 0),
                    this._newThreePoint(this.worldsize.x, this.worldsize.y, this.worldsize.z),
                    this._newThreePoint(this.worldsize.x, -this.worldsize.y, this.worldsize.z),

                    // Plano +y
                    this._newThreePoint(this.worldsize.x, this.worldsize.y, 0),
                    this._newThreePoint(-this.worldsize.x, this.worldsize.y, 0),
                    this._newThreePoint(-this.worldsize.x, this.worldsize.y, this.worldsize.z),
                    this._newThreePoint(this.worldsize.x, this.worldsize.y, this.worldsize.z),

                    // Plano -x
                    this._newThreePoint(-this.worldsize.x, -this.worldsize.y, 0),
                    this._newThreePoint(-this.worldsize.x, this.worldsize.y, 0),
                    this._newThreePoint(-this.worldsize.x, this.worldsize.y, this.worldsize.z),
                    this._newThreePoint(-this.worldsize.x, -this.worldsize.y, this.worldsize.z),

                    // Plano -y
                    this._newThreePoint(this.worldsize.x, -this.worldsize.y, 0),
                    this._newThreePoint(-this.worldsize.x, -this.worldsize.y, 0),
                    this._newThreePoint(-this.worldsize.x, -this.worldsize.y, this.worldsize.z),
                    this._newThreePoint(this.worldsize.x, -this.worldsize.y, this.worldsize.z),

                    // Plano z
                    this._newThreePoint(this.worldsize.x, -this.worldsize.y, this.worldsize.z),
                    this._newThreePoint(this.worldsize.x, this.worldsize.y, this.worldsize.z),
                    this._newThreePoint(-this.worldsize.x, this.worldsize.y, this.worldsize.z),
                    this._newThreePoint(-this.worldsize.x, -this.worldsize.y, this.worldsize.z)
                );
                for (let j = 0; j <= 4; j += 1) {
                    geometry.faces.push(new THREE.Face3(4 * j, 4 * j + 1, 4 * j + 2));
                    geometry.faces.push(new THREE.Face3(4 * j, 4 * j + 2, 4 * j + 3));
                }
                let cube = new THREE.Mesh(geometry, material);
                cube.position.y = 0;
                self.addMeshToScene(cube, this.globals.helper, false);
                // noinspection JSValidateTypes
                this._helperInstances.worldlimits = cube; // Añade la instancia
            }
        } else { // Se elimina el helper si es que se instanció
            if (notNullUndf(this._helperInstances.worldlimits)) {
                this._scene.remove(this._helperInstances.worldlimits);
            }
            this._helperInstances.worldlimits = null;
        }

        /**
         * --------------------------------------------------------------------
         * Dibuja el objetivo de la cámara
         * --------------------------------------------------------------------
         */
        if (this.threejs_helpers.cameratarget) {
            if (isNullUndf(this._helperInstances.cameratarget)) {
                let $targetsize = Math.min(self.worldsize.x, self.worldsize.y, self.worldsize.z) * self.threejs_helpers.cameratargetsize / 2;
                let sphereGeometry = new THREE.SphereGeometry($targetsize, 16, 8);
                let wireframeMaterial = new THREE.MeshBasicMaterial(
                    {
                        color: self.threejs_helpers.cameratargetcolor,
                        transparent: true,
                        wireframe: true,
                    });
                let mesh = new THREE.Mesh(sphereGeometry, wireframeMaterial);
                let $update = function () {
                    // noinspection JSSuspiciousNameCombination
                    mesh.position.x = self.objects_props.camera.target.y;
                    mesh.position.y = self.objects_props.camera.target.z;
                    mesh.position.z = self.objects_props.camera.target.x;
                };
                $update();
                self.addMeshToScene(mesh, this.globals.helper, false);
                this._helpersUpdate.push({
                    update: $update,
                });

                // noinspection JSValidateTypes
                this._helperInstances.cameratarget = { // Añade la instancia
                    update: this._helpersUpdate.length - 1,
                    obj: mesh
                }
            }
        } else { // Se elimina el helper si es que se instanció
            if (notNullUndf(this._helperInstances.cameratarget)) {
                this._helpersUpdate.splice(this._helperInstances.cameratarget.update, 1);
                this._scene.remove(this._helperInstances.cameratarget.obj);
            }
            this._helperInstances.cameratarget = null;
        }

        /**
         * --------------------------------------------------------------------
         * Dibuja la posición del walkingcamera
         * --------------------------------------------------------------------
         */
        if (this.threejs_helpers.walkcamerapos) {
            if (isNullUndf(this._helperInstances.walkcamerapos)) {
                let $walkersize = Math.min(self.worldsize.x, self.worldsize.y, self.worldsize.z) * self.threejs_helpers.walkcamerasize / 2;
                let sphereGeometry = new THREE.SphereGeometry($walkersize, 16, 8);
                let wireframeMaterial = new THREE.MeshBasicMaterial(
                    {
                        color: self.threejs_helpers.walkcameracolor,
                        transparent: true,
                        wireframe: true,
                    });
                let mesh = new THREE.Mesh(sphereGeometry, wireframeMaterial);
                let $update = function () {
                    mesh.position.x = self._three_camera.position.x;
                    mesh.position.y = self._three_camera.position.y;
                    mesh.position.z = self._three_camera.position.z;
                };
                $update();
                self.addMeshToScene(mesh, this.globals.helper, false);
                this._helpersUpdate.push({
                    update: $update,
                });

                // noinspection JSValidateTypes
                this._helperInstances.walkcamerapos = { // Añade la instancia
                    obj: mesh,
                    update: this._helpersUpdate.length - 1,
                }
            }
        } else { // Se elimina el helper si es que se instanció
            if (notNullUndf(this._helperInstances.walkcamerapos)) {
                this._helpersUpdate.splice(this._helperInstances.walkcamerapos.update, 1);
                this._scene.remove(this._helperInstances.walkcamerapos.obj);
            }
            this._helperInstances.walkcamerapos = null;
        }

    };

    /**
     * Añade un mesh a la escena.
     *
     * @function
     * @protected
     * @param {Object3D} mesh - Mesh
     * @param {string} name - Nombre del objeto
     * @param {boolean=} collaidable - Indica si el objeto es colisionable o no
     */
    this.addMeshToScene = function (mesh, name, collaidable) {

        /**
         * Se aplican propiedades
         */
        mesh.name = name;

        /**
         * Se añade el mesh a la escena
         */
        self._scene.add(mesh);

        /**
         * Material colisionable o no
         */
        if (collaidable) self.addToCollidable(mesh);

    };

    /**
     * Añade un mesh a la lista de objetos colisionables.
     *
     * @function
     * @protected
     * @param {object} mesh - Mesh con características colisionables
     */
    this.addToCollidable = function (mesh) {
        this._collaidableMeshes.push(mesh);
    };

    /**
     * Inserta un punto en (x,y,z).
     *
     * @function
     * @private
     * @param {number} x - Coordenada en x
     * @param {number} y - Coordenada en y
     * @param {number} z - Coordenada en z
     * @return {Vector3}
     */
    this._newThreePoint = function (x, y, z) {
        return new THREE.Vector3(y, z, x);
    };

    /**
     * Retorna la distancia de tres puntos al origen.
     *
     * @function
     * @private
     * @param {number} x - Coordenada x
     * @param {number} y - Coordenada y
     * @param {number} z - Coordenada z
     */
    this._distOriginxyz = function (x, y, z) {
        // noinspection JSSuspiciousNameCombination
        return Math.sqrt(Math.pow(x, 2) + Math.pow(y, 2) + Math.pow(z, 2));
    };

    /**
     * Inicia el objeto de dibujo del shader.
     *
     * @function
     */
    this.initShaderObject = function () {
        let plot_things = {};
        plot_things.geometry = new THREE.BufferGeometry();

        let z_r = new Float32Array(6);
        let z_i = new Float32Array(6);
        let vertices = new Float32Array(18);

        // First triangle:
        let plot_z = 0;
        vertices[0] = -1.0;
        vertices[1] = -1.0;
        vertices[2] = plot_z;

        vertices[3] = 1.0;
        vertices[4] = 1.0;
        vertices[5] = plot_z;

        vertices[6] = -1.0;
        vertices[7] = 1.0;
        vertices[8] = plot_z;

        // Second triangle.
        vertices[9] = -1.0;
        vertices[10] = -1.0;
        vertices[11] = plot_z;

        vertices[12] = 1.0;
        vertices[13] = 1.0;
        vertices[14] = plot_z;

        vertices[15] = 1.0;
        vertices[16] = -1.0;
        vertices[17] = plot_z;

        plot_things.material = new THREE.ShaderMaterial({
            "uniforms": {"max_iterations": {"type": "i", "value": 100}},
            "vertexShader": document.getElementById("shader_vertex").textContent,
            "fragmentShader": document.getElementById("shader_fragment").textContent,
            "side": THREE.DoubleSide
        });

        plot_things.geometry = new THREE.BufferGeometry();

        plot_things.geometry.addAttribute("position", new THREE.BufferAttribute(vertices, 3));
        plot_things.geometry.addAttribute("vertex_z_r", new THREE.BufferAttribute(z_r, 1));
        plot_things.geometry.addAttribute("vertex_z_i", new THREE.BufferAttribute(z_i, 1));
        plot_things.geometry.rotateX(-Math.PI / 2);
        plot_things.geometry.rotateY(-Math.PI / 2);


        plot_things.quad = new THREE.Mesh(
            plot_things.geometry,
            plot_things.material
        );

        this._scene.add(plot_things.quad);

        plot_things.init_mid_z_r = -0.5;
        plot_things.init_mid_z_i = 0;

        // I'm defining the range as half the length of the current view.
        plot_things.init_range = 2;
        // set_plot_bounds(plot_things.init_mid_z_r, plot_things.init_mid_z_i, plot_things.init_range);

        z_r = plot_things.quad.geometry.attributes.vertex_z_r.array;
        z_i = plot_things.quad.geometry.attributes.vertex_z_i.array;

        let mid_z_r = plot_things.init_mid_z_r;
        let mid_z_i = plot_things.init_mid_z_i;
        let range = 2;

        // First triangle:
        z_r[0] = mid_z_r - range;
        z_i[0] = mid_z_i - range;

        z_r[1] = mid_z_r + range;
        z_i[1] = mid_z_i + range;

        z_r[2] = mid_z_r - range;
        z_i[2] = mid_z_i + range;

        // Second triangle:
        z_r[3] = mid_z_r - range;
        z_i[3] = mid_z_i - range;

        z_r[4] = mid_z_r + range;
        z_i[4] = mid_z_i + range;

        z_r[5] = mid_z_r + range;
        z_i[5] = mid_z_i - range;

        plot_things.quad.geometry.attributes.vertex_z_r.needsUpdate = true;
        plot_things.quad.geometry.attributes.vertex_z_i.needsUpdate = true;

        this._animateFrame();
    };

}