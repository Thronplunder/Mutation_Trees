#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "test.h"
#include "cinder\Xml.h"
#include "cinder\osc\Osc.h"
#include "cinder/CinderMath.h"
using namespace ci;
using namespace ci::app;
using namespace std;

XmlTree doc(loadAsset("config.xml"));

const std::string destinationHost = doc.getChild("config/destinationIP").getValue<std::string>();
const uint16_t destinationPort = 57120;
const uint16_t localPort = 57123;



class CinderProjectApp : public App {
	
	void removeDeadTrees();
	int maxTrees;
	int Monitor;
	int windowWidth;
	int windowHeight;
	bool mIsConnected;
	int screen;

  public:
	  CinderProjectApp();
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	branch* stamm;
	Sender mSender;
	list<branch> wald;
};

CinderProjectApp::CinderProjectApp():mSender(localPort, destinationHost, destinationPort)
{}

void CinderProjectApp::setup() {
	Rand::randomize();
	XmlTree doc(loadAsset("config.xml"));
	maxTrees = doc.getChild("config/maxTrees").getValue<int>();
	windowWidth = doc.getChild("config/windowWidth").getValue<int>();
	windowHeight = doc.getChild("config/windowHeight").getValue<int>();
	setWindowSize(windowWidth, windowHeight);
	screen = doc.getChild("config/screen").getValue<int>();
	mSender.bind();
	
}

void CinderProjectApp::mouseDown( MouseEvent event )
{

}

void CinderProjectApp::update()
{
	removeDeadTrees();
	if (wald.size() < maxTrees) {
		int chance = Rand::randInt(30);
		if (chance == 0 && getElapsedSeconds() < 300) {
			vec2 position = vec2( Rand::randInt(getWindowWidth()* 0.1, getWindowWidth() * 0.9), getWindowHeight()+ 9);
			float length = Rand::randFloat(getWindowHeight() * 0.5, getWindowHeight() * 0.95);
			float angle = Rand::randFloat(M_PI*0.8, M_PI * 1.2) ;
			int generation = Rand::randInt(3, 4);
			float width = Rand::randFloat(10, 17);
			float age = Rand::randFloat(15, 25);
			wald.push_back(branch(position, length, angle, generation, width, age, screen
			));	
		}
	}
	for (list<branch>::iterator it = wald.begin(); it != wald.end(); it++) {
		it->updateTree();
	}
	if (wald.size() > 0) {
		for (list<branch>::iterator it = wald.begin(); it != wald.end(); it++) {
			vector<osc::Message> messages = it->gatherMessages();
			for (vector<osc::Message>::iterator message = messages.begin(); message != messages.end(); message++) {
				mSender.send(*message);
			}
		}
	}
}

void CinderProjectApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	gl::setMatricesWindow(getWindowSize());
	for (list<branch>::iterator it = wald.begin(); it != wald.end(); it++) {
		it->drawTree();
	}
}

void CinderProjectApp::removeDeadTrees() {
	list<branch>::iterator it = wald.begin();

	while (it != wald.end()) {
		if (it->status()) {
			it = wald.erase(it);
		}
		else {
			it++;
		}
	}
}

CINDER_APP( CinderProjectApp, RendererGl )
