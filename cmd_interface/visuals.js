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
