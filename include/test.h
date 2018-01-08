#include <vector>
#include <cmath>
#include "cinder/Cinder.h"
#include "cinder/CinderGlm.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;

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
	void addChild(vec2 root, float maxLength, float angle, float width, int newGeneration);
	void drawBranch();
	void grow(float speed);
	vec2 polarToCartesian(float radius, float angle);
	void updateBranch();

	public:
	void drawTree();
	branch(vec2 root, float maxLength, float angle, int generation, float width);
	void updateTree();
	vec2 getHead();
	vec2 getRoot();
};

