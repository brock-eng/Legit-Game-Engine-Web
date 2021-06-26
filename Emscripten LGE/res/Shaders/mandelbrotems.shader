attribute highp vec4 position;
attribute highp vec2 texCoords;
attribute highp vec4 color;
attribute highp float textureID;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix;
uniform mat4 ml_matrix;


varying highp vec4 vs_position;
varying highp vec2 vs_texCoords;
varying highp vec4 vs_color;
varying highp float vs_textureID;

void main()
{
   vec4 calc_position = pr_matrix * position;
   gl_Position = calc_position;
   vs_position = calc_position; 
   vs_texCoords = texCoords;
   vs_color = color;
   vs_textureID = textureID;
};

/* --------------------------------------------------------------- */




precision highp float;

varying highp vec4  vs_position;
varying highp vec2  vs_texCoords;
varying highp vec4  vs_color;
varying highp float vs_textureID;


uniform highp vec4 color;
uniform highp vec4 colour;
uniform highp int numIterations;


uniform highp mat4 uMVPMatrix;// = mat4(1.0);

void main() {

	vec2 p = (uMVPMatrix * vec4(vs_position.x, vs_position.y, 0, 1)).xy;
	vec2 c = p;

	vec3 color = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < numIterations; i++) {
		p = vec2(p.x * p.x - p.y * p.y, 2.0 * p.x * p.y) + c;
		if (dot(p, p) > 4.0) {
			float colorRegulator = float(i - 1) - log(((log(dot(p, p))) / log(2.0))) / log(2.0);
			color = vec3(0.95 + .012 * colorRegulator, 1.0, .2 + .4 * (1.0 + sin(.3 * colorRegulator)));
			break;
		}
	}
	// change color from HSV to RGB. Found: https://gist.github.com/patriciogonzalezvivo/114c1653de9e3da6e1e3
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 m = abs(fract(color.xxx + K.xyz) * 6.0 - K.www);
	gl_FragColor.rgb = color.z * mix(K.xxx, clamp(m - K.xxx, 0.0, 1.0), color.y);

	gl_FragColor.a = 1.0;
}

