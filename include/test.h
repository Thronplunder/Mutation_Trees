#include <vector>
#include <cmath>
#include "cinder/Cinder.h"
#include "cinder/CinderGlm.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Log.h"
#include "cinder\osc\Osc.h"
#include "cinder\CinderMath.h"

using namespace ci;
using namespace ci::app;

#define USE_UDP 1

#if USE_UDP
using Sender = osc::SenderUdp;
#else
using Sender = osc::SenderTcp;
#endif

class branch {
	std::vector<branch> children;
	vec2 root;
	vec2 head;
	int screen;
	float angle;
	float maxLength;
	float currentLength;
	int generation;
	float width;
	int maxChildren;
	int currentChildren;
	int id;
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
	bool mIsConnected;
	std::vector<osc::Message> messages;
	std::vector<osc::Message> getMessages();
	static void incID();

	static int globalID;

	public:
	void drawTree();
	branch(vec2 root, float maxLength, float angle, int generation, float width, double maxAge, int screen);
	void updateTree();
	vec2 getHead();
	vec2 getRoot();
	bool status();
	std::vector<osc::Message> gatherMessages();
};

