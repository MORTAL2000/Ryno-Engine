#pragma once
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"
#include "DeferredRenderer.h"
#include <Sprite.h>
#include "Emitter.h"
#include "Global.h"
#include "Scene.h"
#include "Game.h"
#include "ParticleForceRegister.h"



namespace RynoEngine{
	class PhysicsScene : public Scene
	{

	public:
		PhysicsScene() : Scene(){}
	private:
	
		void start() override;
		void input() override;
		void update() override;
	
		I32 mesh;
		Shader shader, dir_light_shader;
		Texture white, white_normal;
	
		GameObject ball,ball2;

		ParticleForceRegister reg;
		ParticleGravity* gravity_force;
		ParticleSpring* spring_force;
		ParticleBuoyancy* buoyancy_force;

	};
}
