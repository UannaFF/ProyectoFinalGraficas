#include "stdafx.h"
	Ogre::SceneNode* nave, *camera_node;

class FrameListenerNave : public Ogre::FrameListener {

private:
	OIS::InputManager* _man;
	OIS::Keyboard* _key;
	float grados;

public:
	FrameListenerNave(Ogre::SceneNode* node, Ogre::RenderWindow* win, Ogre::SceneNode* Cam){
		// Interaccion teclado
		size_t windowHnd = 0;
		std::stringstream windowsHndStr;
		win->getCustomAttribute("WINDOW", &windowHnd);
		windowsHndStr << windowHnd;
		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"), windowsHndStr.str()));
		_man = OIS::InputManager::createInputSystem(pl);
		_key = static_cast<OIS::Keyboard*>(_man->createInputObject(OIS::OISKeyboard,false));
		
		nave = node;
		camera_node = Cam;
		grados = 0.0f;
	}

	~FrameListenerNave(){
		_man->destroyInputObject(_key);
		OIS::InputManager::destroyInputSystem(_man);
	}

	bool frameStarted(const Ogre::FrameEvent &evt){
		float movSpeed = 2500.0f;
		_key->capture();
		
		if (_key->isKeyDown(OIS::KC_ESCAPE))
			return false;

		Ogre::Vector3 t (0,0,0);
		Ogre::Quaternion q (Ogre::Degree(0), Ogre::Vector3::ZERO);

		if (_key->isKeyDown(OIS::KC_W))
			t += Ogre::Vector3(0,0,-movSpeed);

		if (_key->isKeyDown(OIS::KC_S))
			t += Ogre::Vector3(0,0,+movSpeed);

		if(_key->isKeyDown(OIS::KC_D) || _key->isKeyDown(OIS::KC_A)){
			if (_key->isKeyDown(OIS::KC_A)){
				t += Ogre::Vector3(-movSpeed,0,0);
				if (grados > -45)
				{
					grados -= 90 * evt.timeSinceLastFrame;
					q = q * Ogre::Quaternion(Ogre::Degree(-90 * evt.timeSinceLastFrame), Ogre::Vector3::UNIT_X);
				}
			}
			if (_key->isKeyDown(OIS::KC_D)){
				t += Ogre::Vector3(movSpeed,0,0);
				if (grados < 45)
				{
					grados += 90 * evt.timeSinceLastFrame;
					q = q * Ogre::Quaternion(Ogre::Degree(90 * evt.timeSinceLastFrame), Ogre::Vector3::UNIT_X);
				}
			}
		} else
		{
			if (grados > 0.1f)
			{
				grados -= 90 * evt.timeSinceLastFrame;
				q = q * Ogre::Quaternion(Ogre::Degree(-90 * evt.timeSinceLastFrame), Ogre::Vector3::UNIT_X);
			} else if (grados < -0.001f){
				grados += 90 * evt.timeSinceLastFrame;
				q = q * Ogre::Quaternion(Ogre::Degree(90 * evt.timeSinceLastFrame), Ogre::Vector3::UNIT_X);
			}
		}
		if (_key->isKeyDown(OIS::KC_UP))
			t += Ogre::Vector3(0,movSpeed,0);

		if (_key->isKeyDown(OIS::KC_DOWN))
			t += Ogre::Vector3(0,-movSpeed,0);

		nave->rotate(q);
		camera_node->translate(t * evt.timeSinceLastFrame, camera_node->TS_LOCAL);
		

		// Limites del cuarto
		Ogre::Vector3 pos = camera_node->getPosition();
		if (pos.x > 9500)
			pos.x = 9500;
		else if(pos.x < -9500)
			pos.x = -9500;

		if (pos.z > 45000)
			pos.z = 45000;
		else if(pos.z < -45000)
			pos.z = -45000;

		if (pos.y > 10000)
			pos.y = 10000;
		else if(pos.y < 10)
			pos.y = 100;

		camera_node->setPosition(pos);
		return true;
	}
};

class FrameListenerProyectos : public Ogre::FrameListener
{

private:

	Ogre::SceneNode* nodoF01;
	Ogre::SceneNode* nodoF02;
	OIS::InputManager* _man;
	OIS::Keyboard* _key;
	OIS::Mouse* _mouse;
	Ogre::Camera* _cam;

	//Animacion
	Ogre::Entity* _ent;
	Ogre::AnimationState* _aniState;
	Ogre::AnimationState* _aniStateTop;

	float _WalkingSpeed;
	float _rotation;

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

