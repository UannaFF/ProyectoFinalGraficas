#include "Ogre\ExampleApplication.h"

class Example1 : public ExampleApplication
{
public:


	void createCamera() {

		mCamera = mSceneMgr->createCamera("MyCamera1");
		mCamera->setPosition(60,10,60);
		mCamera->lookAt(0,0,0);
		mCamera->setNearClipDistance(5);

	}

	void createScene()
	{ 

		mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));
		Ogre::Entity* entMesh01 = mSceneMgr->createEntity("MyEntity1","Sinbad.mesh");
		Ogre::SceneNode* nodeMesh01 = mSceneMgr->createSceneNode("NodeMesh01");
		mSceneMgr->getRootSceneNode()->addChild(nodeMesh01);
		nodeMesh01->setPosition(10.0,10.0,0.0);
		nodeMesh01->attachObject(entMesh01);

		Ogre::SceneNode* nodeMesh02 = mSceneMgr->createSceneNode("NodeMesh02");
		Ogre::Entity* entMesh02 = mSceneMgr->createEntity("MyEntity2","ninja.mesh");
		nodeMesh01->addChild(nodeMesh02);
		nodeMesh02->setPosition(10.0,0.0,0.0);
		nodeMesh02->setScale(0.02f,0.02f,0.02f);
		nodeMesh02->attachObject(entMesh02);

	}

};


int main (void)
{

	Example1 app;
	app.go();
	return 0;
}