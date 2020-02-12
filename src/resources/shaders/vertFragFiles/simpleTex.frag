uniform sampler2D in_Texture;

varying vec2 ex_TexCoord;

void main()
{
	vec4 tex = texture2D(in_Texture, ex_TexCoord);
    	if(tex.a < 0.1) {discard;}
    	if(tex.a > 0.1) {tex.a = 0.5f;}
	gl_FragColor = tex;
}