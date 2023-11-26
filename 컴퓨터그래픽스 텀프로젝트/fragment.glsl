#version 330 core

in vec3 FragPos;			
in vec3 Normal;				

out vec4 FragColor;			

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform vec3 viewPos;

void main()
{
	float ambientLight = 0.3;

	vec3 ambient = ambientLight * lightColor;
	vec3 nomalVector = normalize (Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diffuseLight = max(dot(nomalVector, lightDir), 0.0);
	vec3 diffuse = diffuseLight * lightColor;

	int shininess = 32; //--- 광택 계수
	vec3 viewDir = normalize(viewPos - FragPos); //--- 관찰자의 방향
	vec3 reflectDir = reflect(-lightDir, nomalVector); //--- 반사 방향: reflect 함수 - 입사 벡터의 반사 방향 계산
	float specularLight = max (dot (viewDir, reflectDir), 0.0); //--- V와 R의 내적값으로 강도 조절: 음수 방지
	specularLight = pow(specularLight, shininess); //--- shininess 승을 해주어 하이라이트를 만들어준다.
	vec3 specular = specularLight * lightColor; //--- 거울 반사 조명값: 거울반사값 * 조명색상값
	vec3 result = (ambient + diffuse + specular) * objectColor; //--- 최종 조명 설정된 픽셀 색상: (주변+산란반사+거울반사조명)*객체 색상

	FragColor = vec4(result, 1.0);
}