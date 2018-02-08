#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "../test.h"
#include "cinder\Xml.h"
#include "cinder\osc\Osc.h"
#include "cinder/CinderMath.h"
using namespace ci;
using namespace ci::app;
using namespace std;
const std::string destinationHost = "127.0.0.1";
const uint16_t destinationPort = 57120;
const uint16_t localPort = 57123;



class CinderProjectApp : public App {
	
	void removeDeadTrees();
	int maxTrees;
	int Monitor;
	int windowWidth;
	int windowHeight;
	XmlTree config;
	bool mIsConnected;

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
	//config = XmlTree();
	//maxTrees = config.getAttributeValue<int>("maxTrees");
	//Monitor = config.getAttributeValue<int>("monitorNumber");
	//windowWidth = config.getAttributeValue<int>("width");
	//windowHeight = config.getAttributeValue<int>("height");
	setWindowSize(800, 450);
	maxTrees = 5;
	mSender.bind();
	/*mSender.connect(// Set up the OnConnectFn. If there's no error, you can consider yourself connected to
					// the endpoint supplied.
		[&](asio::error_code error) {
		if (error) {
			CI_LOG_E("Error connecting: " << error.message() << " val: " << error.value());

		}
		else {
			CI_LOG_V("Connected");
			mIsConnected = true;
		}
	});;*/
	stamm = new branch(vec2(getWindowWidth() / 2, getWindowHeight()), 350, 135, 3, 10, 15);
	osc::Message message("/born");
	message.append(1);
	mSender.send(message);
}

void CinderProjectApp::mouseDown( MouseEvent event )
{
	console() << "click click";
}

void CinderProjectApp::update()
{
	stamm->updateTree();
	std::vector<osc::Message> messages = stamm->gatherMessages();
	if (messages.size() > 0) {
		for (std::vector<osc::Message>::iterator it = messages.begin(); it != messages.end(); ++it) {
			mSender.send(*it);
		}
	}

	removeDeadTrees();
	if (wald.size() < maxTrees) {
		int chance = Rand::randInt(30);
		if (chance == 0) {
			vec2 position = vec2( Rand::randInt(getWindowWidth()), getWindowHeight());
			float length = Rand::randFloat(getWindowHeight() * 0.4, getWindowHeight() * 0.75);
			float angle = Rand::randFloat(M_PI*0.8, M_PI * 1.2) ;
			int generation = Rand::randInt(3, 4);
			float width = Rand::randFloat(8, 15);
			float age = Rand::randFloat(15, 25);
			wald.push_back(branch(position, length, angle, generation, width, age ));	
		}
	}
	for (list<branch>::iterator it = wald.begin(); it != wald.end(); it++) {
		it->updateTree();
	}
}

void CinderProjectApp::draw()
{
	//gl::pushMatrices();
	gl::clear( Color( 0, 0, 0 ) );
	stamm->drawTree();
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
