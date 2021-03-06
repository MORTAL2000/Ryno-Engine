#include "ParticleContact.h"


namespace Ryno {

	void ParticleContact::resolve(F duration)
	{
		resolve_velocity(duration);
		resolve_interpenetration(duration);
	}

	F ParticleContact::calculate_separating_velocity() const
	{
		//Return the dot between the relative velocity and the contact normal.
		V3 relative_velocity = particles[0]->velocity;
		if (particles[1]) relative_velocity -= particles[1]->velocity;
		
		return dot(relative_velocity, contact_normal);
	}

	void ParticleContact::resolve_velocity(F duration)
	{
		F separating_velocity = calculate_separating_velocity();

		//If the separating velocity is > 0, the objects are not colliding
		if (separating_velocity > 0)
			return;

		//New separating speed after contact
		F new_sep_velocity = -separating_velocity * restitution;

		//***START OF RESTING CONTACT***
		//Calculate acceleraton during contact
		V3 frame_acc = particles[0]->delta_velocity;
		if (particles[1]) frame_acc -= particles[1]->delta_velocity;

		//Velocity generated in the current frame
		F frame_velocity = dot(frame_acc,contact_normal);

		//if the frame velocity is < 0, it means they are colliding (as for sep velocity)
		if (frame_velocity < 0) {

			//Remove the velocity generated in the last frame.
			//This makes contact possible.
			F old_sep_vel = new_sep_velocity;
			new_sep_velocity += frame_velocity * restitution;
			
		
			//Avoid floating errors that makes it negative
			if (new_sep_velocity < 20)
				new_sep_velocity = 0;
		}
		//***END OF RESTING CONTACT***


		F delta_velocity = new_sep_velocity - separating_velocity;

		//Calculate glob (inverse) mass
		F total_inverse_mass = particles[0]->get_inverse_mass();
		if (particles[1]) total_inverse_mass += particles[1]->get_inverse_mass();

		//If both masses are infinite, return
		if (total_inverse_mass <= 0)
			return;

		//Calculate total inpulse (but not the one applied to each particle)
		F total_inpulse = delta_velocity / total_inverse_mass;
		V3 inpulse_with_direction = total_inpulse * contact_normal;

		//Add a portion of this inpulse to each particle (depending on mass).
		//Invert the sign in second particle
		particles[0]->velocity += inpulse_with_direction * particles[0]->get_inverse_mass();
		if (particles[1])
			particles[1]->velocity += -inpulse_with_direction * particles[1]->get_inverse_mass();

	}

	void ParticleContact::resolve_interpenetration(F duration)
	{
		//Not interpenetrating
		if (penetration <= 0)
			return;

		//Total mass
		F total_inverse_mass = particles[0]->get_inverse_mass();
		if (particles[1]) total_inverse_mass += particles[1]->get_inverse_mass();

		//If both infinite return
		if (total_inverse_mass <= 0)
			return;

		
		V3 offset =  penetration * contact_normal / total_inverse_mass;

		//Calculate movements of this frame generated by the resolving of the interpenetration.
		//Cache the results because they are used by the resolver for futher iterations
		particle_movements[0] = offset * particles[0]->get_inverse_mass();

		if (particles[1])
			particle_movements[1] = offset * -particles[1]->get_inverse_mass();
		else
			particle_movements[1] = glm::vec3(0, 0, 0);

		//Each particle is moved by an offset that depends on its mass, and on the total offset
		particles[0]->add_position(particle_movements[0]);

		if (particles[1])
			particles[1]->add_position(particle_movements[1]);
		

	}

}