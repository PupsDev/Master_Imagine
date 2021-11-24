function createShader(gl, type, source) {
    var shader = gl.createShader(type);
    gl.shaderSource(shader, source);

    gl.compileShader(shader);
    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        throw new Error(gl.getShaderInfoLog(shader));
    }

    return shader;
}

function createProgram(gl, vertexSource, fragmentSource) {
    var program = gl.createProgram();

    var vertexShader = createShader(gl, gl.VERTEX_SHADER, vertexSource);
    var fragmentShader = createShader(gl, gl.FRAGMENT_SHADER, fragmentSource);

    gl.attachShader(program, vertexShader);
    gl.attachShader(program, fragmentShader);

    gl.linkProgram(program);
    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
        throw new Error(gl.getProgramInfoLog(program));
    }

    var wrapper = {program: program};

    var numAttributes = gl.getProgramParameter(program, gl.ACTIVE_ATTRIBUTES);
    for (var i = 0; i < numAttributes; i++) {
        var attribute = gl.getActiveAttrib(program, i);
        wrapper[attribute.name] = gl.getAttribLocation(program, attribute.name);
    }
    var numUniforms = gl.getProgramParameter(program, gl.ACTIVE_UNIFORMS);
    for (var i$1 = 0; i$1 < numUniforms; i$1++) {
        var uniform = gl.getActiveUniform(program, i$1);
        wrapper[uniform.name] = gl.getUniformLocation(program, uniform.name);
    }

    return wrapper;
}

function createTexture(gl, filter, data, width, height) {
    var texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, texture);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, filter);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, filter);
    if (data instanceof Uint8Array) {
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, width, height, 0, gl.RGBA, gl.UNSIGNED_BYTE, data);
    } else {
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, data);
    }
    gl.bindTexture(gl.TEXTURE_2D, null);
    return texture;
}

function bindTexture(gl, texture, unit) {
    gl.activeTexture(gl.TEXTURE0 + unit);
    gl.bindTexture(gl.TEXTURE_2D, texture);
}

function createBuffer(gl, data) {
    var buffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    gl.bufferData(gl.ARRAY_BUFFER, data, gl.STATIC_DRAW);
    return buffer;
}

function bindAttribute(gl, buffer, attribute, numComponents) {
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    gl.enableVertexAttribArray(attribute);
    gl.vertexAttribPointer(attribute, numComponents, gl.FLOAT, false, 0, 0);
}

function bindFramebuffer(gl, framebuffer, texture) {
    gl.bindFramebuffer(gl.FRAMEBUFFER, framebuffer);
    if (texture) {
        gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, texture, 0);
    }
}

var drawVert = "precision mediump float;\n\nattribute float a_index;\n\nuniform sampler2D u_particles;\nuniform float u_particles_res;\n\nvarying vec2 v_particle_pos;\n\nvoid main() {\n    vec4 color = texture2D(u_particles, vec2(\n        fract(a_index / u_particles_res),\n        floor(a_index / u_particles_res) / u_particles_res));\n\n    // decode current particle position from the pixel's RGBA value\n    v_particle_pos = vec2(\n        color.r / 255.0 + color.b,\n        color.g / 255.0 + color.a);\n\n    gl_PointSize = 1.0;\n    gl_Position = vec4(2.0 * v_particle_pos.x - 1.0, 1.0 - 2.0 * v_particle_pos.y, 0, 1);\n}\n";

var drawFrag = "precision mediump float;\n\nuniform sampler2D u_wind;\nuniform vec2 u_wind_min;\nuniform vec2 u_wind_max;\nuniform sampler2D u_color_ramp;\n\nvarying vec2 v_particle_pos;\n\nvoid main() {\n    vec2 velocity = mix(u_wind_min, u_wind_max, texture2D(u_wind, v_particle_pos).rg);\n    float speed_t = length(velocity) / length(u_wind_max);\n\n    // color ramp is encoded in a 16x16 texture\n    vec2 ramp_pos = vec2(\n        fract(16.0 * speed_t),\n        floor(16.0 * speed_t) / 16.0);\n\n    gl_FragColor = texture2D(u_color_ramp, ramp_pos);\n}\n";

