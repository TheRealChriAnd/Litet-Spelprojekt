#ifndef GRID_H
#define GRID_H
#include <GLM\vec2.hpp>
class Grid
{
private:
	glm::ivec2 m_size;
	int ** m_ppTiles;
public:
	Grid(const glm::ivec2 & size, const int ** tiles);
	Grid(const glm::ivec2 & size = glm::ivec2(0, 0));
	~Grid();

	void edit(const glm::ivec2 & pos, const int & val);
	const int getVal(const glm::ivec2 & pos) const;
};
#endif