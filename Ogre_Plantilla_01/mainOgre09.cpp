#include "stdafx.h"

   Ogre::SceneNode* nM03;

class FrameListenerProyectos : public Ogre::FrameListener
{

private:
	
	Ogre::SceneNode* nodoF01;
	OIS::InputManager* _man;
	OIS::Keyboard* _key;
	OIS::Mouse* _mouse;
	Ogre::Camera* _cam;

public: 

	FrameListenerProyectos(Ogre::SceneNode* nodo01, Ogre::RenderWindow* win, Ogre::Camera* Cam) {
	
		//Conf captura teclado y mouse
		size_t windowHnd = 0;
		std::stringstream windowsHndStr;
		win->getCustomAttribute("WINDOW", &windowHnd);
		windowsHndStr << windowHnd;

		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"), windowsHndStr.str()));

		//Eventos
		_man = OIS::InputManager::createInputSystem(pl);
		_key = static_cast<OIS::Keyboard*>(_man->createInputObject(OIS::OISKeyboard,false));
		_mouse = static_cast<OIS::Mouse*>(_man->createInputObject(OIS::OISMouse,false));
		// Fin eventos
		
		_cam = Cam;
		nodoF01 = nodo01;
	}

	~FrameListenerProyectos(){
		_man->destroyInputObject(_key);
		_man->destroyInputObject(_mouse);
		OIS::InputManager::destroyInputSystem(_man);
	}

	bool frameStarted(const Ogre::FrameEvent& evt) 
	{

		_key->capture();
		_mouse->capture();
		
		float movSpeed = 100.0f;

		Ogre::Vector3 t(0,0,0);

		if (_key->isKeyDown(OIS::KC_ESCAPE))
			return false;

		if (_key->isKeyDown(OIS::KC_T))
			t += Ogre::Vector3(0,0,-10);

		if (_key->isKeyDown(OIS::KC_G))
			t += Ogre::Vector3(0,0,10);

		if (_key->isKeyDown(OIS::KC_F))
			t += Ogre::Vector3(-10,0,0);

		if (_key->isKeyDown(OIS::KC_H))
			t += Ogre::Vector3(10,0,0);

		Ogre::Vector3 tNave(0,0,0);

		if (_key->isKeyDown(OIS::KC_W))
			tNave += Ogre::Vector3(0,0,-10);

		if (_key->isKeyDown(OIS::KC_S))
			tNave += Ogre::Vector3(0,0,10);

		if (_key->isKeyDown(OIS::KC_A))
			tNave += Ogre::Vector3(-10,0,0);

		if (_key->isKeyDown(OIS::KC_D))
			tNave += Ogre::Vector3(10,0,0);
		
		if (_key->isKeyDown(OIS::KC_UP))
			tNave += Ogre::Vector3(0,10,0);

		if (_key->isKeyDown(OIS::KC_DOWN))
			tNave += Ogre::Vector3(0,-10,0);

		nodoF01->translate(tNave * evt.timeSinceLastFrame * movSpeed);

		float rotX = _mouse->getMouseState().X.rel * evt.timeSinceLastFrame * -1;
		float rotY = _mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;
		_cam->yaw(Ogre::Radian(rotX));
		_cam->pitch(Ogre::Radian(rotY));
		_cam->moveRelative(t*evt.timeSinceLastFrame*movSpeed);

		return true;
	}

	bool frameEnded(const Ogre::FrameEvent& evt) 
	{
		return true;
	}

};

class OgreProyectos {

private:
	Ogre::SceneManager* _sceneManager;
	Ogre::Root* _root;
	FrameListenerProyectos* _listener;

public:

	OgreProyectos(){
		_sceneManager = NULL;
		_listener = NULL;
		_root = NULL;
	}


	~OgreProyectos(){ 
		delete _root;
		delete _listener;
	}

	void loadResources() {
		Ogre::ConfigFile cf;
		cf.load("recursos_propios.cfg");

		Ogre::ConfigFile::SectionIterator sectionIter = cf.getSectionIterator();
		Ogre::String sectionName, typeName, dataname;
		while (sectionIter.hasMoreElements()) {
			sectionName = sectionIter.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = sectionIter.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); i++) {
				typeName = i->first;
				dataname = i->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(dataname,typeName,sectionName);

			}
			Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		}
	}
	
	int startup() {

			_root = new Ogre::Root("plugins_d.cfg");
			if (!_root->showConfigDialog()) {
				return -1;
			}

			Ogre::RenderWindow* window = _root->initialise(true,"Ventana Ogre");

			_sceneManager = _root->createSceneManager(Ogre::ST_GENERIC);

			Ogre::Camera* camera = _sceneManager->createCamera("Camera");
			camera->setPosition(Ogre::Vector3(500,100,500));
			camera->lookAt(Ogre::Vector3(0,0,0));
			camera->setNearClipDistance(5);

			Ogre::Viewport* viewport = window->addViewport(camera);
			viewport->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));
			camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()/viewport->getActualHeight()));

		nM03 = _sceneManager->createSceneNode("nM03");

			_listener = new FrameListenerProyectos(nM03,window,camera);
			_root->addFrameListener(_listener);

			loadResources();
			createScene();
			_root->startRendering();

			return 0;
	}

	void createScene() {
	
		_sceneManager->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));

		//Luz
		float lightScale = 0.9f;
		Ogre::Light* light02;
		//Ogre::SceneNode* nodeLuz02 = mSceneMgr->createSceneNode("NodeLuz02");	
		//Ogre::Entity* LightEnt = mSceneMgr->createEntity("MyEntity","sphere.mesh");
		//Ogre::SceneNode* node3 = nodeLuz02->createChildSceneNode("node3");
		//node3->setScale(0.1f,0.1f,0.1f);
		//node3->setPosition(0,20,0);
		//_sceneManager->getRootSceneNode()->addChild(nodeLuz02);
		//Probando LUX
		//node3->attachObject(LightEnt);

		Ogre::SceneNode* nM01 = _sceneManager->createSceneNode("nM01");
		Ogre::Entity* entM01 = _sceneManager->createEntity("EntM01","proyectoOgreI.mesh");
		_sceneManager->getRootSceneNode()->addChild(nM01);
		nM01->attachObject(entM01);
		entM01->setMaterialName("mat01");

		Ogre::SceneNode* nM02 = _sceneManager->createSceneNode("nM02");
		Ogre::Entity* entM02 = _sceneManager->createEntity("EntM02","ejes01.mesh");
		_sceneManager->getRootSceneNode()->addChild(nM02);
		nM02->attachObject(entM02);
		nM02->scale(10.0f,10.0f,10.0f);

		
		/*Ogre::Entity* entM03 = _sceneManager->createEntity("EntM03","poly01.mesh");
		_sceneManager->getRootSceneNode()->addChild(nM03);
		nM03->attachObject(entM03);
		nM03->scale(1.0f,1.0f,1.0f);
		entM03->setMaterialName("MyMaterialNave1");*/

	}

};

int main(void)
{

	OgreProyectos app;
	app.startup();
	return 0;

}