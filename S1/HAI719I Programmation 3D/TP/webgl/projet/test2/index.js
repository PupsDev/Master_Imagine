 
var canvas = document.getElementById('canvas');  

const pxRatio = Math.max(Math.floor(window.devicePixelRatio) || 1, 2);
canvas.width = canvas.clientWidth;
canvas.height = canvas.clientHeight;

const gl = canvas.getContext('webgl', {antialiasing: false});

fadeOpacity = 0.996;  
speedFactor = 0.25;  
dropRate = 0.003;  
dropRateBump = 0.01;  
numParticles = 65536;

drawProgram = createProgram(gl, drawVert, drawFrag);
screenProgram = createProgram(gl, quadVert, screenFrag);
updateProgram = createProgram(gl, quadVert, updateFrag);

quadBuffer = createBuffer(gl, new Float32Array([0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1]));
framebuffer = gl.createFramebuffer();
let windData = ""
     
    var particleRes = particleStateResolution = Math.ceil(Math.sqrt(numParticles));
    _numParticles = particleRes * particleRes;

    var particleState = new Uint8Array(_numParticles * 4);
    for (var i = 0; i < particleState.length; i++) {
        particleState[i] = Math.floor(Math.random() * 256);  
    }
     
    particleStateTexture0 = createTexture(gl, gl.NEAREST, particleState, particleRes, particleRes);
    particleStateTexture1 = createTexture(gl, gl.NEAREST, particleState, particleRes, particleRes);

    var particleIndices = new Float32Array(_numParticles);
    for (var i$1 = 0; i$1 < _numParticles; i$1++) { particleIndices[i$1] = i$1; }
    particleIndexBuffer = createBuffer(gl, particleIndices);


function resize () {

    var emptyPixels = new Uint8Array(canvas.width * canvas.height * 4);
     
    backgroundTexture = createTexture(gl, gl.NEAREST, emptyPixels, gl.canvas.width, gl.canvas.height);
    screenTexture = createTexture(gl, gl.NEAREST, emptyPixels, gl.canvas.width, gl.canvas.height);
};

function setColorRamp (colors) {
     
    colorRampTexture = createTexture(gl, gl.LINEAR, getColorRamp(colors), 16, 16);
};


function setWind (windJson, windImage) {
    windData = windJson;
    windTexture = createTexture(gl, gl.LINEAR, windImage);
};
function draw () {
    gl.disable(gl.DEPTH_TEST);
    gl.disable(gl.STENCIL_TEST);

    bindTexture(gl, windTexture, 0);
    bindTexture(gl, particleStateTexture0, 1);

    drawScreen();
    updateParticles();
};
 function drawScreen () {

     
    bindFramebuffer(gl, framebuffer, screenTexture);
    gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);

    drawTexture(backgroundTexture, fadeOpacity);
    drawParticles();

    bindFramebuffer(gl, null);
     
    gl.enable(gl.BLEND);
    gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
    drawTexture(screenTexture, 1.0);
    gl.disable(gl.BLEND);

     
    var temp = backgroundTexture;
    backgroundTexture = screenTexture;
    screenTexture = temp;
};
function drawTexture (texture, opacity) {

    var program = screenProgram;
    gl.useProgram(program.program);

    bindAttribute(gl, quadBuffer, program.a_pos, 2);
    bindTexture(gl, texture, 2);
    gl.uniform1i(program.u_screen, 2);
    gl.uniform1f(program.u_opacity, opacity);

    gl.drawArrays(gl.TRIANGLES, 0, 6);
};
function drawParticles () {

    var program = drawProgram;
    gl.useProgram(program.program);

    bindAttribute(gl, particleIndexBuffer, program.a_index, 1);
    bindTexture(gl, colorRampTexture, 2);

    gl.uniform1i(program.u_wind, 0);
    gl.uniform1i(program.u_particles, 1);
    gl.uniform1i(program.u_color_ramp, 2);

    gl.uniform1f(program.u_particles_res, particleStateResolution);
    gl.uniform2f(program.u_wind_min, windData.uMin, windData.vMin);
    gl.uniform2f(program.u_wind_max, windData.uMax, windData.vMax);

    gl.drawArrays(gl.POINTS, 0, _numParticles);
};

