#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "Component.h"

class ShaderProgram;
class VertexArray;
class Texture;

class SpriteRenderer : public Component
{
private:
	std::shared_ptr<ShaderProgram> m_shaderProgram; ///< The shader program which will be used to draw the sprite
	std::shared_ptr<VertexArray> m_va;
	std::shared_ptr<Texture> m_tex;
public:
	SpriteRenderer();
	~SpriteRenderer();
	/**
	*\brief Sets the texture
	*/
	void SetTexture(std::shared_ptr<Texture> _tex) { m_tex = _tex; }
	/**
	*\brief Sets the shader
	*/
	void SetShader(std::shared_ptr<ShaderProgram> _shader) { m_shaderProgram = _shader; }

	void onInit();
	void onInit(std::shared_ptr<Texture> _tex);
	void onInit(std::shared_ptr<ShaderProgram> _shader, std::shared_ptr<Texture> _tex);
	void onTick();
	void onDisplay();

	bool Clone(std::shared_ptr<Entity> _entity);
};
#endif