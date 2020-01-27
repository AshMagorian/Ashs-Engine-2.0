#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Material.h"
#include "Entity.h"
#include "Transform.h"
#include "Camera.h"
#include "Application.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}
/**
*\brief Stores the 3 main attributes for the Renderer component when initialised
*/
void Renderer::onInit(std::shared_ptr<ShaderProgram> _shader, std::shared_ptr<VertexArray> _va, std::shared_ptr<Material> _mat)
{
	m_shaderProgram = _shader;
	m_va = _va;
	m_material = _mat;
}
/**
*\brief Updates the uniforms for the ShaderProgram every frame using data from teh main camera(Only if the shader program exists)
*/
void Renderer::onTick()
{
	if (m_shaderProgram)
	{
		m_shaderProgram->SetUniform("in_Projection", getApplication()->GetCamera()->GetProjectionMatrix());
		m_shaderProgram->SetUniform("in_View", getApplication()->GetCamera()->GetViewMatrix());
	}
}
/**
*\brief Draws the model and texture using the shader program (Only if all 3 attributes exist)
*/
void Renderer::onDisplay()
{
	if (m_shaderProgram && m_material && m_va)
	{
		m_shaderProgram->SetUniform("in_Model", getEntity()->GetTransform()->GetModelMatrix());
		//m_shaderProgram->SetUniform("in_Texture", m_material->GetDiffuse());
		m_shaderProgram->SetUniform("in_Material.diffuse", m_material->GetDiffuse());
		m_shaderProgram->SetUniform("in_Material.specular", m_material->GetSpecular());
		m_shaderProgram->SetUniform("in_Material.shininess", m_material->GetShininess());
		m_shaderProgram->Draw(m_va);
	}
}
