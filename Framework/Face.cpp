#include "Face.h";

Face::Face(unsigned short vert1, unsigned short vert2, unsigned short vert3) : vert1(vert1), vert2(vert2), vert3(vert3)
{
};

std::ostream& operator<<(std::ostream &stream, const Face &face)
{
	return stream << face.vert1 << " " << face.vert2 << " " << face.vert3;
}