#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/glm.hpp>

class Texture;

class Material
{
public:
	Material();
	~Material();
	struct ColorChannel
	{
		Texture* mTexture;
		glm::vec3 mColor;
		void LoadTexture(const char* TexPath);
		void LoadColor(float* Color);
		ColorChannel();
		~ColorChannel();
	};

	// Data
	ColorChannel mDiffuse;
	ColorChannel mSpecular;
	float mShininess;
};


#endif // !MATERIAL_H
