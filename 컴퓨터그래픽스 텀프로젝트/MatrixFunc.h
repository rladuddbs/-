#pragma once
#include "CHeader.h"


void translate(GLuint shaderProgramID, glm::mat4 ModelMatrix, glm::vec3 matrix) {
	ModelMatrix = glm::translate(ModelMatrix, matrix);

	glUniformMatrix4fv
	(
		glGetUniformLocation(shaderProgramID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix)
	);
}

void rotate(GLuint shaderProgramID, glm::mat4 ModelMatrix, glm::vec3 matrix, GLfloat angle) {
	ModelMatrix = glm::rotate(ModelMatrix, angle, matrix);

	glUniformMatrix4fv
	(
		glGetUniformLocation(shaderProgramID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix)
	);
}

void scail(GLuint shaderProgramID, glm::mat4 ModelMatrix, glm::vec3 matrix) {
	ModelMatrix = glm::scale(ModelMatrix, matrix);

	glUniformMatrix4fv
	(
		glGetUniformLocation(shaderProgramID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix)
	);
}