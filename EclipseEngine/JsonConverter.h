#pragma once
#include "VulkanRenderer.h"

struct JsonConverter
{
	static void to_json(nlohmann::json& json, std::string&string)
	{
		json = string;
	}
	static void from_json(nlohmann::json& json, std::string& string)
	{
		json.get_to(string);
	}
	static void to_json(nlohmann::json& json, std::vector<std::string>& string)
	{
		json = string;
	}
	static void from_json(nlohmann::json& json, std::vector<std::string>& string)
	{
		/*	for (auto& vec2 : numberList)
			{
				json[0].get_to(vec2.x);
				json[1].get_to(vec2.y);
			}*/
	}

	static void to_json(nlohmann::json& json, int number)
	{
		json = number;
	}
	static void from_json(nlohmann::json& json, int number)
	{
		json.get_to(number);
	}
	static void to_json(nlohmann::json& json, std::vector<int>& numberList)
	{
		json = numberList;
	}
	static void from_json(nlohmann::json& json, std::vector<int>& numberList)
	{
		/*	for (auto& vec2 : numberList)
			{
				json[0].get_to(vec2.x);
				json[1].get_to(vec2.y);
			}*/
	}


	static void to_json(nlohmann::json& json, uint32_t number)
	{
		json = number;
	}
	static void from_json(nlohmann::json& json, uint32_t number)
	{
		json.get_to(number);
	}
	static void to_json(nlohmann::json& json, std::vector<uint32_t>& numberList)
	{
		json = numberList;
	}
	static void from_json(nlohmann::json& json, std::vector<uint32_t>& numberList)
	{
		/*	for (auto& vec2 : numberList)
			{
				json[0].get_to(vec2.x);
				json[1].get_to(vec2.y);
			}*/
	}

	static void to_json(nlohmann::json& json, float number)
	{
		json = number;
	}
	static void from_json(nlohmann::json& json, float number)
	{
		json.get_to(number);
	}

	static void to_json(nlohmann::json& json, std::vector<float>& numberList)
	{
		json = numberList;
	}
	static void from_json(nlohmann::json& json, std::vector<float>& numberList)
	{
	/*	for (auto& vec2 : numberList)
		{
			json[0].get_to(vec2.x);
			json[1].get_to(vec2.y);
		}*/
	}

	static void to_json(nlohmann::json& json, glm::vec2& vec2)
	{
		json = { vec2.x, vec2.y };
	}
	static void from_json(nlohmann::json& json, glm::vec2& vec2)
	{
		json[0].get_to(vec2.x);
		json[1].get_to(vec2.y);
	}

	static void to_json(nlohmann::json& json, std::vector<glm::vec2>& vec2List)
	{
		for (auto& vec2 : vec2List)
		{
			json = { vec2.x, vec2.y };
		}
	}
	static void from_json(nlohmann::json& json, std::vector<glm::vec2>& vec2List)
	{
		for (auto& vec2 : vec2List)
		{
			json[0].get_to(vec2.x);
			json[1].get_to(vec2.y);
		}
	}

	static void to_json(nlohmann::json& json, glm::vec3& vec3)
	{
		json = { vec3.x, vec3.y, vec3.z };
	}
	static void from_json(nlohmann::json& json, glm::vec3& vec3)
	{
		json[0].get_to(vec3.x);
		json[1].get_to(vec3.y);
		json[2].get_to(vec3.z);
	}

	static void to_json(nlohmann::json& json, std::vector<glm::vec3>& vec3List)
	{
		for (auto& vec3 : vec3List)
		{
			json = { vec3.x, vec3.y, vec3.z };
		}
	}
	static void from_json(nlohmann::json& json, std::vector<glm::vec3>& vec3List)
	{
		for (auto& vec3 : vec3List)
		{
			json[0].get_to(vec3.x);
			json[1].get_to(vec3.y);
			json[2].get_to(vec3.z);
		}
	}

	static void to_json(nlohmann::json& json, glm::vec4& vec4)
	{
		json = { vec4.x, vec4.y, vec4.z, vec4.w };
	}
	static void from_json(nlohmann::json& json, glm::vec4& vec4)
	{
		json[0].get_to(vec4.x);
		json[1].get_to(vec4.y);
		json[2].get_to(vec4.z);
		json[3].get_to(vec4.w);
	}

