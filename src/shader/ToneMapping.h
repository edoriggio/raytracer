#include "../../lib/glm.hpp"

#ifndef ToneMapping_h
#define ToneMapping_h

/**
 * @brief Function that performs the tone mapping of a color
 * 
 * @param intensity Input color
 * @return Tone mapped color
 */
glm::vec3 toneMapping(glm::vec3 intensity) {
	glm::vec3 alpha(10.0);
	glm::vec3 beta(3.0);
	glm::vec3 gamma(3.0);

	glm::vec3 tone_mapped = glm::pow(alpha * glm::pow(intensity, beta), glm::vec3(1.0) / gamma);
	
	return glm::clamp(tone_mapped, glm::vec3(0.0), glm::vec3(1.0));
}

#endif /* ToneMapping_h */
