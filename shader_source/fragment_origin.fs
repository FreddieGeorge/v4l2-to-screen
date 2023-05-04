#extension GL_OES_EGL_image_external : require
precision mediump float;
uniform samplerExternalOES uTexSampler;

varying vec2 vTexCoord;
void main()
{
	vec4 temp = texture2D(uTexSampler, vTexCoord);
	gl_FragColor = vec4(temp.xyz, 1.0);
}