	static void to_json(nlohmann::json& json, std::vector<glm::vec4>& vec4List)
	{
		for (auto& vec4 : vec4List)
		{
			json = { vec4.x, vec4.y, vec4.z, vec4.w };
		}
	}
	static void from_json(nlohmann::json& json, std::vector<glm::vec4>& vec4List)
	{
		for (auto& vec4 : vec4List)
		{
			json[0].get_to(vec4.x);
			json[1].get_to(vec4.y);
			json[2].get_to(vec4.z);
			json[3].get_to(vec4.w);
		}
	}

	static void to_json(nlohmann::json& json, glm::ivec2& ivec2)
	{
		json = { ivec2.x, ivec2.y };
	}
	static void from_json(nlohmann::json& json, glm::ivec2& ivec2)
	{
		json[0].get_to(ivec2.x);
		json[1].get_to(ivec2.y);
	}

	static void to_json(nlohmann::json& json, std::vector<glm::ivec2>& ivec2List)
	{
		for (auto& ivec2 : ivec2List)
		{
			json = { ivec2.x, ivec2.y };
		}
	}
	static void from_json(nlohmann::json& json, std::vector<glm::ivec2>& ivec2List)
	{
		for (auto& ivec2 : ivec2List)
		{
			json[0].get_to(ivec2.x);
			json[1].get_to(ivec2.y);
		}
	}

	static void to_json(nlohmann::json& json, glm::ivec3& ivec3)
	{
		json = { ivec3.x, ivec3.y, ivec3.z };
	}
	static void from_json(nlohmann::json& json, glm::ivec3& ivec3)
	{
		json[0].get_to(ivec3.x);
		json[1].get_to(ivec3.y);
		json[2].get_to(ivec3.z);
	}

	static void to_json(nlohmann::json& json, std::vector<glm::ivec3>& ivec3List)
	{
		for (auto& ivec3 : ivec3List)
		{
			json = { ivec3.x, ivec3.y, ivec3.z };
		}
	}
	static void from_json(nlohmann::json& json, std::vector<glm::ivec3>& ivec3List)
	{
		for (auto& ivec3 : ivec3List)
		{
			json[0].get_to(ivec3.x);
			json[1].get_to(ivec3.y);
			json[2].get_to(ivec3.z);
		}
	}

	static void to_json(nlohmann::json& json, glm::ivec4& ivec4)
	{
		json = { ivec4.x, ivec4.y, ivec4.z, ivec4.w };
	}
	static void from_json(nlohmann::json& json, glm::ivec4& ivec4)
	{
		json[0].get_to(ivec4.x);
		json[1].get_to(ivec4.y);
		json[2].get_to(ivec4.z);
		json[3].get_to(ivec4.w);
	}

	static void to_json(nlohmann::json& json, std::vector<glm::ivec4>& ivec4List)
	{
		for (auto& ivec4 : ivec4List)
		{
			json = { ivec4.x, ivec4.y, ivec4.z, ivec4.w };
		}
	}
	static void from_json(nlohmann::json& json,  std::vector<glm::ivec4>& ivec4List)
	{
		for (auto& ivec4 : ivec4List)
		{
			json[0].get_to(ivec4.x);
			json[1].get_to(ivec4.y);
			json[2].get_to(ivec4.z);
			json[3].get_to(ivec4.w);
		}
	}

	static void to_json(nlohmann::json& json, glm::mat2& mat2)
	{
		json = { mat2[0][0], mat2[0][1],
				 mat2[1][0], mat2[1][1]};
	}
	static void from_json(nlohmann::json& json, glm::mat4& mat2)
	{
		json[0].get_to(mat2[0][0]);
		json[1].get_to(mat2[0][1]);

		json[4].get_to(mat2[1][0]);
		json[5].get_to(mat2[1][1]);
	}

	static void to_json(nlohmann::json& json, std::vector<glm::mat2>& mat2List)
	{
		for (auto& mat2 : mat2List)
		{
			json = { mat2[0][0], mat2[0][1],
					 mat2[1][0], mat2[1][1]};
		}
	}
	static void from_json(nlohmann::json& json, std::vector<glm::mat3>& mat2List)
	{
		for (auto& mat2 : mat2List)
		{
			json[0].get_to(mat2[0][0]);
			json[1].get_to(mat2[0][1]);

			json[4].get_to(mat2[1][0]);
			json[5].get_to(mat2[1][1]);
		}
	}

