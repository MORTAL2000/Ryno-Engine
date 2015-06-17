#pragma once
#include "Types.h"
#include "Model.h"

namespace Ryno{

	class GameObject{
	public:
		GameObject(){}
		~GameObject(){}

		Model* model;
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::mat4 model_matrix;
		void generate_model_matrix();
	};

}