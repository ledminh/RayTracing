#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(ofColor::black);
	
	// Set up mainCam
	theCam = &mainCam;
	mainCam.setDistance(10);
	mainCam.setNearClip(.1);

	//Set up preview Cam
		//position = position of renderCam
	previewCam.setPosition(renderCam.position);

		// Fov = Fov of renderCam
	ofVec3f v1 = glm::vec3(renderCam.view.topRight(), renderCam.view.position.z) - renderCam.position;
	ofVec3f v2 = glm::vec3(renderCam.view.bottomRight(), renderCam.view.position.z) - renderCam.position;

	previewCam.setFov(v1.angle(v2));
	
	//Set up sideCam
	sideCam.setPosition(glm::vec3(-40,renderCam.position.y, renderCam.position.z/2));
	sideCam.lookAt(glm::vec3(0,2,10));

	
	initScene();


	//Set up ofImage
	image.allocate(imageWidth, imageHeight, ofImageType::OF_IMAGE_COLOR);

	
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	theCam->begin();
	
	drawAxis(5);

	ofNoFill();

	for (int i = 0; i < scene.size(); i++) {
		scene[i]->draw();
	}

	ofSetColor(ofColor::deepSkyBlue);
	renderCam.draw();



	theCam->end();


	if (bShowImage) {
		image.draw(glm::vec3(0, 0, 0), imageWidth, imageHeight);
	}
		
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case OF_KEY_F1:
		theCam = &mainCam;
		break;
	case OF_KEY_F2:
		theCam = &sideCam;
		break;
	case OF_KEY_F3:
		theCam = &previewCam;
		break;
	case OF_KEY_RETURN:
		rayTrace();
		break;
	case 'I':
	case 'i':
		bShowImage = !bShowImage;
		break;
	case 'Z':
	case 'z':
		saveImage();
		break;
	case 'C':
	case 'c':
		if (mainCamMouseEnable)
			mainCam.disableMouseInput();
		else
			mainCam.enableMouseInput();

		mainCamMouseEnable = !mainCamMouseEnable;
		break;
	case 'P':
	case 'p':
		unselectScene();
		selectPlane();
		break;
	case 'L':
	case 'l':
		unselectScene();
		selectLight();
		break;
	case OF_KEY_UP:
		sceneMove(glm::vec3(0, .5, 0));
		break;
	case OF_KEY_DOWN:
		sceneMove(glm::vec3(0, -.5, 0));
		break;
	case OF_KEY_LEFT:
		sceneMove(glm::vec3(-.5, 0, 0));
		break;
	case OF_KEY_RIGHT:
		sceneMove(glm::vec3(.5, 0, 0));
		break;
	case 'W':
	case 'w':
		sceneMove(glm::vec3(0, 0, -.5));
		break;
	case 'S':
	case 's':
		sceneMove(glm::vec3(0, 0, .5));
		break;
	case 'H':
	case 'h':
		increaseSizeScene(.5);
		break;
	case 'N':
	case 'n':
		decreaseSizeScene(.5);
		break;
	
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (mainCamMouseEnable) return;
	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 rn = glm::normalize(p - theCam->getPosition());

	Ray r = Ray(p, rn);
	int index;
	glm::vec3 point, normal;

	sceneIntersect(r, index, point, normal);

	
	glm::vec3 diff = point - currentMousePos;
	
	sceneMove(diff);

	currentMousePos = point;

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (mainCamMouseEnable) return;

	unselectScene();

	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 rn = glm::normalize(p - theCam->getPosition());

	Ray r = Ray(p, rn);
	int index;
	glm::vec3 point, normal;
	
	sceneIntersect(r, index, point, normal);

	if (index != -1) {
		scene[index]->setMouseSelected(true);
		currentMousePos = point;
	}

	
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	
	for (int i = 0; i < scene.size(); i++) {
		scene[i]->setMouseSelected(false);
	}

	currentMousePos = glm::vec3(-100000, -100000, -100000);

	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//New methods

