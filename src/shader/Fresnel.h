#ifndef Fresnel_h
#define Fresnel_h

using namespace std;

/**
 * @brief Function that computes the Fresnel Effect
 * 
 * @param beta_1 The delta of the first material
 * @param beta_2 The delta of the second material
 * @param cos_1 The cosine of the angle of the original direction
 * @param cos_2 The cosine of the angle of the refracted ray
 * @return The Fresnel Effect
 */
float compute_fresnel(float beta_1, float beta_2, float cos_1, float cos_2) {
	float part_1 = pow((beta_1 * cos_1 - beta_2 * cos_2) / (beta_1 * cos_1 + beta_2 * cos_2), 2);
	float part_2 = pow((beta_1 * cos_2 - beta_2 * cos_1) / (beta_1 * cos_2 + beta_2 * cos_1), 2);

	return 0.5 * (part_1 + part_2);
}

#endif /* Fresnel_h */