	static void to_json(nlohmann::json& json, glm::mat3& mat3)
	{
		json = { mat3[0][0], mat3[0][1], mat3[0][2],
				 mat3[1][0], mat3[1][1], mat3[1][2],
				 mat3[2][0], mat3[2][1], mat3[2][2]};
	}
	static void from_json(nlohmann::json& json, glm::mat3& mat3)
	{
		json[0].get_to(mat3[0][0]);
		json[1].get_to(mat3[0][1]);
		json[2].get_to(mat3[0][2]);

		json[4].get_to(mat3[1][0]);
		json[5].get_to(mat3[1][1]);
		json[6].get_to(mat3[1][2]);

		json[8].get_to(mat3[2][0]);
		json[9].get_to(mat3[2][1]);
		json[10].get_to(mat3[2][2]);
	}

	static void to_json(nlohmann::json& json, std::vector<glm::mat3>& mat3List)
	{
		for (auto& mat3 : mat3List)
		{
			json = { mat3[0][0], mat3[0][1], mat3[0][2],
					 mat3[1][0], mat3[1][1], mat3[1][2],
					 mat3[2][0], mat3[2][1], mat3[2][2] };
		}
	}
	//static void from_json(nlohmann::json& json, std::vector<glm::mat3>& mat3List)
	//{
	//	for (auto& mat3 : mat3List)
	//	{
	//		json[0].get_to(mat3[0][0]);
	//		json[1].get_to(mat3[0][1]);
	//		json[2].get_to(mat3[0][2]);

	//		json[4].get_to(mat3[1][0]);
	//		json[5].get_to(mat3[1][1]);
	//		json[6].get_to(mat3[1][2]);

	//		json[8].get_to(mat3[2][0]);
	//		json[9].get_to(mat3[2][1]);
	//		json[10].get_to(mat3[2][2]);
	//	}
	//}

	static void to_json(nlohmann::json& json, glm::mat4& mat4)
	{
		json = { mat4[0][0], mat4[0][1], mat4[0][2], mat4[0][3],
			     mat4[1][0], mat4[1][1], mat4[1][2], mat4[1][3],
			     mat4[2][0], mat4[2][1], mat4[2][2], mat4[2][3],
			     mat4[3][0], mat4[3][1],mat4[3][2], mat4[3][3]};
	}
	//static void from_json(nlohmann::json& json, glm::mat4& mat4)
	//{
	//	json[0].get_to(mat4[0][0]);
	//	json[1].get_to(mat4[0][1]);
	//	json[2].get_to(mat4[0][2]);
	//	json[3].get_to(mat4[0][3]);

	//	json[4].get_to(mat4[1][0]);
	//	json[5].get_to(mat4[1][1]);
	//	json[6].get_to(mat4[1][2]);
	//	json[7].get_to(mat4[1][3]);

	//	json[8].get_to(mat4[2][0]);
	//	json[9].get_to(mat4[2][1]);
	//	json[10].get_to(mat4[2][2]);
	//	json[11].get_to(mat4[2][3]);

	//	json[12].get_to(mat4[3][0]);
	//	json[13].get_to(mat4[3][1]);
	//	json[14].get_to(mat4[3][2]);
	//	json[15].get_to(mat4[3][3]);
	//}

	static void to_json(nlohmann::json& json, std::vector<glm::mat4>& mat4List)
	{
		for (auto& mat4 : mat4List)
		{
			json = { mat4[0][0], mat4[0][1], mat4[0][2], mat4[0][3],
					 mat4[1][0], mat4[1][1], mat4[1][2], mat4[1][3],
					 mat4[2][0], mat4[2][1], mat4[2][2], mat4[2][3],
					 mat4[3][0], mat4[3][1],mat4[3][2], mat4[3][3] };
		}
	}
	static void from_json(nlohmann::json& json, std::vector<glm::mat4> mat4List)
	{
		for (auto& mat4 : mat4List)
		{
			json[0].get_to(mat4[0][0]);
			json[1].get_to(mat4[0][1]);
			json[2].get_to(mat4[0][2]);
			json[3].get_to(mat4[0][3]);

			json[4].get_to(mat4[1][0]);
			json[5].get_to(mat4[1][1]);
			json[6].get_to(mat4[1][2]);
			json[7].get_to(mat4[1][3]);

			json[8].get_to(mat4[2][0]);
			json[9].get_to(mat4[2][1]);
			json[10].get_to(mat4[2][2]);
			json[11].get_to(mat4[2][3]);

			json[12].get_to(mat4[3][0]);
			json[13].get_to(mat4[3][1]);
			json[14].get_to(mat4[3][2]);
			json[15].get_to(mat4[3][3]);
		}
	}
};