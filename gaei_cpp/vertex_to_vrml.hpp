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
    /// ˆê‚Â‚Ì`Shape`‚É‚È‚é‚æ‚¤‚È’¸“_‚ÌW‡‚Ì“ü—Í‚ğó‚¯æ‚é
    /// </summary>
	void push(const std::vector<vertex<vec3f, color>>& vertexes);
    /// <summary>
    /// VRML‚ğo—Í‚·‚é
    /// </summary>
	bool write(std::ostream& out) const;
};

}
