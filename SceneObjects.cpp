#include "SceneObjects.h"


//Texture
//
void Texture::loadImage(ofImage i) {
	image = i;

	width = image.getWidth();
	height = image.getHeight();
}

ofColor Texture::getColor(float u, float v) {
	u = u - (int)u;
	v = v - (int)v;

	int i = u * width - 0.5,
		j = v * height - 0.5;

	return image.getColor(i, j);


}


// Sphere
//
bool Sphere::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	return (glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal));
}
void Sphere::draw() {
	if (isMouseSelected() || isKeyBoardSelected()) {
		ofSetColor(ofColor::red);
	}
	else {
		ofSetColor(diffuseColor);
	}

	ofDrawSphere(position, radius);
}

void Sphere::move(glm::vec3 diff) {
	if (isMouseSelected())
		position = position + diff;
}

ofColor Sphere::getDiffuseColor(glm::vec3 point) {
	if (!haveTexture) return diffuseColor;

	return getTextureColor(point);

}

ofColor Sphere::getSpecularColor(glm::vec3 point) {
	if (!haveTexture) return diffuseColor;

	return getTextureColor(point);

}

ofColor Sphere::getTextureColor(glm::vec3 point) {
	glm::vec3 d = glm::normalize(point - position),
		dx = glm::vec3(d.x, 0, 0),
		dy = glm::vec3(0, d.y, 0),
		dz = glm::vec3(0, 0, d.z);



	float u = tile * (0.5 + glm::atan(d.x, d.z) / (2 * PI)),
		v = tile * (0.5 - glm::asin(d.y) / PI);

	return texture.getColor(u, v);
}


// Plane
//
Plane::Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse, float w, float h) {
	position = p; normal = n;
	width = w;
	height = h;

	if (normal.x == 1) {
		origin = glm::vec3(p.x, p.y - h / 2, p.z + w / 2);
		plane.rotateDeg(90, 0, 1, 0);
	}
	else if (normal.y == 1) {
		origin = glm::vec3(p.x - w / 2, p.y, p.z + h / 2);
		plane.rotateDeg(90, 1, 0, 0);
	}
	else if (normal.z == 1) {
		origin = glm::vec3(p.x - w / 2, p.y - h / 2, p.z);

	}





	diffuseColor = diffuse;

}
Plane::Plane() {
	normal = glm::vec3(0, 1, 0);
	plane.rotateDeg(90, 1, 0, 0);
}

//*****************************************************************************************
// Intersect Ray with Plane  (wrapper on glm::intersect*
//
bool Plane::intersect(const Ray &ray, glm::vec3 & point, glm::vec3 & normalAtIntersect) {
	float dist;
	bool insidePlane = false;
	bool hit = glm::intersectRayPlane(ray.p, ray.d, position, this->normal, dist);
	if (hit) {
		Ray r = ray;
		point = r.evalPoint(dist);
		normalAtIntersect = this->normal;

		if (normal.x == 1) {
			glm::vec2 yrange = glm::vec2(position.y - width / 2, position.y + width / 2);
			glm::vec2 zrange = glm::vec2(position.z - height / 2, position.z + height / 2);
			if (point.y < yrange[1] && point.y > yrange[0] && point.z < zrange[1] && point.z > zrange[0]) {
				insidePlane = true;
			}
		}
		else if (normal.y == 1) {
			glm::vec2 xrange = glm::vec2(position.x - width / 2, position.x + width / 2);
			glm::vec2 zrange = glm::vec2(position.z - height / 2, position.z + height / 2);
			if (point.x < xrange[1] && point.x > xrange[0] && point.z < zrange[1] && point.z > zrange[0]) {
				insidePlane = true;
			}
		}
		else if (normal.z == 1) {
			glm::vec2 xrange = glm::vec2(position.x - width / 2, position.x + width / 2);
			glm::vec2 yrange = glm::vec2(position.y - width / 2, position.y + width / 2);
			if (point.x < xrange[1] && point.x > xrange[0] && point.y < yrange[1] && point.y > yrange[0]) {
				insidePlane = true;
			}

		}



	}
	return insidePlane;
}

void Plane::move(glm::vec3 diff) {
	if (isKeyBoardSelected()) {
		position = position + diff;

		if (normal.x == 1) {
			origin = glm::vec3(position.x, position.y - height / 2, position.z + width / 2);
		}
		else if (normal.y == 1) {
			origin = glm::vec3(position.x - width / 2, position.y, position.z + height / 2);
		}
		else if (normal.z == 1) {
			origin = glm::vec3(position.x - width / 2, position.y - height / 2, position.z);

		}
	}

}

void Plane::increaseSize(float s) {
	if (isKeyBoardSelected()) {
		width += s;
		height += s;

		if (normal.x == 1) {
			origin = glm::vec3(position.x, position.y - height / 2, position.z + width / 2);
		}
		else if (normal.y == 1) {
			origin = glm::vec3(position.x - width / 2, position.y, position.z + height / 2);
		}
		else if (normal.z == 1) {
			origin = glm::vec3(position.x - width / 2, position.y - height / 2, position.z);

		}
	}

}

void Plane::decreaseSize(float s) {
	if (isKeyBoardSelected()) {
		width -= s;
		height -= s;

		if (normal.x == 1) {
			origin = glm::vec3(position.x, position.y - height / 2, position.z + width / 2);
		}
		else if (normal.y == 1) {
			origin = glm::vec3(position.x - width / 2, position.y, position.z + height / 2);
		}
		else if (normal.z == 1) {
			origin = glm::vec3(position.x - width / 2, position.y - height / 2, position.z);

		}
	}

}

void Plane::draw() {
	if (isKeyBoardSelected()) {
		ofSetColor(ofColor::red);
	}
	else {
		ofSetColor(diffuseColor);
	}

	plane.setPosition(position);
	plane.setWidth(width);
	plane.setHeight(height);
	plane.setResolution(4, 4);
	plane.drawWireframe();
}

ofColor Plane::getDiffuseColor(glm::vec3 point) {
	if (!haveTexture) return diffuseColor;

	return getTextureColor(point);

}

ofColor Plane::getSpecularColor(glm::vec3 point) {
	if (!haveTexture) return diffuseColor;

	return getTextureColor(point);

}

ofColor Plane::getTextureColor(glm::vec3 point) {
	float u, v;

	if (normal.x == 1) {
		u = tile * (origin.z - point.z) / width;
		v = tile * (point.y - origin.y) / height;
	}
	else if (normal.y == 1) {
		u = tile * (point.x - origin.x) / width;
		v = tile * (origin.z - point.z) / height;
	}
	else if (normal.z == 1) {
		u = tile * (point.x - origin.x) / width;
		v = tile * (point.y - origin.y) / height;


	}


	return texture.getColor(u, v);
}