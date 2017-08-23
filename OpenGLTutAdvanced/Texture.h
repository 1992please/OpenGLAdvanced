#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
public:
	unsigned int ID;
	Texture(const char* texPath);
	void ActivateAs(unsigned int index);
};
#endif // !TEXTURE_H
