//#version 460

uniform mat4 mvMatrix;
uniform mat4 pMatrix;
//===New==//
// إذا وضعت الخاصية attribute لابد أن يكون معطى قيمة من خلال الشيدر بفر
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
const int MAX_JOINTS = 20; // أقصى عدد للمفاصل في الهيكل العظمي
const int MAX_WEIGHTS = 4; // أعلى عدد الأوزان والمفاصل المرتبطة بالرؤوس

attribute vec4 a_joints;
attribute vec4 a_Weights;

uniform mat4 jointTransforms[MAX_JOINTS];
    //uniform mat4 projectionViewMatrix;
//========//

//attribute vec2 states;

//attribute float a_trans;
//varying float v_trans;

void main(void)
{


    // Sleleton
    vec4 totalLocalPos = vec4(0.0);
    vec4 totalNormal = vec4(0.0);
    for(int i=0; i<MAX_WEIGHTS; i++)
    {
//        int iii = int(a_joints[i]);
        mat4 jointTransform = jointTransforms[ int(a_joints[i]) ];
        vec4 posePosition = jointTransform * vec4(a_vertex, 1.0);
        totalLocalPos += posePosition * a_Weights[i];

        vec4 worldNormal = jointTransform * vec4(a_normal, 0.0);
        totalNormal += worldNormal * a_Weights[i];
    }
    /// =================================== ///

    //color.a = 0.5;
//        v_trans = a_trans;

    // Diffuse
//        vec3 tnorm = normalize(normalMatrix * vertexNormal);
//        vec4 eyeCoords = modelViewMatrix * vec4(vertex, 1.0);
//        vec3 s = normalize(vec3( vec4(lightPosition, 1.0) - eyeCoords));
//        lightIntensity = lightLd * lightKd * max( dot(s, tnorm), 0.0);


    /// ==================================== ///
    // Biscally

    gl_Position = pMatrix * mvMatrix * vec4(totalLocalPos.xyz, 1.0);
    v_normal = normalize( vec3(mvMatrix * vec4(totalNormal.xyz, 1.0)) );
//    gl_Position = pMatrix * mvMatrix * vec4(a_vertex, 1.0);
//    v_normal = normalize( vec3(mvMatrix * vec4(a_normal, 0.0)) );
    v_texcoord = a_texcoord;
//    v_color = color;

    v_vertex = mvMatrix * (totalLocalPos);
}
