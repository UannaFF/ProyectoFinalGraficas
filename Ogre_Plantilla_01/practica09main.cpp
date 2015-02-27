#include "stdafx.h"

class FrameListenerProyectos : public Ogre::FrameListener
{
public:
	//Se hace apenas comienza el frame
	bool frameStarted(const Ogre::FrameEvent& evt) {

		return true;
	}

	//Se hace cuando se termina el frame
	bool frameEnded(const Ogre::FrameEvent& evt) {

		return true;
	}
};

class OgreRecursosPropios
{
private:
	Ogre::SceneManager* _sceneManager;
	Ogre::Root* _root;
	FrameListenerProyectos* _listener;

public:

	OgreRecursosPropios() {
		_sceneManager = NULL;
		_root = NULL;
		_listener = NULL;
	}
	~OgreRecursosPropios() {
		delete _sceneManager;
		delete _root;
		delete _listener;
	}
	void createCamera() {

		mCamera = mSceneMgr->createCamera("MyCamera1");
		mCamera->setPosition(14500,50,-19000);
		mCamera->lookAt(0,0,0);
		mCamera->setNearClipDistance(5);

	}

	void createScene()
	{
		_sceneManager->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));
		Ogre::SceneNode* nM01 = _sceneManager->createSceneNode("nM01");
		Ogre::Entity* entMesh01 = _sceneManager->createEntity("entMesh01", "proyectoOgreI.mesh");
		_sceneManager->getRootSceneNode()->addChild(nM01);
		nM01->attachObject(entMesh01);

		Ogre::SceneNode* nM02 = _sceneManager->createSceneNode("ejes");
		Ogre::Entity* entMesh02 = _sceneManager->createEntity("entMesh02", "ejes01.mesh");
		_sceneManager->getRootSceneNode()->addChild(nM02);
		nM02->attachObject(entMesh02);
		entMesh02->scale(10.0f,10.0f,10.0f);

		Ogre::SceneNode* nM03 = _sceneManager->createSceneNode("nM03");
		Ogre::Entity* entMesh03 = _sceneManager->createEntity("Entnm03", "ogrehead.mesh");
		_sceneManager->getRootSceneNode()->addChild(nM03);
		nM02->attachObject(entMesh03);
		entMesh02->scale(5.0f,5.0f,5.0f);
	}

	void startup() {
		_listener = new FrameListenerProyectos;
		_root->addFrameListener(_listener);
	}

};


int main (void)
{

	OgreRecursosPropios app;
	app.go();
	return 0;
}