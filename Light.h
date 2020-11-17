#pragma once

#include "SceneObjects.h"



// Light class
class Light : public SceneObject {
public:
	Light() {}
	Light(glm::vec3 p, float i) {
		intensity = i;
		position = p;
	}

	void setIntensity(float i) {
		intensity = i;
	}

	float getIntensity() {
		return intensity;
	}

	void move(glm::vec3 diff) {
		if (isKeyBoardSelected()) {
			position = position + diff;
		}
	}

	virtual void showGuiPanel(bool &spotlightP, bool &pointlightP) {
		spotlightP = false;
		pointlightP = true;
	}

	void hideGuiPanel(bool &spotlightP, bool &pointlightP) {
		spotlightP = false;
		pointlightP = false;
	}

	void draw() {
		if (isMouseSelected() || isKeyBoardSelected()) {
			ofSetColor(ofColor::red);
		}
		else {
			ofSetColor(ofColor::blue);
		}

		ofDrawSphere(position, .1);
	}

	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		return false;
		//return (glm::intersectRaySphere(ray.p, ray.d, position, .1, point, normal));
	}

	virtual bool isVisible(glm::vec3 p) { return true; }



private:
	float intensity = 0;


};


class SpotLight : public Light {
public:
	SpotLight() {}

	SpotLight(glm::vec3 p, float i, glm::vec3 d, float cA) {
		setIntensity(i);
		position = p;
		direction = d;
		coneAngle = cA;

	}

	void setDirection(glm::vec3 d) {
		direction = d;
	}

	bool isVisible(glm::vec3 p) {
		bool result = false;

		glm::vec3 line = glm::normalize(p - position);


		if (glm::abs(glm::degrees(glm::angle(line, direction))) <= coneAngle)
			result = true;

		return result;
	}

	void showGuiPanel(bool &spotlightP, bool &pointlightP) {
		spotlightP = true;
		pointlightP = false;
	}

	glm::vec3 getDirection() { return direction; }

	void setConeAngle(float cA) { coneAngle = cA; }
	float getConeAngle() { return coneAngle; }

	void draw() {
		if (isMouseSelected() || isKeyBoardSelected()) {
			ofSetColor(ofColor::red);
		}
		else {
			ofSetColor(ofColor::blue);
		}

		ofDrawSphere(position, .1);

		ofDrawLine(position, position + direction * 2);
	}


private:
	glm::vec3 direction = glm::vec3(0, 0, 0);
	float coneAngle = 0;

};