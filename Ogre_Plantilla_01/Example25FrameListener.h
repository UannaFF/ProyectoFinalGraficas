#pragma once
#include "ogreframelistener.h"
class Example25FrameListener :
	public Ogre::FrameListener
{
public:
	Example25FrameListener(Ogre::SceneNode* node);
	~Example25FrameListener();
	bool frameStarted(const Ogre::FrameEvent &evt);
};

