#pragma once
#include "HouseScene.h"
#include "Text.h"
#include "GPUProfiler.h"
#include <string>
#include <GLM/gtx/transform.hpp>
#include "GJK.h"
#include "Terrain.h"
#include "ValleyScene.h"

namespace Ryno{
	
	void HouseScene::start(){
		

		//initializations
		camera->position = glm::vec4(0, 150, -500, 1);

		//shader
		shader.create("Geometry/geometry",GAME);
		//loading graphics
	
		 bt = game->texture_manager->load_png("pack/161", GAME);
		 bn = game->texture_manager->load_png("pack/161_norm", GAME);
		 solaire = game->texture_manager->load_png("solaire", GAME);
		 sun = game->texture_manager->load_png("sun", GAME);
		 doge = game->texture_manager->load_png("doge", GAME);
		 white = game->texture_manager->load_png("white_pixel", GAME);
		 white_normal = game->texture_manager->load_png("normal_pixel", GAME);


		//loading models
		 sphere_mesh = game->mesh_manager->load_mesh("sphere", 1, GAME);
		 cone_mesh = game->mesh_manager->load_mesh("cone", 1, GAME);
		 cube_mesh = game->mesh_manager->load_mesh("cube", 1, GAME);
		 terrain_mesh = game->mesh_manager->create_empty_mesh(GAME);

		game->mesh_builder->set_mesh(terrain_mesh);
		NewTerrain(game->mesh_builder,80,5,5,50,.01f);


		//loading skyboxes
		camera->skybox = game->texture_manager->load_cube_map("day", GAME);
		////loading audio
		//sound = game->audio_manager->load_sound("stomp.wav", GAME);
		//music = game->audio_manager->load_music("journey.ogg", GAME);
		//sound.set_volume(1.0f);
		//music.set_volume(1.0f);
		//music.play();

		//Environnement

		//Center big Cone
		cones[0].model = new Model();
		cones[0].model->material.set_shader(&shader);
		cones[0].model->material.set_attribute("in_Color", ColorRGBA(255, 255, 255, 0));
		cones[0].model->material.set_attribute("in_Tiling", glm::vec2(1,1));
		cones[0].model->material.set_uniform("texture_sampler", white.id);
		cones[0].model->material.set_uniform("normal_map_sampler", white_normal.id);
		cones[0].model->mesh = cone_mesh;

		cones[0].transform.set_scale(100, 100, 100);
		cones[0].transform.set_position(0, 55, 50);

		//Left small cone
		cones[1].copy(cones[0]);
		cones[1].transform.set_scale(30, 30, 30);
		cones[1].transform.set_position(-150, 20, -170);

		//Right small cone
		cones[2].copy(cones[1]);
		cones[2].transform.set_position(150, 20, -170);

		//Base
		walls[0].copy(cones[2]);
		walls[0].transform.set_scale(1, 1, 1);
		walls[0].model->material.set_uniform("texture_sampler", bt.id);
		walls[0].model->material.set_uniform("normal_map_sampler", bn.id);

		walls[0].model->mesh = terrain_mesh;
		walls[0].model->material.set_attribute("in_Tiling", glm::vec2(3,3));
		walls[0].model->cast_shadows = true;
		walls[0].transform.set_position(5, -15, -5);

		//Left
		walls[1].copy(walls[0]);
		walls[1].model->mesh = cube_mesh;
		walls[1].transform.set_scale(5, 100, 200);
		walls[1].transform.set_position(-200, 105, 0);

		//Right
		walls[2].copy(walls[1]);
		walls[2].transform.set_scale(5, 100, 200);
		walls[2].transform.set_position(200, 105, 0);

		//Front
		walls[3].copy(walls[2]);
		walls[3].transform.set_scale(200, 100, 5);
		walls[3].transform.set_position(0, 105, 200);

		//Roof
		walls[4].copy(walls[3]);
		walls[4].transform.set_scale(200, 5, 200);
		walls[4].transform.set_position(0, 205, 0);

		//Spot light
		//Directional light
		spot_light_shader.create("LightPass/spot", ENGINE);

		spot_light_go.transform.set_position(0, 190, 50);
		spot_light_go.spot_light = new SpotLight();
		auto* s = spot_light_go.spot_light;
		s->model = new Model();
		s->model->material.set_shader(&spot_light_shader);
		s->set_rotation(-90, 0,0);
		s->cutoff = 30;
		s->set_diffuse_color(0, 255, 0);
		s->diffuse_intensity = 30;
		s->attenuation = .001;
		s->specular_intensity = 10;
		s->set_specular_color(0, 255, 0);
		s->absolute_movement = true;


		
		//Point lights
		//Point light 1
		spheres[0].copy(walls[3]);
		spheres[0].model->material.set_attribute("in_Color", ColorRGBA::white);
		spheres[0].model->material.set_uniform("texture_sampler", white.id);
		spheres[0].model->material.set_uniform("normal_map_sampler", white_normal.id);
		spheres[0].model->cast_shadows = false;

		spheres[0].transform.set_scale(12, 12, 12);
		spheres[0].transform.set_position(180, 20, 180);
		spheres[0].model->mesh = sphere_mesh;


		point_light_shader.create("LightPass/point", ENGINE);

		spheres[0].point_light = new PointLight();
		auto* p = spheres[0].point_light;
		p->model = new Model();
		p->model->material.set_shader(&point_light_shader);
		p->set_diffuse_color(255, 80, 0);
		p->diffuse_intensity = 3;
		p->attenuation = .001;
		p->specular_intensity = 10;
		p->set_specular_color(255, 80, 0);

		//Point light 2
		spheres[1].copy(spheres[0]);
		spheres[1].transform.set_position(-180, 20, 180);

		//Point light 3
		spheres[2].copy(spheres[1]);
		spheres[2].transform.set_position(-180, 20, -180);

		//Point light 4
		spheres[3].copy(spheres[2]);
		spheres[3].transform.set_position(180, 20, -180);

		//Directional light
		dir_light_shader.create("LightPass/directional", ENGINE);

	
		directional_light_go.dir_light = new DirectionalLight();
		auto* l = directional_light_go.dir_light;
		l->model = new Model();
		l->model->material.set_shader(&dir_light_shader);
		l->set_rotation(-70, 10,0);
		l->diffuse_intensity = .3f;
		l->set_diffuse_color(255, 255, 200);
		l->specular_intensity = .05;
		l->set_specular_color(255, 255, 200);
		l->ambient_intensity = .0;
		l->set_ambient_color(255, 255, 200);
		l->absolute_movement = true;

		//GUI
		font.create("Aaargh", 24, GAME);
	
		gui.sprite = new Sprite();
		gui.text = new Text();
		auto* sp = gui.sprite;
		auto* t = gui.text;
		sp->depth = 20;
		sp->angle = 0;
		sp->set_color(255, 255, 255, 255);
		sp->set_texture(doge);
		sp->set_position(0.01f, 0.99f);
		sp->anchor_point = TOP_LEFT;
		sp->set_scale(100, 100);
		sp->set_tiling(1, 1);
		sp->angle = 0;
		t->depth = 10;
		t->font = &font;
		t->set_position(1, 1);
		t->anchor_point = TOP_RIGHT;
		t->set_scale(.99f,.99f);
		t->set_color(255, 255, 0, 255);
		t->text = "Anchored text";

		emitter_obj.transform.set_position(0, 105, 50);
		emitter_obj.emitter = new Emitter(&emitter_obj);
		Emitter* emitter = emitter_obj.emitter;
		emitter->save_map.add("texture", &white);
		emitter->save_map.add("normal", &white_normal);
		emitter->save_map.add("mesh", &sphere_mesh);
		emitter->save_map.add("shad", &shader);





		emitter->lambda_creation = [](Emitter* e, Particle3D* p){
			Texture* white, *normal;
			//Emitter* emitter;
			I32* mesh;
			Shader* shad;

			white = e->save_map.get<Texture>("texture");
			normal = e->save_map.get<Texture>("normal");
			mesh = e->save_map.get<I32>("mesh");
			shad = e->save_map.get<Shader>("shad");

			p->decay_rate = .001f;
			p->speed = .05f;
			p->model = new Model();
			p->model->material.set_shader(shad);
			p->model->material.set_uniform("texture_sampler", white->id);
			p->model->material.set_uniform("normal_map_sampler", normal->id);

			p->model->mesh = *mesh;
			p->model->material.set_attribute("in_Color", ColorRGBA::yellow);
			//p->set_emitter(new Emitter(emitter));
		};
		emitter->lambda_spawn = [](Emitter* e){
			for (U8 t = 0; t < 2; t++){
				Particle3D* p = e->new_particle();
				p->transform.set_position(e->game_object->transform.get_position());
				p->direction = ryno_math::get_rand_dir(0, 360, 0, 360);
				//bool b = false;
				//p->get_emitter()->save_map.replace("go_crazy", b);
			}
		};

		emitter->lambda_particle_update = [](Emitter* e, Particle3D* p, float _delta_time)
		{

			//bool f = false;
			//bool t = true;
			//if (p->lifetime < .75f){
			p->transform.add_position(p->direction * p->speed * _delta_time);
			p->transform.set_scale(ryno_math::lerp(glm::vec3(.1), glm::vec3(5), p->lifetime));

			p->model->material.set_attribute("in_Color",ColorRGBA(255, ryno_math::lerp(0, 255, p->lifetime), 0,255));
			//}

			/*if (p->lifetime > .75f && p->lifetime < .82f){
			p->get_emitter()->save_map.replace("go_crazy", t);
			if (p.transform.scale.x > 1)
			p.transform.set_scale(0,0,0);
			}
			else p->get_emitter()->save_map.replace("go_crazy", f);*/

		};

		

		//e2->init(200);
		emitter->init(200);
		


	}


	void HouseScene::update(){

		for (auto & sphere : spheres){
			glm::vec3 m = sphere.transform.get_position();
			m.y = 40 + sin(game->time / 1000.0f) * 20;
			sphere.transform.set_position(m);
		}

	}

	void HouseScene::input(){
		if (!game->shell->active){
			if (game->input_manager->is_key_pressed(SDLK_c, KEYBOARD)){
				game->set_scene("firework");
				return;
			}
		}
	}


}