#include <myEngine/myEngine.h>
/**
*A simple component which increments the rotation every frame
*/
class SpinComponent : public Component
{
private:

	float m_spinSpeed = 50;
	std::shared_ptr<Transform> m_transform;

public:
	void SetSpinSpeed(float _value) { m_spinSpeed = _value; }

	void onBegin();
	void onTick();

};