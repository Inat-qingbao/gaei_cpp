#include <vector>
#include <iostream>
#include <fstream>

#include "vertex.hpp"
#include "color.hpp"

namespace gaei {
// insert code here

class vertex_to_vrml {
public:
    /// <summary>
    /// ���`Shape`�ɂȂ�悤�Ȓ��_�̏W���̓��͂��󂯎��
    /// </summary>
	void push(const std::vector<vertex<vec3f, color>>& vertexes);
    /// <summary>
    /// VRML���o�͂���
    /// </summary>
	bool write(std::ostream& out) const;
};

}