		std::cout << "Entro a crear el framelistener" << std::endl;
	}

	~FrameListenerProyectos(){
		_man->destroyInputObject(_key);
		_man->destroyInputObject(_mouse);
		OIS::InputManager::destroyInputSystem(_man);
	}

	bool frameStarted(const Ogre::FrameEvent& evt) {
		_key->capture();
		_mouse->capture();
		
		float movSpeed = 500.0f;


		if (_key->isKeyDown(OIS::KC_ESCAPE))
			return false;

		Ogre::Vector3 t(0,0,0);

		if (_key->isKeyDown(OIS::KC_W))
			t += Ogre::Vector3(0,0,-10);

		if (_key->isKeyDown(OIS::KC_S))
			t += Ogre::Vector3(0,0,10);

		if (_key->isKeyDown(OIS::KC_A))
			t += Ogre::Vector3(-10,0,0);

		if (_key->isKeyDown(OIS::KC_D))
			t += Ogre::Vector3(10,0,0);

		_cam->moveRelative(t*evt.timeSinceLastFrame*movSpeed);
		float rotX = _mouse->getMouseState().X.rel * evt.timeSinceLastFrame* -1;
		float rotY = _mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;
		_cam->yaw(Ogre::Radian(rotX));
		_cam->pitch(Ogre::Radian(rotY));
		
		return true;
	}

	bool frameEnded(const Ogre::FrameEvent& evt) 
	{
		return true;
	}

};

class OgreProyectos {

public:
	Ogre::SceneManager* _sceneManager;
	Ogre::Root* _root;
	Ogre::Camera* camera;
	Ogre::SceneNode* nM03;
	Ogre::RenderWindow* window;
	FrameListenerNave* FrameListenerNave01;
	
	OgreProyectos(){
		_sceneManager = NULL;
		_root = NULL;
		FrameListenerNave01 = NULL;
	}


	~OgreProyectos(){ 
		delete _root;
		delete FrameListenerNave01;
	}

	void createCamera() {
		camera = _sceneManager->createCamera("Camera");
		camera->setPosition(0,150,900);
		camera->lookAt(Ogre::Vector3(0,0,0));
		camera->setNearClipDistance(5);
	}

	void createFrameListeners() {
		FrameListenerNave01 = new FrameListenerNave(nave, window, camera_node);
		_root->addFrameListener(FrameListenerNave01);
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

			window = _root->initialise(true,"Ventana Ogre");

			_sceneManager = _root->createSceneManager(Ogre::ST_GENERIC);
			createCamera();
			Ogre::Viewport* viewport = window->addViewport(camera);
			viewport->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));
			camera->setAspectRatio(Ogre::Real(viewport->getActualWidth()/viewport->getActualHeight()));

			nM03 = _sceneManager->createSceneNode("nM03");
			createFrameListeners();
			//_listener = new FrameListenerProyectos(nM03,window,camera);
			//_root->addFrameListener(_listener);

			loadResources();
			createScene();
			_root->startRendering();

			return 0;
	}

	void createScene() {
	
		//_sceneManager->setSkyBox(true, "SkyBox");
		_sceneManager->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));
		Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0.0);
		Ogre::MeshManager::getSingleton().createPlane("plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 20000, 90000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
		Ogre::Entity* entPlano = _sceneManager->createEntity("PlanoEntity", "plane");
		_sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(entPlano);
		entPlano->setMaterialName("mat01");

		//Creando camara
		camera_node = _sceneManager->createSceneNode("Camera");
		camera_node->attachObject(camera);
		camera_node->setPosition(0,0,45000);
		_sceneManager->getRootSceneNode()->addChild(camera_node);

		//Creando nave
		nave = camera_node->createChildSceneNode("Nave");
		Ogre::Entity* naveMesh = _sceneManager->createEntity("nav","poly05.mesh");
		naveMesh->setMaterialName("mat02");
		nave->attachObject(naveMesh);
		 //Luz
		//float lightScale = 0.9f;
		//Ogre::Light* light02;
		//Ogre::SceneNode* nodeLuz02 = mSceneMgr->createSceneNode("NodeLuz02");	
		//Ogre::Entity* LightEnt = mSceneMgr->createEntity("MyEntity","sphere.mesh");
		//Ogre::SceneNode* node3 = nodeLuz02->createChildSceneNode("node3");
		//node3->setScale(0.1f,0.1f,0.1f);
		//node3->setPosition(0,20,0);
		//_sceneManager->getRootSceneNode()->addChild(nodeLuz02);
		//Probando LUX
		//node3->attachObject(LightEnt);


		//EJEMPLO DE COMO PONER MATERIALS
		/*Ogre::SceneNode* nM01 = _sceneManager->createSceneNode("nM01");
		Ogre::Entity* entM01 = _sceneManager->createEntity("EntM01","proyectoOgreI.mesh");
		_sceneManager->getRootSceneNode()->addChild(nM01);
		nM01->attachObject(entM01);
		entM01->setMaterialName("mat01");

		Ogre::SceneNode* nM02 = _sceneManager->createSceneNode("nM02");
		Ogre::Entity* entM02 = _sceneManager->createEntity("EntM02","penguin.mesh");
		_sceneManager->getRootSceneNode()->addChild(nM02);
		nM02->attachObject(entM02);
		nM02->scale(10.0f,10.0f,10.0f);
*/
		
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