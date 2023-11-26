#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 vNormal;

out vec3 FragPos; //--- 객체의 위치값을 프래그먼트 세이더로 보낸다.
out vec3 Normal;

uniform mat4 modelMatrix;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;
uniform mat3 normalMatrix;

void main()
{
	gl_Position = projectionTransform * viewTransform * modelMatrix * vec4(in_Position, 1.f);

	FragPos = vec3(modelMatrix * vec4(in_Position, 1.0));		//--- 객체에 대한 조명 계산을 프래그먼트 셰이더에서 한다. 
																//--- 따라서 월드공간에 있는 버텍스 값을 프래그먼트 셰이더로 보낸다.
	Normal = normalMatrix * vNormal;							//--- 노멀값을 프래그먼트 세이더로 보낸다
}