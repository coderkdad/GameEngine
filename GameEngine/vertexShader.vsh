//#version 460

uniform mat4 mvMatrix;
uniform mat4 pMatrix;
//===New==//

attribute vec3 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
vec3 color;

varying vec4 v_vertex;
varying vec3 v_normal;
varying vec2 v_texcoord;
//varying vec3 v_color;

varying vec3 lightIntensity;
uniform vec3 lightPosition;
uniform vec3 lightKd;
uniform vec3 lightLd;

//========//

//attribute vec2 states;

//attribute float a_trans;
//varying float v_trans;

void main(void)
{

    //color.a = 0.5;
//    v_color = color;
//        v_trans = a_trans;

    // Diffuse
//        vec3 tnorm = normalize(normalMatrix * vertexNormal);
//        vec4 eyeCoords = modelViewMatrix * vec4(vertex, 1.0);
//        vec3 s = normalize(vec3( vec4(lightPosition, 1.0) - eyeCoords));
//        lightIntensity = lightLd * lightKd * max( dot(s, tnorm), 0.0);

    gl_Position = pMatrix * mvMatrix * vec4(a_vertex, 1.0);

    v_texcoord = a_texcoord;
    v_normal = normalize( vec3(mvMatrix * vec4(a_normal, 0.0)) );
    v_vertex = mvMatrix * vec4(a_vertex, 1.0);
}
