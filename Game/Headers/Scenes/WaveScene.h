#pragma once
#include "Lights/DirectionalLight.h"
#include "DeferredRenderer.h"
#include <Sprite.h>
#include "Scene.h"
#include "Game.h"


namespace Ryno {

	class WaveScene : public Scene
	{

	public:
		WaveScene() : Scene() {}
	private:
		void start() override;
		void update() override;

		I32 mesh,sphere;
		Shader shader;
		Texture white, white_normal;
		F32 lato = 110;
		F32 size = 3;
		std::vector<GameObject> cubes;
		GameObject t;
	};
}

