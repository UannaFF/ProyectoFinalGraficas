#include "stdafx.h"
using namespace Ogre;

Ogre::SceneNode* nave, *camera_node, *met, **aros, **monedas;
int score, vidas;
Ogre::SceneNode* rocks[3], *colgolem[30];


//Ogros de roca, movimiento
class FrameListenerRock : public Ogre::FrameListener {
private:
	OIS::InputManager* _man;
	OIS::Keyboard* _key;
	float grados;
public:
	FrameListenerRock(Ogre::RenderWindow* win) {
		size_t windowHnd = 0;
		std::stringstream windowsHndStr;
		win->getCustomAttribute("WINDOW", &windowHnd);
		windowsHndStr << windowHnd;
		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"), windowsHndStr.str()));
		_man = OIS::InputManager::createInputSystem(pl);
		_key = static_cast<OIS::Keyboard*>(_man->createInputObject(OIS::OISKeyboard,false));
		grados = 0.0f;
	}

	~FrameListenerRock() {
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

		/*if (_key->isKeyDown(OIS::KC_W))
			t += Ogre::Vector3(0,0,-movSpeed);

		if (_key->isKeyDown(OIS::KC_S))
			t += Ogre::Vector3(0,0,+movSpeed);*/
		Ogre::Vector3 navePos = camera_node->getPosition();
		for(int i=0;i<3;i++){
			Ogre::Vector3 ogrePos = rocks[i]->getPosition();
			//std::cout << "Posogro: " << ogrePos << " PosNave: "<< navePos <<std::endl;
			if(navePos.z <= (ogrePos.z+10000) && navePos.z >= (ogrePos.z-10000)){
				Ogre::Node* rightArm = rocks[i]->getChild("rightArm"+i);
				Ogre::Quaternion Quat = rightArm->getOrientation();
				Ogre::Degree DegY; 
				Quat.ToAngleAxis( DegY, Ogre::Vector3(0,1,0) );
				if(DegY > Ogre::Degree(0)){
					rightArm->rotate(Ogre::Quaternion(Ogre::Degree(15 * evt.timeSinceLastFrame), Ogre::Vector3::UNIT_Y));
				}

				Ogre::Node* leftArm = rocks[i]->getChild("leftArm"+i);
				Quat = leftArm->getOrientation();
				Quat.ToAngleAxis( DegY, Ogre::Vector3(0,1,0) );
				if(DegY > Ogre::Degree(0)) {
					leftArm->rotate(Ogre::Quaternion(Ogre::Degree(-15 * evt.timeSinceLastFrame), Ogre::Vector3::UNIT_Y));
				}

				Ogre::Node* leftLeg = rocks[i]->getChild("leftLeg"+i);
				Quat = leftLeg->getOrientation();
				Quat.ToAngleAxis( DegY, Ogre::Vector3(1,0,0) );
				if(DegY > Ogre::Degree(0)) {
					leftLeg->rotate(Ogre::Quaternion(Ogre::Degree(15 * evt.timeSinceLastFrame), Ogre::Vector3::UNIT_X));
				}

				Ogre::Node* rightLeg = rocks[i]->getChild("rightLeg"+i);
				Quat = rightLeg->getOrientation();
				Quat.ToAngleAxis( DegY, Ogre::Vector3(1,0,0) );
				if(DegY > Ogre::Degree(0)) {
					rightLeg->rotate(Ogre::Quaternion(Ogre::Degree(15 * evt.timeSinceLastFrame), Ogre::Vector3::UNIT_X));
				}
			}
		}
		return true;
	}
};

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
		//
		// INTERACCION CON TECLADO
		//
		float movSpeed = 2500.0f;
		_key->capture();
		
		if (_key->isKeyDown(OIS::KC_ESCAPE))
			return false;

		Ogre::Vector3 t (0,0,0);
		Ogre::Quaternion q (Ogre::Degree(0), Ogre::Vector3::ZERO);
		Ogre::Quaternion qc (Degree(0), Vector3::ZERO);

		if (_key->isKeyDown(OIS::KC_W))
			t += Ogre::Vector3(0,0,-movSpeed);

