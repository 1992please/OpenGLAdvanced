#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
class Image
{
public:
	unsigned int ID;
	Image(const char* texPath);
	void ActivateAs(unsigned int index);
};
#endif // !TEXTURE_H
