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
     * Selector del shader
     * @type {null | JQuery<HTMLElement> | HTMLElement}
     * @private
     */
    this._shaderSelector = null;

    /**
     * Variables del shader
     * @private
     */
    this._shaderObject = {
        datashader: {           // Contiene los datos cargados del shader
            f: '',
            v: '',
        },
        geometry: null,         // Geometría
        init: {                 // Valores iniciales del shader
            zi: 0,
            zr: 0,
            range: 0,
        },
        iters: 100,             // Número de iteraciones del shader
        material: null,         // Material del shader
        plotz: 0.0,             // Altura del objeto
        quad: null,             // Contiene la geometría + material
        vertex: {               // Contiene los vértices
            zi: 0,
            zr: 0,
        },
    };


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
        planes: false,                   // Dibuja los planos
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
        planecolorx: 0X0000FF,          // Color plano x
        planecolory: 0XFF0000,          // Color plano y
        planecolorz: 0X00FF00,          // Color plano z
        planeopacity: 0.5,              // Opacidad del plano
        worldlimitscolor: 0X444444,     // Colores límite del mundo

    };

    // noinspection JSUnusedGlobalSymbols
    /**
     * ID de la GUI
     * @type {object | string}
     * @protected
     */
    this._guiID = 'viewer-gui';

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
        planes: null,
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
            maxpolarangle: 1.000,               // Máximo ángulo que puede alcanzar la cámara (Por pi/2)
            near: 0.001,                        // Plano cercano de la cámara
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
                z: 0.050,
            },
            zoom: 1.000,                        // Factor de zoom
        },

    };

    /**
     * Límites del mundo, modificar
     */
    this._worldsize = {
        x: 1.000,
        y: 1.000,
        z: 1.000,
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
        this._worldsize.diagl = Math.sqrt(Math.pow(2 * this._worldsize.x, 2) + Math.pow(2 * this._worldsize.y, 2) + Math.pow(this._worldsize.z, 2));
        this._worldsize.diagx = Math.sqrt(Math.pow(2 * this._worldsize.x, 2) + Math.pow(this._worldsize.z, 2));
        this._worldsize.diagy = Math.sqrt(Math.pow(2 * this._worldsize.y, 2) + Math.pow(this._worldsize.z, 2));

        /**
         * --------------------------------------------------------------------
         * Setea cámara
         * --------------------------------------------------------------------
         */

        // Restricciones de la cámara
        this.objects_props.camera.far *= this._worldsize.diagl;
        this.objects_props.camera.maxdistance *= this._worldsize.diagl;
        this.objects_props.camera.maxpolarangle *= Math.PI / 2;

        // Posición inicial de la cámara
        this.objects_props.camera.posx *= this._worldsize.x;
        this.objects_props.camera.posy *= this._worldsize.y;
        this.objects_props.camera.posz *= this._worldsize.z;

        // Rotaciones iniciales
        this.objects_props.camera.rotationx *= Math.PI / 2;
        this.objects_props.camera.rotationy *= Math.PI / 2;
        this.objects_props.camera.rotationz *= Math.PI / 2;

        // Target de la cámara
        this.objects_props.camera.target.x *= this._worldsize.x;
        this.objects_props.camera.target.y *= this._worldsize.y;
        this.objects_props.camera.target.z *= this._worldsize.z;

        // Velocidad de movimiento del target en actualización de controles
        this.objects_props.camera.targetSpeed.x *= this._worldsize.x;
        this.objects_props.camera.targetSpeed.y *= this._worldsize.y;
        this.objects_props.camera.targetSpeed.z *= this._worldsize.z;
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
        this.objects_props.camera.light.distance *= this._worldsize.diagl;

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
                if (self._checkCameraTargetCollision(dir, val) && self._checkCameraTargetLimits(dir, -self._worldsize.x, self._worldsize.x) && self.objects_props.camera.targetMoveCamera) {
                    self._three_camera.position.z += val;
                }
                break;
            case 'y':
                if (flipSignPos) { // Se actualiza el factor dependiendo de la ubicación de la cámara
                    $factor = Math.sign(self._three_camera.position.x);
                }
                val *= $factor;

                // Actualiza el target y la cámara
                if (self._checkCameraTargetCollision(dir, val) && self._checkCameraTargetLimits(dir, -self._worldsize.y, self._worldsize.y) && self.objects_props.camera.targetMoveCamera) {
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
                if (self._checkCameraTargetCollision(dir, val) && self._checkCameraTargetLimits(dir, 0.00001, self._worldsize.z) && self.objects_props.camera.targetMoveCamera) {
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
        self._checkCameraTargetLimits('x', -self._worldsize.x, self._worldsize.x);
        self._checkCameraTargetLimits('y', -self._worldsize.y, self._worldsize.y);

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
        camerafolder.add(this._guiCameraParams, 'maxdistance', 100, 5 * self._worldsize.diagl).onChange(function (val) {
            self._controls.maxDistance = val;
            self._animateFrame();
        });
        camerafolder.add(this._guiCameraParams, 'maxpolarangle', 0, Math.PI).onChange(function (val) {
            self._controls.maxPolarAngle = val;
            self._animateFrame();
        });
        camerafolder.add(this._guiCameraParams, 'posx', 1, 2 * self._worldsize.diagl).onChange(function (val) {
            self._three_camera.position.z = val;
            self._animateFrame();
        }).listen();
        camerafolder.add(this._guiCameraParams, 'posy', 1, 2 * self._worldsize.diagl).onChange(function (val) {
            self._three_camera.position.x = val;
            self._animateFrame();
        }).listen();
        camerafolder.add(this._guiCameraParams, 'posz', 1, 2 * self._worldsize.diagl).onChange(function (val) {
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
        $('#' + this._guiID).append(this._gui.domElement);

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
            $('#' + self._guiID).empty();
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
                let $helpersize = Math.min(self._worldsize.x, self._worldsize.y, self._worldsize.z) * self.threejs_helpers.axissize;
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
                    this._newThreePoint(this._worldsize.x, 0, 0),
                    this._newThreePoint(-this._worldsize.x, 0, 0),
                    this._newThreePoint(-this._worldsize.x, 0, this._worldsize.z),
                    this._newThreePoint(this._worldsize.x, 0, this._worldsize.z)
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
                    this._newThreePoint(0, -this._worldsize.y, 0),
                    this._newThreePoint(0, this._worldsize.y, 0),
                    this._newThreePoint(0, this._worldsize.y, this._worldsize.z),
                    this._newThreePoint(0, -this._worldsize.y, this._worldsize.z)
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
                    this._newThreePoint(this._worldsize.x, this._worldsize.y, 0),
                    this._newThreePoint(-this._worldsize.x, this._worldsize.y, 0),
                    this._newThreePoint(-this._worldsize.x, -this._worldsize.y, 0),
                    this._newThreePoint(this._worldsize.x, -this._worldsize.y, 0)
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
                let $mapsize = 2 * Math.max(this._worldsize.x, this._worldsize.y);
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
                    this._newThreePoint(this._worldsize.x, -this._worldsize.y, 0),
                    this._newThreePoint(this._worldsize.x, this._worldsize.y, 0),
                    this._newThreePoint(this._worldsize.x, this._worldsize.y, this._worldsize.z),
                    this._newThreePoint(this._worldsize.x, -this._worldsize.y, this._worldsize.z),

                    // Plano +y
                    this._newThreePoint(this._worldsize.x, this._worldsize.y, 0),
                    this._newThreePoint(-this._worldsize.x, this._worldsize.y, 0),
                    this._newThreePoint(-this._worldsize.x, this._worldsize.y, this._worldsize.z),
                    this._newThreePoint(this._worldsize.x, this._worldsize.y, this._worldsize.z),

                    // Plano -x
                    this._newThreePoint(-this._worldsize.x, -this._worldsize.y, 0),
                    this._newThreePoint(-this._worldsize.x, this._worldsize.y, 0),
                    this._newThreePoint(-this._worldsize.x, this._worldsize.y, this._worldsize.z),
                    this._newThreePoint(-this._worldsize.x, -this._worldsize.y, this._worldsize.z),

                    // Plano -y
                    this._newThreePoint(this._worldsize.x, -this._worldsize.y, 0),
                    this._newThreePoint(-this._worldsize.x, -this._worldsize.y, 0),
                    this._newThreePoint(-this._worldsize.x, -this._worldsize.y, this._worldsize.z),
                    this._newThreePoint(this._worldsize.x, -this._worldsize.y, this._worldsize.z),

                    // Plano z
                    this._newThreePoint(this._worldsize.x, -this._worldsize.y, this._worldsize.z),
                    this._newThreePoint(this._worldsize.x, this._worldsize.y, this._worldsize.z),
                    this._newThreePoint(-this._worldsize.x, this._worldsize.y, this._worldsize.z),
                    this._newThreePoint(-this._worldsize.x, -this._worldsize.y, this._worldsize.z)
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
                let $targetsize = Math.min(self._worldsize.x, self._worldsize.y, self._worldsize.z) * self.threejs_helpers.cameratargetsize / 2;
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
     * Ejecuta el shader seleccionado desde el menú.
     * @function
     * @since 0.1.6
     */
    this.loadSelectedShader = function () {

        /**
         * Inicia capa de carga
         */
        loadingHandler(true);

        /**
         * Obtiene los archivos del shader
         */
        let $shader = self._shaderSelector.val();
        $shader = shader_lib[$shader];
        app_console.info(lang.loading_shader.format($shader.name));

        /**
         * Carga el shader
         */
        load_shader($shader.files.vert, $shader.files.frag, self._loadSelectedShaderHandler);

    };

    /**
     * Función que recibe los datos cargados de los shaders y ejecuta la función.
     * @param data
     * @private
     */
    this._loadSelectedShaderHandler = function (data) {

        /**
         * Guarda los datos al shader
         */
        self._shaderObject.datashader.f = data.fragment;
        self._shaderObject.datashader.v = data.vertex;

        /**
         * Inicia el shader
         */
        self._initShaderObject();

        /**
         * Desactiva capa de carga
         */
        loadingHandler(false);

    };

    /**
     * Define el selector del shader.
     *
     * @function
     * @param {JQuery<HTMLElement>, HTMLElement} selector - Selector del shader
     * @since 0.1.6
     */
    this.setShaderSelector = function (selector) {
        self._shaderSelector = selector;
    };

    /**
     * Inicia el objeto de dibujo del shader.
     *
     * @function
     * @since 0.1.6
     */
    this._initShaderObject = function () {

        /**
         * Si el objeto ya existía se elimina
         */
        if (notNullUndf(self._shaderObject.quad)) {
            this._scene.remove(self._shaderObject.quad);
        }

        /**
         * Crea la geometría
         */
        self._shaderObject.geometry = new THREE.BufferGeometry();

        /**
         * Inicia los valores iniciales del shader complejo
         */
        self._shaderObject.vertex.zi = new Float32Array(6);
        self._shaderObject.vertex.zr = new Float32Array(6);

        /**
         * Dibuja la figura
         */
        let vertices = new Float32Array(18);
        vertices[0] = -1.0;
        vertices[1] = -1.0;
        vertices[2] = self._shaderObject.plotz;

        vertices[3] = 1.0;
        vertices[4] = 1.0;
        vertices[5] = self._shaderObject.plotz;

        vertices[6] = -1.0;
        vertices[7] = 1.0;
        vertices[8] = self._shaderObject.plotz;

        // Second triangle.
        vertices[9] = -1.0;
        vertices[10] = -1.0;
        vertices[11] = self._shaderObject.plotz;

        vertices[12] = 1.0;
        vertices[13] = 1.0;
        vertices[14] = self._shaderObject.plotz;

        vertices[15] = 1.0;
        vertices[16] = -1.0;
        vertices[17] = self._shaderObject.plotz;

        /**
         * Crea el material
         */
        self._shaderObject.material = new THREE.ShaderMaterial({
            'fragmentShader': self._shaderObject.datashader.f,
            'side': THREE.DoubleSide,
            'uniforms': {
                'max_iterations': {
                    'type': 'i',
                    'value': self._shaderObject.iters,
                }
            },
            'vertexShader': self._shaderObject.datashader.v,
        });

        /**
         * Crea la geometría
         * @type {BufferGeometry}
         */
        self._shaderObject.geometry = new THREE.BufferGeometry();
        self._shaderObject.geometry.addAttribute('position', new THREE.BufferAttribute(vertices, 3));
        self._shaderObject.geometry.addAttribute('vertex_z_r', new THREE.BufferAttribute(self._shaderObject.vertex.zr, 1));
        self._shaderObject.geometry.addAttribute('vertex_z_i', new THREE.BufferAttribute(self._shaderObject.vertex.zi, 1));

        // Rota la geometría para dejarla coplanar
        self._shaderObject.geometry.rotateX(-Math.PI / 2);
        self._shaderObject.geometry.rotateY(-Math.PI / 2);

        /**
         * Crea el mesh
         * @type {Mesh}
         */
        self._shaderObject.quad = new THREE.Mesh(
            self._shaderObject.geometry,
            self._shaderObject.material
        );
        this._scene.add(self._shaderObject.quad);

        /**
         * Define valores iniciales del shader
         * @type {number}
         */
        self._shaderObject.init.range = 2;
        self._shaderObject.init.zi = -0.5;
        self._shaderObject.init.zr = 0;

        /**
         * Dibuja de manera inicial
         */
        self._setPlotBounds(self._shaderObject.init.zi, self._shaderObject.init.zr, self._shaderObject.init.range);

    };

    /**
     * Define los bordes del shader y ejecuta un nuevo cuadro.
     *
     * @function
     * @private
     * @param {number} mid_z_i - Valor medio de Zi
     * @param {number} mid_z_r - Valor medio de zr
     * @param {number} range - Rango del gráfico
     * @since 0.1.6
     */
    this._setPlotBounds = function (mid_z_r, mid_z_i, range) {

        /**
         * Obtiene los atributos del shader para modificarlos
         */
        let z_r = self._shaderObject.quad.geometry.attributes.vertex_z_r.array;
        let z_i = self._shaderObject.quad.geometry.attributes.vertex_z_i.array;

        /**
         * Primer triángulo
         */
        z_r[0] = mid_z_r - range;
        z_i[0] = mid_z_i - range;

        z_r[1] = mid_z_r + range;
        z_i[1] = mid_z_i + range;

        z_r[2] = mid_z_r - range;
        z_i[2] = mid_z_i + range;

        /**
         * Segundo triángulo
         */
        z_r[3] = mid_z_r - range;
        z_i[3] = mid_z_i - range;

        z_r[4] = mid_z_r + range;
        z_i[4] = mid_z_i + range;

        z_r[5] = mid_z_r + range;
        z_i[5] = mid_z_i - range;

        /**
         * Anima un nuevo cuadro
         */
        self._shaderObject.quad.geometry.attributes.vertex_z_r.needsUpdate = true;
        self._shaderObject.quad.geometry.attributes.vertex_z_i.needsUpdate = true;
        this._animateFrame();

    };

}