/*		if (_key->isKeyDown(OIS::KC_S))
			t += Ogre::Vector3(0,0,+movSpeed);
			*/
		if(_key->isKeyDown(OIS::KC_D) || _key->isKeyDown(OIS::KC_A)){
			if (_key->isKeyDown(OIS::KC_A)){
				t += Ogre::Vector3(-movSpeed,0,0);
				if (grados > -45)
				{
					grados -= 90 * evt.timeSinceLastFrame;
					q = q * Ogre::Quaternion(Ogre::Degree(-90 * evt.timeSinceLastFrame), Ogre::Vector3::UNIT_Y);
				}
			}
			if (_key->isKeyDown(OIS::KC_D)){
				t += Ogre::Vector3(movSpeed,0,0);
				if (grados < 45)
				{
					grados += 90 * evt.timeSinceLastFrame;
					q = q * Ogre::Quaternion(Ogre::Degree(90 * evt.timeSinceLastFrame), Ogre::Vector3::UNIT_Y);
				}
			}
		} else
		{
			if (grados > 0.1f)
			{
				grados -= 90 * evt.timeSinceLastFrame;
				q = q * Ogre::Quaternion(Ogre::Degree(-90 * evt.timeSinceLastFrame), Ogre::Vector3::UNIT_Y);
			} else if (grados < -0.001f){
				grados += 90 * evt.timeSinceLastFrame;
				q = q * Ogre::Quaternion(Ogre::Degree(90 * evt.timeSinceLastFrame), Ogre::Vector3::UNIT_Y);
			}
		}
		if (_key->isKeyDown(OIS::KC_UP))
			t += Ogre::Vector3(0,movSpeed,0);

		if (_key->isKeyDown(OIS::KC_DOWN))
			t += Ogre::Vector3(0,-movSpeed,0);
		/*if (_key->isKeyDown(OIS::KC_LEFT))
			qc = Quaternion(Degree(90 * evt.timeSinceLastFrame), Vector3::UNIT_Y);

		if (_key->isKeyDown(OIS::KC_RIGHT))
			qc = Quaternion(Degree(-90 * evt.timeSinceLastFrame), Vector3::UNIT_Y);
			*/
		nave->rotate(q);
		camera_node->translate(t * evt.timeSinceLastFrame, camera_node->TS_LOCAL);
		camera_node->rotate(qc);
		

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
		else if(pos.y < 500)
			pos.y = 500;

		camera_node->setPosition(pos);

		//
		// COLISIONES
		//
		// Aros
		for (int i = 0; i < sizeof(aros); i++)
		{
			if (aros[i] != NULL)
			{
				AxisAlignedBox aab = nave->_getWorldAABB().intersection(aros[i]->_getWorldAABB());
				if(!aab.isNull())
				{
					score += 200;
					std::cout << "Score:" << score << std::endl;
					delete aros[i];
					aros[i] = NULL;
				}
			}
		}

		// Monedas
		for (int i = 0; i < sizeof(monedas); i++)
		{
			if (monedas[i] != NULL)
			{
				AxisAlignedBox aab = nave->_getWorldAABB().intersection(monedas[i]->_getWorldAABB());
				if(!aab.isNull())
				{
					score += 100;
					std::cout << "Score:" << score << std::endl;
					delete monedas[i];
					monedas[i] = NULL;
				}
			}
		}
		// Golems
		for (int i = 0; i < sizeof(colgolem); i++)
		{
			if (colgolem[i] != NULL)
			{
				AxisAlignedBox aab = nave->_getWorldAABB().intersection(colgolem[i]->_getWorldAABB());
				if(!aab.isNull())
				{
					vidas -= 1;
					std::cout << "Vidas:" << vidas << std::endl;
					camera_node->translate(0,0,2500);
					if (vidas == 0)
						return false;
				}
			}
		}
		// Meteorito
		if (met != NULL)
		{
			AxisAlignedBox aab = nave->_getWorldAABB().intersection(met->_getWorldAABB());
			if(!aab.isNull())
			{
				vidas -= 1;
				std::cout << "Vidas:" << vidas << std::endl;
				camera_node->translate(0,0,2500);
			}
		}
		return true;
	}
};


