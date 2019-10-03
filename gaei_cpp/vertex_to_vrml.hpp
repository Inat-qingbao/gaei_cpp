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
    /// 一つの`Shape`になるような頂点の集合の入力を受け取る
    /// </summary>
	void push(const std::vector<vertex<vec3f, color>>& vertexes);
    /// <summary>
    /// VRMLを出力する
    /// </summary>
	bool write(std::ostream& out) const;
};

}
