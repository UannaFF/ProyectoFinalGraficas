#include "Example25FrameListener.h"

private:
	Ogre::SceneNode* _node;
Example25FrameListener::Example25FrameListener(Ogre::SceneNode* node)
{
	_node = node;
}

bool frameStarted(const Ogre::FrameEvent &evt)
{
	_node->translate(Ogre::Vector3(0.1,0,0));
	return true;
}


Example25FrameListener::~Example25FrameListener(void)
{
}
