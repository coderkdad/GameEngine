//#version 460
//#ifdef GL_ES
//// Set default precision to medium
//precision mediump int;
//precision mediump float;
//#endif

uniform sampler2D texture;
varying vec2 v_texcoord;
in vec3 v_color;
out float fragFlag;
out float colorTransparency;
uniform lowp float texturTransparency;

uniform bool renderTexture;
varying vec3 lightIntensity;


// New

uniform highp vec4 u_lightPosition;
uniform highp float u_lightPower;
varying highp vec4 v_vertex;
varying highp vec3 v_normal;



//! [0]
void main()
{


    /// ================== LIGHTING ================== ///
//    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
//    vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0);
//    vec4 diffMatColor = texture2D(texture, v_texcoord);

//    vec3 eyeVect = normalize(v_vertex.xyz - eyePosition.xyz);
//    vec3 lightVect = normalize(v_vertex.xyz - u_lightPosition.xyz);
//    vec3 reflectLight = normalize( reflect(lightVect, v_normal) );

//    float len = length(v_vertex.xyz - eyePosition.xyz);
//    float specularFactor = 60.0;
//    float ambientFactor = 0.1;

//    vec4 diffcolor = diffMatColor * u_lightPower * max(0.0, dot(v_normal, -lightVect) );// / (1.0 + 0.25 * pow(len, 2.0)) ;
//    resultColor += diffcolor ;
//    vec4 ambientColor= ambientFactor * diffMatColor;
//    resultColor += ambientColor;
//    vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0) * u_lightPower * pow( max(0.0, dot(reflectLight, -eyeVect)) , specularFactor);// / (1.0 + 0.25 * pow(len, 2.0)) ;
//    resultColor += specularColor;

//    gl_FragColor = resultColor;

    /// ================================================///





    /// ================ TRUE ============= ///

    vec4 texCol = texture2D(texture,v_texcoord.st);
    if(fragFlag < 0.1)
    {
//        discard;
//        gl_FragColor = vec4(v_color, colorTransparency);
    }
    else
    {
        texCol.a = texturTransparency;
        gl_FragColor = texCol;
    }


//        ///============== الشفافية على لون ==========///
//        /// إذا تريد جعل لون ما شفافي على الصورة
//        gl_FragColor = texCol *vec4(1.0, 0.0, 0.5, 1.0)* texturTransparency;
//        أو
//        gl_FragColor = mix(vec4(v_color,0.0),
//                           texCol,
//                           textureFlag);
//        ///=========================================///


    /// ================ TRUE ================= ///
}
//    gl_FragColor = renderTexture ? vec4(lightIntensity, 1.0) * texCol : vec4(lightIntensity, 1.0);
//    gl_FragColor =texture2D(texture, v_texcoord.st) * varyingColor;    //[USE TEXTURE]
//    gl_FragColor = varyingColor;           //[USE COLOR]
//! [0]
