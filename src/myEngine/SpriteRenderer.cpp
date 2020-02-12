#include <myEngine/myEngine.h>

SpriteRenderer::SpriteRenderer()
{

}
SpriteRenderer::~SpriteRenderer()
{

}

void SpriteRenderer::onInit()
{
	m_va = std::make_shared<VertexArray>();
	m_shaderProgram = getApplication()->GetResourceManager()->LoadFromResources<ShaderProgram>("simple_shader");
}

void SpriteRenderer::onInit(std::shared_ptr<Texture> _tex)
{
	m_va = std::make_shared<VertexArray>();
	m_shaderProgram = getApplication()->GetResourceManager()->LoadFromResources<ShaderProgram>("simple_shader");
	m_tex = _tex;
}

void SpriteRenderer::onInit(std::shared_ptr<ShaderProgram> _shader, std::shared_ptr<Texture> _tex)
{
	m_va = std::make_shared<VertexArray>();
	m_shaderProgram = _shader;
	m_tex = _tex;
}

void SpriteRenderer::onTick()
{
	if (m_shaderProgram)
	{
		m_shaderProgram->SetUniform("in_Projection", getApplication()->GetCamera()->GetProjectionMatrix());
		m_shaderProgram->SetUniform("in_View", getApplication()->GetCamera()->GetViewMatrix());
	}
}

void SpriteRenderer::onDisplay() 
{
	if (m_shaderProgram && m_tex && m_va)
	{
		m_shaderProgram->SetUniform("in_Model", getEntity()->GetTransform()->GetModelMatrix());
		m_shaderProgram->SetUniform("in_Texture", m_tex);
		m_shaderProgram->Draw(m_va);
	}
}

bool SpriteRenderer::Clone(std::shared_ptr<Entity> _entity)
{
	std::shared_ptr<SpriteRenderer> tmp = _entity->addComponent<SpriteRenderer>();
	tmp->SetTexture(m_tex);
	tmp->SetShader(m_shaderProgram);
	return true;
}