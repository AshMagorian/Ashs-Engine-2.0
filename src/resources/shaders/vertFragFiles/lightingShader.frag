struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

varying vec2 ex_TexCoord;
varying vec3 ex_FragPos;
varying vec3 ex_Normal;

//uniform mat4 in_View;
uniform vec3 in_ViewPos;
uniform DirLight in_DirLight;
uniform Material in_Material;

vec3 CalcDirLight(DirLight _light, vec3 _normal, vec3 _viewDir);

void main()
{
	vec3 norm = normalize(ex_Normal);
	//vec4 viewPos = inverse(in_View) * vec4(0, 0, 0, 1);
	//vec3 viewDir = normalize(vec3(viewPos) - ex_FragPos);
	vec3 viewDir = normalize(in_ViewPos - ex_FragPos);


	vec3 result = CalcDirLight(in_DirLight, norm, viewDir);
	gl_FragColor = vec4(result, 1);
}

vec3 CalcDirLight(DirLight _light, vec3 _normal, vec3 _viewDir)
{
	vec3 lightDir = normalize(-_light.direction);
	// Diffuse shading
	float diff = max(dot(_normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, _normal);
	float spec = pow(max(dot(_viewDir, reflectDir), 0.0), in_Material.shininess);
	// combine results
	vec3 ambient = _light.ambient * vec3(texture(in_Material.diffuse, ex_TexCoord));
	vec3 diffuse = _light.diffuse * diff * vec3(texture(in_Material.diffuse, ex_TexCoord));
	vec3 specular = _light.specular * spec * vec3(texture(in_Material.specular, ex_TexCoord));
	
	return (ambient + diffuse + specular);
}