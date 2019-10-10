#pragma once

#include "ShaderFlat.h"

class CScene;

class CShaderPhong : public CShaderFlat
{
public:
	/**
	 * @brief Constructor
	 * @param scene Reference to the scene
	 * @param color The color of the object
	 * @param ka The ambient coefficient
	 * @param kd The diffuse reflection coefficients
	 * @param ks The specular refelection coefficients
	 * @param ke The shininess exponent
	 */
	CShaderPhong(CScene& scene, Vec3f color, float ka, float kd, float ks, float ke)
		: CShaderFlat(color)
		, m_scene(scene)
		, m_ka(ka)
		, m_kd(kd)
		, m_ks(ks)
		, m_ke(ke)
	{}
	virtual ~CShaderPhong(void) = default;

	virtual Vec3f Shade(const Ray& ray) const override
	{
		// --- PUT YOUR CODE HERE ---

		int n = 1; // change this to 1000 for the last task. Thank you 

		Vec3f cd 	= CShaderFlat::Shade(ray);
		Vec3f diff  = 0;
		Vec3f spec  = 0;
		Vec3f N 	= ray.hit->GetNormal(ray);
		
		Ray light_ray;
		light_ray.org 	= ray.org + ray.t * ray.dir;
		
		for(auto light : m_scene.m_vpLights)
			for(int i = 0; i < n; i ++){

				auto Li = light->Illuminate(light_ray);
				if(!Li || m_scene.Occluded(light_ray))
					continue;

				Vec3f I 	= light_ray.dir;
				float IdotN = I.dot(N);
				Vec3f R  	= 2 * IdotN * N - I;

				diff += *Li * std::max(0.f, IdotN);
				spec += *Li * std::pow(std::max(0.f, R.dot(-ray.dir)), m_ke);
			}

		return  m_ka * cd * 1.1 + 
				m_kd * cd.mul(diff / n) + 
				m_ks * RGB(1, 1, 1).mul(spec / n);
	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