var quadVert = "precision mediump float;\n\nattribute vec2 a_pos;\n\nvarying vec2 v_tex_pos;\n\nvoid main() {\n    v_tex_pos = a_pos;\n    gl_Position = vec4(1.0 - 2.0 * a_pos, 0, 1);\n}\n";

var screenFrag = "precision mediump float;\n\nuniform sampler2D u_screen;\nuniform float u_opacity;\n\nvarying vec2 v_tex_pos;\n\nvoid main() {\n    vec4 color = texture2D(u_screen, 1.0 - v_tex_pos);\n    // a hack to guarantee opacity fade out even with a value close to 1.0\n    gl_FragColor = vec4(floor(255.0 * color * u_opacity) / 255.0);\n}\n";

var updateFrag = "precision highp float;\n\nuniform sampler2D u_particles;\nuniform sampler2D u_wind;\nuniform vec2 u_wind_res;\nuniform vec2 u_wind_min;\nuniform vec2 u_wind_max;\nuniform float u_rand_seed;\nuniform float u_speed_factor;\nuniform float u_drop_rate;\nuniform float u_drop_rate_bump;\n\nvarying vec2 v_tex_pos;\n\n// pseudo-random generator\nconst vec3 rand_constants = vec3(12.9898, 78.233, 4375.85453);\nfloat rand(const vec2 co) {\n    float t = dot(rand_constants.xy, co);\n    return fract(sin(t) * (rand_constants.z + t));\n}\n\n// wind speed lookup; use manual bilinear filtering based on 4 adjacent pixels for smooth interpolation\nvec2 lookup_wind(const vec2 uv) {\n    // return texture2D(u_wind, uv).rg; // lower-res hardware filtering\n    vec2 px = 1.0 / u_wind_res;\n    vec2 vc = (floor(uv * u_wind_res)) * px;\n    vec2 f = fract(uv * u_wind_res);\n    vec2 tl = texture2D(u_wind, vc).rg;\n    vec2 tr = texture2D(u_wind, vc + vec2(px.x, 0)).rg;\n    vec2 bl = texture2D(u_wind, vc + vec2(0, px.y)).rg;\n    vec2 br = texture2D(u_wind, vc + px).rg;\n    return mix(mix(tl, tr, f.x), mix(bl, br, f.x), f.y);\n}\n\nvoid main() {\n    vec4 color = texture2D(u_particles, v_tex_pos);\n    vec2 pos = vec2(\n        color.r / 255.0 + color.b,\n        color.g / 255.0 + color.a); // decode particle position from pixel RGBA\n\n    vec2 velocity = mix(u_wind_min, u_wind_max, lookup_wind(pos));\n    float speed_t = length(velocity) / length(u_wind_max);\n\n    // take EPSG:4236 distortion into account for calculating where the particle moved\n    float distortion = cos(radians(pos.y * 180.0 - 90.0));\n    vec2 offset = vec2(velocity.x / distortion, -velocity.y) * 0.0001 * u_speed_factor;\n\n    // update particle position, wrapping around the date line\n    pos = fract(1.0 + pos + offset);\n\n    // a random seed to use for the particle drop\n    vec2 seed = (pos + v_tex_pos) * u_rand_seed;\n\n    // drop rate is a chance a particle will restart at random position, to avoid degeneration\n    float drop_rate = u_drop_rate + speed_t * u_drop_rate_bump;\n    float drop = step(1.0 - drop_rate, rand(seed));\n\n    vec2 random_pos = vec2(\n        rand(seed + 1.3),\n        rand(seed + 2.1));\n    pos = mix(pos, random_pos, drop);\n\n    // encode the new particle position back into RGBA\n    gl_FragColor = vec4(\n        fract(pos * 255.0),\n        floor(pos * 255.0) / 255.0);\n}\n";

var defaultRampColors = {
    0.0: "#762a83",
    0.1: "#9970ab",
    0.2: "#c2a5cf",
    0.3: "#e7d4e8",
    0.4: "#d9f0d3",
    0.5: "#a6dba0",
    0.6: "#5aae61",
    1.0: "#1b7837"
    
};
