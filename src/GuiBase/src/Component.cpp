#pragma once

#include "Component.h"
#include <iostream>

using namespace SynGUI;

Component::Component(){

}

//Set position of the current component and update children components accordingly ---- WARNING ---- Do not use this for now, use translate instead
void Component::setPosition(glm::vec2 _pos) {
	pos = _pos;

	for (int i = 0; i < childComponents.size(); i++) {
		childComponents.at(i)->setPosition(pos);
	}
}

//Translate he current component and update children components accordingly
void Component::translate(glm::vec2 _translate) {
	pos += _translate;

	for (int i = 0; i < childComponents.size(); i++) {
		childComponents.at(i)->translate(_translate);
	}
}

void Component::addChildComponent(Component* comp) {

	childComponents.push_back(comp);
}

void Component::calculateBoundingBox() {

}

Component::~Component() {

	for (int i = 0; i < childComponents.size(); i++) {
		delete childComponents.at(i);
	}
}