class FrameListenerMet : public Ogre::FrameListener {


public:
	FrameListenerMet(){
	}

	~FrameListenerMet(){
	}

	bool frameStarted(const Ogre::FrameEvent &evt){
		//
		// INTERACCION CON TECLADO
		//
		float movSpeed = 2000.0f;
		Vector3 t (0, -movSpeed, 0);
		met->translate(t*evt.timeSinceLastFrame, met->TS_LOCAL);	
		if (met->getPosition().y < 1000)
		{
			met->translate(-13000, 12000, -10000);
		}
		return true;
	}
};

/*
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
*/


class OgreProyectos {

public:
	Ogre::SceneManager* _sceneManager;
	Ogre::Root* _root;
	Ogre::Camera* camera;
	Ogre::SceneNode* nM03;
	Ogre::RenderWindow* window;
	FrameListenerNave* FrameListenerNave01;
	FrameListenerMet* FrameListenerMet01;
	FrameListenerRock* FrameListenerRock01;

	OgreProyectos(){
		monedas = new SceneNode* [rand() % 5 + 3];
		aros = new SceneNode* [rand() % 8 + 3];
		_sceneManager = NULL;
		_root = NULL;
		FrameListenerNave01 = NULL;
		score = 0;
		vidas = 3;
		for (int i = 0; i < sizeof(colgolem); i++)
		{
			colgolem[i] = NULL;
		}
	}


	~OgreProyectos(){ 
		delete _root;
		delete FrameListenerNave01;
		delete FrameListenerMet01;
		delete aros;
		delete monedas;
	}

	void createCamera() {
		camera = _sceneManager->createCamera("Camera");
		camera->setPosition(0,300,2000);
		camera->lookAt(Ogre::Vector3(0,0,0));
		camera->setNearClipDistance(5);
	}

	void createFrameListeners() {
		FrameListenerNave01 = new FrameListenerNave(nave, window, camera_node);
		_root->addFrameListener(FrameListenerNave01);

		FrameListenerRock01 = new FrameListenerRock(window);
		_root->addFrameListener(FrameListenerRock01);
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

	//Funci�n para crear una moneda
	SceneNode* createCoin(int pos_x, int pos_y, int pos_z, int num){
		Ogre::SceneNode* nodoMoneda = _sceneManager->createSceneNode("Moneda"+ std::to_string(num));
		Ogre::Entity* entMoneda = _sceneManager->createEntity("moneda"+ std::to_string(num),"poly14.mesh");
		entMoneda->setMaterialName("coin");
		nodoMoneda->attachObject(entMoneda);
		nodoMoneda->setPosition(pos_x, pos_y, pos_z);
		nodoMoneda->scale(1.7,2,2);
		nodoMoneda->rotate(Quaternion(Degree(90), Vector3::UNIT_X));
		_sceneManager->getRootSceneNode()->addChild(nodoMoneda);
		return nodoMoneda;
	}

	//Funci�n para crear un aro
	SceneNode* createRing(int pos_x, int pos_y, int pos_z, int num){
		Ogre::SceneNode* nodoAro = _sceneManager->createSceneNode("Aro"+std::to_string(num));
		Ogre::Entity* entAro = _sceneManager->createEntity("aro"+std::to_string(num), "poly04.mesh");
		entAro->setMaterialName("mat01");
		nodoAro->attachObject(entAro);
		nodoAro->setPosition(pos_x, pos_y, pos_z);
		nodoAro->scale(5,5,5);
		nodoAro->rotate(Quaternion(Degree(90), Vector3::UNIT_X));
		_sceneManager->getRootSceneNode()->addChild(nodoAro);
		return nodoAro;
	}

	void createScene() {
	
		//_sceneManager->setSkyBox(true, "SkyBox");
		_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		_sceneManager->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));
		_sceneManager->setSkyBox(true, "MySkyBox1");
		
