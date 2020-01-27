#include "Component.h"
#include "glm/glm.hpp"

#include <iostream>


class ShaderProgram;
class VertexArray;
class Texture;
class Camera;
class Material;
/**
*Renderer is a component which stores the data necessary to draw an object to the screen
*/
class Renderer : public Component
{
private:

	std::shared_ptr<ShaderProgram> m_shaderProgram; ///< The shader program which will be used to draw the model
	std::shared_ptr<VertexArray> m_va; ///< The model to be drawn
	//std::shared_ptr<Texture> m_tex; ///< the texture of the model
	std::shared_ptr<Material> m_material; ///< the material of the model
	std::shared_ptr<Camera> m_mainCamera; ///< The main camera of the scene

public:
	Renderer();
	~Renderer();
	/**
	*\brief A blank init for if you don't want to set the attributes straight away
	*/
	void onInit() {}
	/**
	*\brief Stores the 3 main attributes for the Renderer component when initialised
	*/
	void onInit(std::shared_ptr<ShaderProgram> _shader, std::shared_ptr<VertexArray> _va, std::shared_ptr<Material> _mat);
	/**
	*\brief Updates the uniforms for the ShaderProgram every frame
	*/
	void onTick();
	/**
	*\brief Draws the model and texture using the shader program
	*/
	void onDisplay();

	/**
	*\brief Sets the model
	*/
	void SetMesh(std::shared_ptr<VertexArray> _va) { m_va = _va; }
	/**
	*\brief Sets the texture
	*/
	//void SetTexture(std::shared_ptr<Texture> _tex) { m_tex = _tex; }

	/**
	*\brief Sets the material
	*/
	void SetMaterial(std::shared_ptr<Material> _mat) { m_material = _mat; }
	/**
	*\brief Sets the shader
	*/
	void SetShader(std::shared_ptr<ShaderProgram> _shader) { m_shaderProgram = _shader; }

	std::shared_ptr<Material> GetMaterial() { return m_material; }
};