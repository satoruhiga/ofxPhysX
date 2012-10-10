#include "testApp.h"

#include "ofxPhysX.h"

ofxPhysX::World world;
ofxPhysX::Static rigid;

vector<ofxPhysX::Rigid> rigids;

//--------------------------------------------------------------
void testApp::setup()
{
//	ofSetVerticalSync(true);
//	ofSetFrameRate(60);
	
	ofBackground(0);
	
	world.setup(ofVec3f());
	
	for (int i = 0; i < 300; i++)
	{
		float r = 10;
		rigids.push_back(world.addSphere(4, ofVec3f(ofRandom(-r, r), ofRandom(-r, r), ofRandom(-r, r))));
	}
	
	rigid = world.addBox(ofVec3f(10, 10, 10), ofVec3f(), ofQuaternion(), 0);
}

//--------------------------------------------------------------
void testApp::update()
{
	world.update();
	
	for (int i = 0; i < rigids.size(); i++)
	{
		ofxPhysX::Rigid& o = rigids[i];
		o.applyImpulseForce(o.getPosition().normalized() * -5);
	}
	
	ofVec3f s;
	float t = 10;
	s.x = sin(ofGetElapsedTimef() * t) + 1;
	s.y = sin(ofGetElapsedTimef() * t + TWO_PI * 1. / 3.) + 1;
	s.z = sin(ofGetElapsedTimef() * t + TWO_PI * 2. / 3.) + 1;
	s *= 100;
	rigid.setSize(s);
	
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw()
{
	cam.begin();
	world.draw();
	cam.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}