		Ogre::Light* light = _sceneManager->createLight("Light01");
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDiffuseColour(0.0f, 1.0f, 1.0f);
		light->setSpecularColour(0.0f, 1.0f, 1.0f);
		light->setDirection(Vector3(0,-1,-1));

		/*Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0.0);

		Ogre::MeshManager::getSingleton().createPlane("plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 20000, 90000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
		Ogre::Entity* entPlano = _sceneManager->createEntity("PlanoEntity", "plane");
		_sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(entPlano);
		entPlano->setMaterialName("mat01");*/

		//Creando camara
		camera_node = _sceneManager->createSceneNode("Camera");
		camera_node->attachObject(camera);
		camera_node->setPosition(0,0,0);
		_sceneManager->getRootSceneNode()->addChild(camera_node);

		//Creando nave
		nave = camera_node->createChildSceneNode("Nave");
		Ogre::Entity* naveMesh = _sceneManager->createEntity("nav","poly05.mesh");
		naveMesh->setMaterialName("mat02");
		nave->attachObject(naveMesh);

		ParticleSystem* particulas_nave = _sceneManager->createParticleSystem("Nave", "ParticulasNave");
		camera_node->attachObject(particulas_nave);


		met = _sceneManager->createSceneNode("Met01");
		Ogre::Entity* entMet = _sceneManager->createEntity("Met01", "poly18.mesh");
		entMet->setMaterialName("mat01");
		met->attachObject(entMet);
		met->setPosition(-8000,12000,5000);
		met->scale(7,7,7);
		met->rotate(Quaternion(Degree(45), Vector3::UNIT_Z));
		_sceneManager->getRootSceneNode()->addChild(met);

		ParticleSystem* particulas_met = _sceneManager->createParticleSystem("Smoke", "Examples/Smoke");
		met->createChildSceneNode("ParticulasMet")->attachObject(particulas_met);

		FrameListenerMet01 = new FrameListenerMet();
		_root->addFrameListener(FrameListenerMet01);

		// Creamos los aros y las monedas de manera aleatoria
		for (int i = 0; i < sizeof(aros); i++)
		{
			aros[i] = createRing((rand() % 180)*100 - 10000,  (rand() % 80)*100 + 1000, (rand() % 870)*100 - 43000, i);
		}
		for (int i = 0; i < sizeof(monedas); i++)
		{
			monedas[i] = createCoin((rand() % 180)*100 - 10000,  (rand() % 80)*100 + 1000, (rand() % 870)*100 - 43000, i);
		}

		Ogre::SceneNode* parteIzq = nave -> createChildSceneNode("PartIzq");
		Ogre::Entity* naveMesh_izq = _sceneManager->createEntity("nav_izquierda","poly10.mesh");
		naveMesh_izq->setMaterialName("mat02");
		parteIzq->scale(1.0,1.0,0.5);
		parteIzq->translate(-190.0f,0.0f,0.0f);
		parteIzq->rotate(Ogre::Quaternion(Ogre::Degree(20), Ogre::Vector3::UNIT_Z));
		parteIzq->attachObject(naveMesh_izq);

		Ogre::SceneNode* parteTriIzq = nave -> createChildSceneNode("PartTriIzq");
		Ogre::Entity* naveMesh_triIzq = _sceneManager->createEntity("nav_triIzq","usb_prisma.mesh");
		naveMesh_triIzq->setMaterialName("mat02");
		parteTriIzq->scale(25.0,50.0,10.0);
		parteTriIzq->translate(-105.0f,375.0f,0.0f);
		parteTriIzq->rotate(Ogre::Quaternion(Ogre::Degree(-180), Ogre::Vector3::UNIT_Y));
		parteTriIzq->attachObject(naveMesh_triIzq);

