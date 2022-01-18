#version 330 core

out vec4 FragColor;

in vec3 o_color;
in vec2 TexCoord;
in vec3 Fpos;
in vec3 Fnor;

uniform sampler2D texColor;
uniform sampler2D texMetal;
uniform sampler2D texNormal;
uniform sampler2D texRough;

uniform float roughness;
uniform float metalness;
uniform vec3 albedo;
uniform vec3 lightPos;

const float PI = 3.141592653589;

float distributionGGX(float NdotH, float roughness)
{
		float a = roughness*roughness;
		float a2 = a*a;

		float denom = NdotH*NdotH*(a2-1.)+1.;
		denom = PI * denom * denom;
		return a2 / max (denom, 0.000001);

}
float geometrySmith(float NdotV, float NdotL, float roughness)
{
	float r = roughness +1.;
	float k = (r*r)/8.;
	float ggx1 = NdotV/(NdotV * (1. -k) +k);
	float ggx2 = NdotL/(NdotL * (1. -k) +k);
	return ggx1*ggx2;

}
vec3 fresnelSchlick(float HdotV, vec3 baseReflectivity)
{
	return baseReflectivity + (1. - baseReflectivity) * pow(1. -HdotV, 5.);
}
void main()
{
	float roughness=texture(texRough, TexCoord).r;
	float metalness=texture(texMetal, TexCoord).r;
	vec3 albedo=texture(texColor, TexCoord).rgb;
	


	vec3 camPos = vec3(0.,0.,0.);
	vec3 worldPos =  Fpos;

	vec3 N = texture(texNormal, TexCoord).bgr *2 -1.;// -1.;

	N =  normalize(N);
	vec3 V = normalize(camPos - worldPos);

	vec3 baseReflectivity = mix(vec3(0.04), albedo,metalness);
	vec3 Lo = vec3(0.);

	vec3 L = normalize(lightPos-worldPos);
	vec3 H = normalize(V+L);

	/*
	Blinn phong
	float spec = pow(max(dot(N, H), 0.0), 16.);
	vec3 specular = albedo * spec;
	FragColor = vec4(specular,1.);
	*/

	float distance = length(lightPos-worldPos);
	float attenuation = 1./(distance*distance);
	vec3 radiance = vec3(1.,1.,1.)*attenuation;


	float NdotV = max(dot(N, V), 0.0000001);
	float NdotL = max(dot(N, L), 0.0000001);
	float HdotV = max(dot(H, V), 0.);
	float NdotH = max(dot(N, H), 0.);



	float D = distributionGGX(NdotH, roughness);
	float G = geometrySmith(NdotV,NdotL, roughness);
	vec3 F = fresnelSchlick(HdotV, baseReflectivity);

	vec3 specular = D*G*F;
	specular /= 4. * NdotV * NdotL;

	vec3 kD = vec3(1.)-F;
	kD *= 1.- metalness;

	Lo += (kD * albedo / PI + specular)*radiance*NdotL;

	vec3 ambient = vec3(0.03)* albedo;
	vec3 color  = ambient + Lo;


	//HDR tpne
	color = color /(color + vec3(1.));

	// gamma
	color = pow(color, vec3(1./2.2));

    FragColor = vec4(color,1.);

}
