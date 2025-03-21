#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model
{
private:
	std::vector<glm::vec3> verts_;
	std::vector<std::vector<int>> faces_;

public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	glm::vec3 vert(int i);
	std::vector<int> face(int idx);
};

#endif //__MODEL_H__