		Ogre::SceneNode* parteDer = nave -> createChildSceneNode("PartDer");
		Ogre::Entity* naveMesh_der = _sceneManager->createEntity("nav_derecha","poly10.mesh");
		naveMesh_der->setMaterialName("mat02");
		parteDer->scale(1.0,1.0,0.5);
		parteDer->translate(170.0f,0.0f,0.0f);
		parteDer->rotate(Ogre::Quaternion(Ogre::Degree(-200), Ogre::Vector3::UNIT_Z));
		parteDer->attachObject(naveMesh_der);

		Ogre::SceneNode* parteTriDer = nave -> createChildSceneNode("PartTriDer");
		Ogre::Entity* naveMesh_triDer= _sceneManager->createEntity("nav_triDer","usb_prisma.mesh");
		naveMesh_triDer->setMaterialName("mat02");
		parteTriDer->scale(25.0,50.0,10.0);
		parteTriDer->translate(83.0f,375.0f,0.0f);
		parteTriDer->attachObject(naveMesh_triDer);

		Ogre::SceneNode* parteBall = nave -> createChildSceneNode("PartBall");
		Ogre::Entity* naveMesh_ball = _sceneManager->createEntity("nav_ball","poly12.mesh");
		naveMesh_ball->setMaterialName("ballTex");
		parteBall->translate(0.0f,50.0f,0.0f);
		parteBall->attachObject(naveMesh_ball);