function updateParticles () {
    bindFramebuffer(gl, framebuffer, particleStateTexture1);
    gl.viewport(0, 0, particleStateResolution, particleStateResolution);

    var program = updateProgram;
    gl.useProgram(program.program);

    bindAttribute(gl, quadBuffer, program.a_pos, 2);

    gl.uniform1i(program.u_wind, 0);
    gl.uniform1i(program.u_particles, 1);

    gl.uniform1f(program.u_rand_seed, Math.random());
    gl.uniform2f(program.u_wind_res, windData.width, windData.height);
    gl.uniform2f(program.u_wind_min, windData.uMin, windData.vMin);
    gl.uniform2f(program.u_wind_max, windData.uMax, windData.vMax);
    gl.uniform1f(program.u_speed_factor, speedFactor);
    gl.uniform1f(program.u_drop_rate, dropRate);
    gl.uniform1f(program.u_drop_rate_bump, dropRateBump);

    gl.drawArrays(gl.TRIANGLES, 0, 6);

     
    var temp = particleStateTexture0;
    particleStateTexture0 = particleStateTexture1;
    particleStateTexture1 = temp;
};

function getColorRamp(colors) {
    var canvas = document.createElement('canvas');
    var ctx = canvas.getContext('2d');

    canvas.width = 256;
    canvas.height = 1;

    var gradient = ctx.createLinearGradient(0, 0, 256, 0);
    for (var stop in colors) {
        gradient.addColorStop(+stop, colors[stop]);
    }

    ctx.fillStyle = gradient;
    ctx.fillRect(0, 0, 256, 1);

    return new Uint8Array(ctx.getImageData(0, 0, 256, 1).data);
}



function frame() {
    if (windData) {
        draw();
    }
    requestAnimationFrame(frame);
}
setColorRamp(defaultRampColors);
resize();
const windImage = new Image();
windImage.src = "png.png";
windJson = {
  "source": "http://nomads.ncep.noaa.gov",
  "date": "2021-1-1T00:00Z",
  "width": 360,
  "height": 180,
  "uMin": -17.6,
  "uMax": 21.225,
  "vMin": -20.575,
  "vMax": 16.375
}
 windImage.onload = function () {
    setWind(windJson,windImage);
};
frame();



getJSON('https://d2ad6b4ur7yvpq.cloudfront.net/naturalearth-3.3.0/ne_110m_coastline.geojson', function (data) {
    const canvas = document.getElementById('coastline');
    canvas.width = canvas.clientWidth * pxRatio;
    canvas.height = canvas.clientHeight * pxRatio;

    const ctx = canvas.getContext('2d');
    ctx.lineWidth = pxRatio;
    ctx.lineJoin = ctx.lineCap = 'round';
    ctx.strokeStyle = 'white';
    ctx.beginPath();

    for (let i = 0; i < data.features.length; i++) {
        const line = data.features[i].geometry.coordinates;
        for (let j = 0; j < line.length; j++) {
            ctx[j ? 'lineTo' : 'moveTo'](
                (line[j][0] + 180) * canvas.width / 360,
                (-line[j][1] + 90) * canvas.height / 180);
        }
    }
    ctx.stroke();
});

function getJSON(url, callback) {
    const xhr = new XMLHttpRequest();
    xhr.responseType = 'json';
    xhr.open('get', url, true);
    xhr.onload = function () {
        if (xhr.status >= 200 && xhr.status < 300) {
            callback(xhr.response);
        } else {
            throw new Error(xhr.statusText);
        }
    };
    xhr.send();
}