void ofApp::increaseSizeScene(float s) {
	for (int i = 0; i < scene.size(); i++) {
		scene[i]->increaseSize(s);
	}
}

void ofApp::decreaseSizeScene(float s) {
	for (int i = 0; i < scene.size(); i++) {
		scene[i]->decreaseSize(s);
	}
}


void ofApp::unselectScene() {
	for (int i = 0; i < scene.size(); i++) {
		scene[i]->setMouseSelected(false);
		scene[i]->setKeyBoardSelected(false);
	}
}

void ofApp::sceneMove(glm::vec3 diff) {
	for (int i = 0; i < scene.size(); i++) {
		scene[i]->move(diff);
	}
}
void ofApp::selectPlane() {	
	currentPlaneIndexIndex++;
	
	if (currentPlaneIndexIndex == planeIndexes.size()) {
		currentPlaneIndexIndex = -1;
	}
	else {
		scene[planeIndexes[currentPlaneIndexIndex]]->setKeyBoardSelected(true);
	}
	
	
}

void ofApp::selectLight() {
	currentLightIndex++;

	if (currentLightIndex == lights.size()) {
		currentLightIndex = -1;
		lights[currentLightIndex]->hideGuiPanel(bShowSpotLightPanel, bShowPointLightPanel);
	}
	else {
		lights[currentLightIndex]->setKeyBoardSelected(true);
		lights[currentLightIndex]->showGuiPanel(bShowSpotLightPanel, bShowPointLightPanel);
	}
		
}

void ofApp::addPlane(glm::vec3 pos, glm::vec3 n, ofColor c, string imagePath, int tile, int width, int height) {
	Plane* p = new Plane(pos, n, c, width, height);
	p->setTexture(Texture(ofImage(imagePath)), tile);
	scene.push_back(p);
	planeIndexes.push_back(scene.size() - 1);
}



void ofApp::sceneIntersect(const Ray &r, int &index, glm::vec3 &point, glm::vec3 &normal) {
	float dSquare = 1000000;
	index = -1;

	for (int iS = 0; iS < scene.size(); iS++) {

		glm::vec3 p;
		glm::vec3 n;

		if (scene[iS]->intersect(r, p, n)) {
			float thisDSquare = ofDistSquared(p.x, p.y, p.z, renderCam.position.x, renderCam.position.y, renderCam.position.z);


			if (thisDSquare < dSquare) {
				dSquare = thisDSquare;
				index = iS;
				point = p;
				normal = n;
			}

		}


	}
}

void ofApp::initScene() {
	// Create horizontal plane
	addPlane(glm::vec3(0, -2, 0), glm::vec3(0, 1, 0), ofColor::white, "wood_floor_2.jpg", 20, 60, 60);

	// Create vertical plane
	addPlane(glm::vec3(0, 10, -30), glm::vec3(0, 0, 1), ofColor::white, "concrete_bunker_texture.jpg", 1, 60, 60);

	
	// Create 3 spheres
	Sphere* s1 = new Sphere(glm::vec3(-1, -1, -1), 1, ofColor(153, 76, 0));
	s1->setTexture(Texture(ofImage("sphere_texture_1.jpg")), 1);
	scene.push_back(s1);

	Sphere* s2 = new Sphere(glm::vec3(-.5, -1.5, .5), .5, ofColor(102, 0, 51));
	s2->setTexture(Texture(ofImage("metal_texture.tiff")), 1);
	scene.push_back(s2);


	Sphere* s3 = new Sphere(glm::vec3(1, -1, 0), 1, ofColor(0, 102, 204));
	s3->setTexture(Texture(ofImage("beachball_texture.jpg")), 1);
	scene.push_back(s3);


	//Create lights
	lights.push_back(new Light(glm::vec3(0, 8, 0), 2));


	//Spot light
	glm::vec3 posSpotLight1 = glm::vec3(-6, 5, 6);
	glm::vec3 direction = glm::normalize(-posSpotLight1);
	lights.push_back(new SpotLight(posSpotLight1, .4, direction, 20));





	//Push lights to scene
	for (int i = 0; i < lights.size(); i++) {
		scene.push_back(lights[i]);
	}
}