		//Poner la nave en la posicion que es
		nave->rotate(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_X));
		nave->scale(0.4f,0.4f,0.4f);
		int j = 0;
		//Generando ogro
		for(int i= 0;i<3;i++){
			rocks[i] = _sceneManager->getRootSceneNode()->createChildSceneNode("rock"+i);
			colgolem[j++] = rocks[i];
			//Ogre::SceneNode* bodyRock = rocks[0]->createChildSceneNode("head0");
			Ogre::Entity* entBodyRock = _sceneManager->createEntity("rocBodykEnt"+i,"poly01.mesh");
			entBodyRock->setMaterialName("rockMat");
			rocks[i]->attachObject(entBodyRock);
			if(i == 0)
				rocks[i]->setPosition(0,5000,20000);
			else if(i == 1)
				rocks[i]->setPosition(5000,6000,0);
			else
				rocks[i]->setPosition(-5000, 5000, -30000);
			rocks[i]->scale(30.0,50.0,20.0);

			Ogre::SceneNode* head = rocks[i]->createChildSceneNode("head"+i);
			Ogre::Entity* entBodyRockHead = _sceneManager->createEntity("rocBodykEntHead"+i,"poly01.mesh");
			entBodyRockHead->setMaterialName("extremMat");
			head->attachObject(entBodyRockHead);
			head->setPosition(0,60,0);
			head->scale(0.6,0.4,0.6);
			colgolem[j++] = head;

			//Pierna izquierda
			Ogre::SceneNode* leftLeg = rocks[i]->createChildSceneNode("leftLeg"+i);
			Ogre::Entity* entLeftLeg = _sceneManager->createEntity("ballLegLeft"+i,"poly18.mesh");
			entLeftLeg->setMaterialName("rockMat");
			leftLeg->attachObject(entLeftLeg);
			leftLeg->translate(-40.0f,-65.0f,0.0f);
			leftLeg->scale(0.25,0.2,0.3);
			leftLeg->rotate(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_X));
			colgolem[j++] = leftLeg;

			Ogre::SceneNode* leftLeg2 = leftLeg->createChildSceneNode("leftLeg2"+i);
			Ogre::Entity* entleftLeg2 = _sceneManager->createEntity("entleftLeg2"+i,"poly01.mesh");
			entleftLeg2->setMaterialName("extremMat");
			leftLeg2->attachObject(entleftLeg2);
			leftLeg2->translate(-35.0f,-95.0f,0.0f);
			leftLeg2->scale(1.8,2.0,1.0);
			colgolem[j++] = leftLeg2;



			//Pierna derecha
			Ogre::SceneNode* rightLeg = rocks[i]->createChildSceneNode("rightLeg"+i);
			Ogre::Entity* entRightLeg = _sceneManager->createEntity("ballRightLeft"+i,"poly18.mesh");
			entRightLeg->setMaterialName("rockMat");
			rightLeg->attachObject(entRightLeg);
			rightLeg->translate(40.0f,-65.0f,0.0f);
			rightLeg->scale(0.25,0.2,0.3);
			rightLeg->rotate(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_X));
			colgolem[j++] = rightLeg;

			Ogre::SceneNode* rightLeg2 = rightLeg->createChildSceneNode("rightLeg2"+i);
			Ogre::Entity* entRightLeg2 = _sceneManager->createEntity("entRightLeg2"+i,"poly01.mesh");
			entRightLeg2->setMaterialName("extremMat");
			rightLeg2->attachObject(entRightLeg2);
			rightLeg2->translate(35.0f,-95.0f,0.0f);
			rightLeg2->scale(1.8,2.0,1.0);
			colgolem[j++] = rightLeg2;

			//Brazo derecho
			Ogre::SceneNode* rightArm = rocks[i]->createChildSceneNode("rightArm"+i);
			Ogre::Entity* entRightArm = _sceneManager->createEntity("ballRightArm"+i,"poly18.mesh");
			entRightArm->setMaterialName("rockMat");
			rightArm->attachObject(entRightArm);
			rightArm->translate(67.0f,30.0f,0.0f);
			rightArm->scale(0.25,0.2,0.3);
			rightArm->rotate(Ogre::Quaternion(Ogre::Degree(-90), Ogre::Vector3::UNIT_Y));
			colgolem[j++] = rightArm;

			Ogre::SceneNode* rightArm2 = rightArm->createChildSceneNode("rightArm2"+i);
			Ogre::Entity* entRightArm2 = _sceneManager->createEntity("entRightArm2"+i,"poly01.mesh");
			entRightArm2->setMaterialName("extremMat");
			rightArm2->attachObject(entRightArm2);
			rightArm2->translate(95.0f,30.0f,0.0f);
			rightArm2->scale(2.5,1.7,1.0);
			colgolem[j++] = rightArm2;

			//Brazo izquierdo
			Ogre::SceneNode* leftArm = rocks[i]->createChildSceneNode("leftArm"+i);
			Ogre::Entity* entLeftArm = _sceneManager->createEntity("ballLeftArm"+i,"poly18.mesh");
			entLeftArm->setMaterialName("rockMat");
			leftArm->attachObject(entLeftArm);
			leftArm->translate(-63.0f,30.0f,0.0f);
			leftArm->scale(0.25,0.2,0.3);
			leftArm->rotate(Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y));
			colgolem[j++] = leftArm;

			Ogre::SceneNode* leftArm2 = leftArm->createChildSceneNode("lefttArm2"+i);
			Ogre::Entity* entLeftArm2 = _sceneManager->createEntity("entLeftArm2"+i,"poly01.mesh");
			entLeftArm2->setMaterialName("extremMat");
			leftArm2->attachObject(entLeftArm2);
			leftArm2->translate(-145.0f,30.0f,0.0f);
			leftArm2->scale(2.5,1.7,1.0);
			colgolem[j++] = leftArm2;
		}


		/*Ogre::Entity* naveMesh_der = _sceneManager->createEntity("nav_derecha","poly10.mesh");
		naveMesh_der->setMaterialName("mat02");
		nave->attachObject(naveMesh_der);*/


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
		camera_node->translate(0,0,45000);
		std::cout << "Score:" << score << std::endl;
	}

};

int main(void)
{
	srand(time(NULL));
	OgreProyectos app;
	app.startup();
	return 0;

}