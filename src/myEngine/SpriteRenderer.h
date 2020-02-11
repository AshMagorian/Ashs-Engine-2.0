#include <myEngine/myEngine.h>

class SpriteRenderer : public Component
{
private:
	std::shared_ptr<ShaderProgram> m_shaderProgram; ///< The shader program which will be used to draw the sprite
	std::shared_ptr<VertexArray> m_va;
public:

};