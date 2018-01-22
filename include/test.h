#include <vector>
#include <cmath>
#include "cinder/Cinder.h"
#include "cinder/CinderGlm.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/osc/Osc.h"

using namespace ci;
using Sender = osc::SenderTcp;
const std::string destinationHost = "127.0.0.1";
const uint16_t destinationPort = 57121;
const uint16_t localPort = 57120;

class branch {
	std::vector<branch> children;
	vec2 root;
	vec2 head;
	float angle;
	float maxLength;
	float currentLength;
	int generation;
	float width;
	int maxChildren;
	int CurrentChildren;
	float growSpeed;
	double deltaTime;
	double oldTime;
	double age;
	double maxAge;
	bool isDead; 
	void addChild(vec2 root, float maxLength, float angle, float width, int newGeneration);
	void drawBranch();
	void grow(float speed);
	vec2 polarToCartesian(float radius, float angle);
	void updateBranch();
	void die();
	Sender mSender;
	bool mIsConnected;
	void onSendError(asio::error_code error);

	public:
	void drawTree();
	branch(vec2 root, float maxLength, float angle, int generation, float width, double maxAge);
	void updateTree();
	vec2 getHead();
	vec2 getRoot();
	bool status();
};

