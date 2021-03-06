#include "Model.h"


namespace Ryno{

	
	SubModel& Model::add_sub_model()
	{
		sub_models.emplace_back();
		SubModel& s = sub_models.back();
		s.parent = this;
		return s;
	}

	void Model::copy(const Model& copy)
	{
		for (const SubModel& s : copy.sub_models) {
			sub_models.emplace_back(s);
			sub_models.back().parent = this;
		}
	}
	
	Model::Model(const Model& cp) {
		copy(cp);
	}


	SubModel::SubModel(const SubModel& cp)
	{
		copy(cp);
	}

	void SubModel::copy(const SubModel& cp) {
		cast_shadows = cp.cast_shadows;
		parent = cp.parent;
		mesh = cp.mesh;
		material.copy(cp.material);
	}

}