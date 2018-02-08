#include "test.h"

int branch::globalID = 0;
branch::branch(vec2 root, float maxLength, float angle, int generation, float width, double maxAge, int screen){
	this->root = root;
	this->maxLength = maxLength;
	this->angle = angle;
	this->width = width;
	this->currentLength = 0.0011f;
	this->head = root + polarToCartesian(currentLength, angle);
	this->maxChildren = Rand::randInt(6);
	this->currentChildren = 0;
	this->growSpeed = Rand::randFloat(15, 30);
	this->generation = generation;
	this->oldTime = app::getElapsedSeconds();
	this->deltaTime = 0;
	this->age = 0;
	this->isDead = false;
	this->maxAge = maxAge;
	this->id = branch::globalID;
	this->screen = screen;
	branch::incID();

	osc::Message message("/born");
	message.append(id);
	message.append(generation);
	message.append(screen);
	this->messages.push_back(message);
}

void branch::addChild(vec2 root, float maxLength, float angle, float width, int newGeneration){
	children.push_back(branch(root, maxLength, angle, newGeneration, width, (this->maxAge - this->age) * Rand::randFloat(0.75, 0.95), this->screen));
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
		gl::color(1, 1, 1, 1.0 -  1.7*(1.0 / (this->generation + 5)));
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

	if(generation > 0 && currentChildren < maxChildren && age > (2 + 0.5 * this->children.size()) && this->currentLength > (0.25 * this->maxLength)) {
		int chance = Rand::randFloat(0, 55);
		if(chance == 1) {
			vec2 newRoot = root + polarToCartesian(currentLength, angle) * Rand::randFloat(0.1, 0.96);
			float newLength = maxLength * Rand::randFloat(0.3, 0.9);
			float newAngle = angle + Rand::randFloat( M_PI * -0.4, M_PI * 0.4);
			float newWidth = width * Rand::randFloat(0.7, 0.93);
			int newGen = this->generation - 1;
			addChild(newRoot, newLength, newAngle, newWidth, newGen);
			currentChildren = currentChildren + 1;
			int test = this->generation;
		}
	}
	if(currentLength < maxLength){
		grow(growSpeed);
	}
	if (this->age > this->maxAge && this->isDead == false) {
		this->die();
	}
	this->oldTime = app::getElapsedSeconds();
}

void branch::grow(float speed) {
	currentLength += speed * deltaTime;
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
	osc::Message message("/die");
	message.append(id);
	message.append(generation);
	this->messages.push_back(message);
}

std::vector<osc::Message> branch::getMessages()
{
	std::vector<osc::Message> mess = this->messages;
	this->messages.clear();
	return mess;
}

bool branch::status()
 {
	return this->isDead;
}

std::vector<osc::Message> branch::gatherMessages() {
	std::vector<osc::Message> mess;
	mess = this->getMessages();
	if (this->currentChildren > 0) {
		for (std::vector<branch>::iterator it = children.begin(); it != children.end();++it) {
			std::vector<osc::Message> branchMessages = it->gatherMessages();
			mess.insert(mess.end(), branchMessages.begin(), branchMessages.end());
		}
	}
	return mess;
}

void branch::incID() {
	branch::globalID++;
}