#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "test.h"
#include "cinder\Xml.h"
#include "cinder\osc\Osc.h"
using namespace ci;
using namespace ci::app;
using namespace std;

class CinderProjectApp : public App {
	void removeDeadTrees();
	int maxTrees;
	int Monitor;
	int windowWidth;
	int windowHeight;
	XmlTree config;
	;
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	branch* stamm;
	list<branch> wald;
};

void CinderProjectApp::setup()
{
	//config = XmlTree(;
	//maxTrees = config.getAttributeValue<int>("maxTrees");
	//Monitor = config.getAttributeValue<int>("monitorNumber");
	//windowWidth = config.getAttributeValue<int>("width");
	//windowHeight = config.getAttributeValue<int>("height");
	setWindowSize(800, 600);
	app::console() << "Hello";	
	stamm = new branch(vec2(getWindowWidth() / 2, getWindowHeight()), 350, 135, 3, 10, 15);
}

void CinderProjectApp::mouseDown( MouseEvent event )
{
	console() << "click click";
}

void CinderProjectApp::update()
{
	stamm->updateTree();
}

void CinderProjectApp::draw()
{
	//gl::pushMatrices();
	gl::clear( Color( 0, 0, 0 ) );
	stamm->drawTree();
	//gl::drawString(to_string(stamm->getHead().y), vec2(getWindowWidth() / 2.0, getWindowHeight()/2.0));
	//gl::drawLine(vec2(0, 0), stamm->getRoot());
	
	//gl::popMatrices();
}

void CinderProjectApp::removeDeadTrees() {
	list<branch>::iterator it = wald.begin();

	while (it != wald.end()) {
		if (it->status()) {
			it = wald.erase(it);
		}
	}
}

CINDER_APP( CinderProjectApp, RendererGl )
