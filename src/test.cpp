#include "test.h"

  
branch::branch(vec2 root, float maxLength, float angle, int generation, float width, double maxAge)
 :mSender(localPort, destinationHost, destinationPort), mIsConnected(false) {
	this->root = root;
	this->maxLength = maxLength;
	this->angle = angle;
	this->width = width;
	this->currentLength = 0.0011f;
	this->head = root + polarToCartesian(currentLength, angle);
	this->maxChildren = Rand::randInt(7);
	this->CurrentChildren = 0;
	this->growSpeed = Rand::randFloat(0.1, 1);
	this->generation = generation;
	this->oldTime = app::getElapsedSeconds();
	this->deltaTime = 0;
	this->age = 0;
	this->isDead = false;
	this->maxAge = maxAge;
	mSender.bind();
	//osc::Message msg("/born");
	//msg.append(generation);
	//mSender.send(msg);
}

void branch::addChild(vec2 root, float maxLength, float angle, float width, int newGeneration){
	children.push_back(branch(root, maxLength, angle, newGeneration, width, (this->maxAge - this->age) * 0.9));
}
void branch::drawTree(){
	drawBranch();

	if (children.size() > 0) {
		for (std::vector<branch>::iterator it = children.begin(); it != children.end(); ++it) {
			it->drawTree();
		}
	}
}
void branch::drawBranch(){
	if (this->isDead == false) {
		gl::pushMatrices();
		gl::lineWidth(width);
		glLineWidth(width);
		gl::color(Color(0.5, 0.5, 0));
		gl::drawLine(root, head);
		gl::popMatrices();
	}
}
vec2 branch::polarToCartesian(float radius, float angle) {
	return vec2(radius * std::sin(angle), radius * std::cos(angle));
}
void branch::updateBranch() {

	this->deltaTime = app::getElapsedSeconds() - this->oldTime;
	this->age += this->deltaTime;

	if(generation > 0 && CurrentChildren < maxChildren && age > (2 + 0.5 * this->children.size())) {
		int chance = Rand::randFloat(0, 25);
		if(chance == 1) {
			vec2 newRoot = root + polarToCartesian(currentLength, angle) * Rand::randFloat(0.1, 0.96);
			float newLength = maxLength * Rand::randFloat(0.3, 0.9);
			float newAngle = angle + Rand::randFloat(-45, 45);
			float newWidth = width * Rand::randFloat(0.6, 0.9);
			int newGen = this->generation - 1;
			addChild(newRoot, newLength, newAngle, newWidth, newGen);
			CurrentChildren = CurrentChildren + 1;
			int test = this->generation;
		}
	}
	if(currentLength < maxLength){
		grow(growSpeed);
	}
	if (this->age > this->maxAge) {
		this->die();
	}
	this->oldTime = app::getElapsedSeconds();
}

void branch::grow(float speed) {
	currentLength += speed;
	head = root + polarToCartesian(currentLength, angle);
}

void branch::updateTree() {
	this->updateBranch();

	if (children.size() > 0) {
		for (std::vector<branch>::iterator it = children.begin(); it != children.end(); ++it) {
			it->updateTree();
		}
	}
}
vec2 branch::getHead() {
	return polarToCartesian(currentLength, angle);
}
vec2 branch::getRoot() {
	return root;
}

void branch::die() {
	this->isDead = true;
}

bool branch::status() {
	return this->isDead;
}