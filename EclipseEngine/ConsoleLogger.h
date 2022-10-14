#pragma once
#include <iostream>
#include <glm/glm.hpp>

class ConsoleLogger
{
public:
	static void MatrixLogger(const std::string& name, glm::mat2& matrix)
	{
		std::cout << name << std::endl;
		std::cout << matrix[0][0] << " " << matrix[0][1] << std::endl;
		std::cout << matrix[1][0] << " " << matrix[1][1] << std::endl;
		std::cout << std::endl << std::endl;
	}

	static void MatrixLogger(const std::string& name, glm::mat3& matrix)
	{
		std::cout << name << std::endl;
		std::cout << matrix[0][0] << " " << matrix[0][1] << " " << matrix[0][2] << std::endl;
		std::cout << matrix[1][0] << " " << matrix[1][1] << " " << matrix[1][2] << std::endl;
		std::cout << matrix[2][0] << " " << matrix[2][1] << " " << matrix[2][2] << std::endl;
		std::cout << std::endl << std::endl;
	}

	static void MatrixLogger(const std::string& name, glm::mat4& matrix)
	{
		std::cout << name << std::endl;
		std::cout << matrix[0][0] << " " << matrix[0][1] << " " << matrix[0][2] << " " << matrix[0][3] << std::endl;
		std::cout << matrix[1][0] << " " << matrix[1][1] << " " << matrix[1][2] << " " << matrix[1][3] << std::endl;
		std::cout << matrix[2][0] << " " << matrix[2][1] << " " << matrix[2][2] << " " << matrix[2][3] << std::endl;
		std::cout << matrix[3][0] << " " << matrix[3][1] << " " << matrix[3][2] << " " << matrix[3][3] << std::endl;
		std::cout << std::endl << std::endl;
	}

	static void MatrixLogger(const std::string& name, const glm::mat2& matrix)
	{
		std::cout << name << std::endl;
		std::cout << matrix[0][0] << " " << matrix[0][1] << std::endl;
		std::cout << matrix[1][0] << " " << matrix[1][1] << std::endl;
		std::cout << std::endl << std::endl;
	}

	static void MatrixLogger(const std::string& name, const glm::mat3& matrix)
	{
		std::cout << name << std::endl;
		std::cout << matrix[0][0] << " " << matrix[0][1] << " " << matrix[0][2] << std::endl;
		std::cout << matrix[1][0] << " " << matrix[1][1] << " " << matrix[1][2] << std::endl;
		std::cout << matrix[2][0] << " " << matrix[2][1] << " " << matrix[2][2] << std::endl;
		std::cout << std::endl << std::endl;
	}

	static void MatrixLogger(const std::string& name, const glm::mat4& matrix)
	{
		std::cout << name << std::endl;
		std::cout << matrix[0][0] << " " << matrix[0][1] << " " << matrix[0][2] << " " << matrix[0][3] << std::endl;
		std::cout << matrix[1][0] << " " << matrix[1][1] << " " << matrix[1][2] << " " << matrix[1][3] << std::endl;
		std::cout << matrix[2][0] << " " << matrix[2][1] << " " << matrix[2][2] << " " << matrix[2][3] << std::endl;
		std::cout << matrix[3][0] << " " << matrix[3][1] << " " << matrix[3][2] << " " << matrix[3][3] << std::endl;
		std::cout << std::endl << std::endl;
	}
};