void ofApp::saveImage() {
	int i = 1;
	string fileName = "image_" + to_string(i) + ".jpg";
	ofFile file = ofFile(fileName);
	
	while (file.exists()) {
		i++;
		fileName = "image_" + to_string(i) + ".jpg";
		file = ofFile(fileName);
	}

	image.save(fileName);
}

ofColor ofApp::lambert(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse) {
	ofColor result = ofColor::black;
	
	for (int i = 0; i < lights.size(); i++) {
		if (!isBlocked(p, lights[i]->position)) {
			
			if (lights[i]->isVisible(p)) {
				float I = lights[i]->getIntensity();
				
				glm::vec3 l = lights[i]->position - p;
									   
				float r = glm::length(l)*0.1;
				result += diffuse * (I / (r * r))*glm::max(0.0, (double)glm::dot(norm, glm::normalize(l)));
			}
			
		
		
		
		}


		
	}

	return result;
}

ofColor ofApp::phong(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse, const ofColor specular, float power) {
	ofColor result = ofColor::black;

	glm::vec3 v = glm::normalize(renderCam.position - p);

	for (int i = 0; i < lights.size(); i++) {
		
		if(lights[i]->isVisible(p)){
			float I = lights[i]->getIntensity();


			glm::vec3 l = lights[i]->position - p;
			float r = glm::length(l);

			l = glm::normalize(l);

			glm::vec3 h = (v + l) / glm::length(v + l);


			result += specular * (I / (r * r)) *  glm::pow(glm::max(0.0, (double)glm::dot(norm, h)), power);
		}
		
		

		
	}

	return result;
}

void ofApp::rayTrace() {	
	
	for (int w = 0; w < imageWidth; w++) {
		for (int h = 0; h < imageHeight; h++) {
			int index;
			glm::vec3 point, normal;
					
			float u = (w + 0.5) / imageWidth,
				v = 1 - (h + 0.5) / imageHeight;
			Ray r = renderCam.getRay(u, v);

			sceneIntersect(r, index, point, normal);
			
					
			if (index == -1) { //Background
				image.setColor(w, h, ofColor::black);
			}
			else { //Scene
				ofColor color = ofColor::white * .04; //Ambient light

				scene[index]->getNormal(normal);

				color += lambert(point, normal, scene[index]->getDiffuseColor(point));

				float power = 100;

				color += phong(point, normal, scene[index]->getDiffuseColor(point), scene[index]->getSpecularColor(point), power);

				

							   				 				
				image.setColor(w, h, color);
			}
			
		}
	}
	
	image.update();

}

bool ofApp::isBlocked(glm::vec3 point, glm::vec3 lightPos) {
	glm::vec3 vFromPtoL = lightPos - point;
	glm::vec3 d = glm::normalize(vFromPtoL);

	Ray r = Ray(point + d * 0.01, d);
	glm::vec3 pIntersect, n;

	bool blocked = false;

	for (int i = 0; i < scene.size() && !blocked; i++) {
		if (scene[i]->intersect(r, pIntersect, n)) {
			float distPoint2Intersect = glm::distance2(pIntersect, point),
				distPoint2Light = glm::distance2(lightPos, point);
			
			if (distPoint2Intersect < distPoint2Light)
				blocked = true;
		}
	}

	return blocked;
}


void ofApp::drawAxis(double s) {
	//X
	ofSetColor(ofColor::red);
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(s, 0, 0));

	//Y
	ofSetColor(ofColor::green);
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, s, 0));

	//Z
	ofSetColor(ofColor::blue);
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, s));
}





