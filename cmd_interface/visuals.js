// Setup scene
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(70, window.innerWidth/window.innerHeight, 0.1, 1000);
camera.position.z = 5;

const vizContainer = document.getElementById('attitude-viz');
const renderer = new THREE.WebGLRenderer({ antialias: true });
renderer.setSize(vizContainer.clientWidth, vizContainer.clientHeight);
vizContainer.appendChild(renderer.domElement);

// Create a drone cube
const geometry = new THREE.BoxGeometry(1, 0.1, 1); // flat to resemble a quad
const material = new THREE.MeshNormalMaterial({wireframe: false});
const drone = new THREE.Mesh(geometry, material);
scene.add(drone);

// Coordinate axes
const axesHelper = new THREE.AxesHelper(2);
scene.add(axesHelper);

// Animation loop
function animate() {
    requestAnimationFrame(animate);
    renderer.render(scene, camera);
}
animate();

// Helper: Convert degrees to radians
function deg2rad(deg) {
    return deg * Math.PI / 180;
}

// Hook this into your telemetry handler
function updateDroneOrientation(roll, pitch, yaw) {
    // Three.js uses intrinsic Tait-Bryan angles: ZYX (yaw, pitch, roll)
    drone.rotation.set(
        deg2rad(-pitch),
        deg2rad(yaw),
        deg2rad(-roll)
    );
}

function createEscGauge(id) {
  return new RadialGauge({
    renderTo: id,
    width: 150, height: 150,
    units: '', title: '',
    minValue: 0,
    maxValue: 1.0,       // ESC throttle normalized
    majorTicks: ['0','0.25','0.5','0.75','1.0'],
    minorTicks: 4,
    needle: true,
    animatedValue: true,
    animationRule: 'linear',
    animationDuration: 100,
    colorPlate: '#222',
    colorNeedle: '#ff5722',
    colorNumbers: '#ddd',
    valueBox: false
  }).draw();
}

const gauges = {
  bl: createEscGauge('esc-gauge-bl'),
  br: createEscGauge('esc-gauge-br'),
  fl: createEscGauge('esc-gauge-fl'),
  fr: createEscGauge('esc-gauge-fr')
};

// Call this in your telemetry update handler:
function updateEscGauges(values) {
  // values should be normalized throttle 0.0–1.0
  gauges.bl.value = values.m_bl;
  gauges.br.value = values.m_br;
  gauges.fl.value = values.m_fl;
  gauges.fr.value = values.m_fr;
}
