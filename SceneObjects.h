#pragma once
#include "ofMain.h"
#include "Ray.h"


class Texture {
public:
	Texture() {}
	Texture(ofImage i) { loadImage(i); }

	void loadImage(ofImage i);

	float getWidth() { return width; }
	float getHeight() { return height; }

	ofColor getColor(float u, float v);

private:
	ofImage image;
	float width, height;
};

//  Base class for any renderable object in the scene
//
class SceneObject {
public:
	SceneObject() {}
	virtual void draw() = 0;    // pure virtual funcs - must be overloaded
	virtual bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { return false; }


	virtual void setTexture(Texture t, int tle) {
		texture = t;
		tile = tle;
		haveTexture = true;
	}

	void setMouseSelected(bool s) { bMouseSelected = s; }
	bool isMouseSelected() { return bMouseSelected; }

	void setKeyBoardSelected(bool s) { bKeyBoardSelected = s; }
	bool isKeyBoardSelected() { return bKeyBoardSelected; }

	virtual void move(glm::vec3 diff) {};
	virtual void increaseSize(float s) {};
	virtual void decreaseSize(float s) {};

	// any data common to all scene objects goes here
	glm::vec3 position = glm::vec3(0, 0, 0);

	virtual ofColor getDiffuseColor(glm::vec3 point) { return diffuseColor; }
	virtual ofColor getSpecularColor(glm::vec3 point) { return specularColor; }

	virtual void setDiffuseColor(ofColor d) { diffuseColor = d; }
	virtual void setSpecularColor(ofColor s) { specularColor = s; }


protected:
	// material properties (we will ultimately replace this with a Material class - TBD)
	//
	ofColor diffuseColor = ofColor::grey;    // default colors - can be changed.
	ofColor specularColor = ofColor::white;

	bool haveTexture = false;
	Texture texture;
	int tile = 1;

	bool bMouseSelected = false,
		bKeyBoardSelected = false;
};

//  General purpose sphere  (assume parametric)
//
class Sphere : public SceneObject {
public:
	Sphere(glm::vec3 p, float r, ofColor diffuse = ofColor::lightGray) { position = p; radius = r; diffuseColor = diffuse; }
	Sphere() {}
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	
	void draw(); 

	void move(glm::vec3 diff); 

	ofColor getDiffuseColor(glm::vec3 point);
	ofColor getSpecularColor(glm::vec3 point); 

	float radius = 1.0;

private:
	ofColor getTextureColor(glm::vec3 point); 
};

//  Mesh class (will complete later- this will be a refinement of Mesh from Project 1)
//
class Mesh : public SceneObject {
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { return false; }
	void draw() { }
};


//  General purpose plane 
//
class Plane : public SceneObject {
public:
	Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse = ofColor::darkOliveGreen, float w = 20, float h = 20);
	Plane(); 

	bool intersect(const Ray &ray, glm::vec3 & point, glm::vec3 & normal);

	void move(glm::vec3 diff);
	void increaseSize(float s); 
	void decreaseSize(float s); 

	void draw(); 

	ofColor getDiffuseColor(glm::vec3 point); 
	ofColor getSpecularColor(glm::vec3 point); 

	
	glm::vec3 normal = glm::vec3(0, 1, 0);
	
	ofPlanePrimitive plane;
	float width = 20;
	float height = 20;

	glm::vec3 origin;

private:
	ofColor getTextureColor(glm::vec3 point); 
};
