#pragma once
#include "CHeader.h"

static void My_translate(GLuint shaderProgramID, glm::mat4 ModelMatrix, glm::vec3 matrix) {
	ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::translate(ModelMatrix, matrix);

	glUniformMatrix4fv
	(
		glGetUniformLocation(shaderProgramID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix)
	);

	glUniformMatrix3fv
	(
		glGetUniformLocation(shaderProgramID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr
		(
			glm::mat3(glm::transpose(glm::inverse(ModelMatrix)))
		)
	);
}

static void My_rotate(GLuint shaderProgramID, glm::mat4 ModelMatrix, glm::vec3 matrix, GLfloat angle) {
	ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::rotate(ModelMatrix, angle, matrix);

	glUniformMatrix4fv
	(
		glGetUniformLocation(shaderProgramID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix)
	);

	glUniformMatrix3fv
	(
		glGetUniformLocation(shaderProgramID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr
		(
			glm::mat3(glm::transpose(glm::inverse(ModelMatrix)))
		)
	);
}

static void My_scale(GLuint shaderProgramID, glm::mat4 ModelMatrix, glm::vec3 matrix) {
	ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::scale(ModelMatrix, matrix);

	glUniformMatrix4fv
	(
		glGetUniformLocation(shaderProgramID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix)
	);

	glUniformMatrix3fv
	(
		glGetUniformLocation(shaderProgramID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr
		(
			glm::mat3(glm::transpose(glm::inverse(ModelMatrix)))
		)
	);
}