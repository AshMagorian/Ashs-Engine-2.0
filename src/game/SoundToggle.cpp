#include <myEngine/myEngine.h>
#include "SoundToggle.h"

void SoundToggle::onTick()
{
	if (getApplication()->GetInput()->IsKeyPressed(' '))
	{
		getEntity()->GetComponent<SoundComponent>()->SetIsPlaying(true);
	}
}