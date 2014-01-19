#include "ofMain.h"

#include "ofxPhysX.h"

ofxPhysX::World world;

ofxPhysX::RigidStatic rigid;

vector<ofxPhysX::RigidBody> rigids;

class ofApp : public ofBaseApp
{
public:
	void setup()
	{
		ofSetFrameRate(60);
		ofSetVerticalSync(true);
		ofBackground(0);
		
		world.setup(ofVec3f(0, 0, 0));
		
		for (int i = 0; i < 300; i++)
		{
			float r = 30;
			rigids.push_back(world.addSphere(4, ofVec3f(ofRandom(-r, r), ofRandom(-r, r), ofRandom(-r, r))));
		}
		
		rigid = world.addBox(ofVec3f(10, 10, 10), ofVec3f(), ofQuaternion(), 0);
	}
	
	void update()
	{
		world.update();

		for (int i = 0; i < rigids.size(); i++)
		{
			ofxPhysX::RigidBody& o = rigids[i];
			o.applyImpulseForce(o.getPosition().normalized() * -1000);
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
	
	ofEasyCam cam;
	void draw()
	{
		cam.begin();
		world.draw();
		cam.end();
	}

	void keyPressed(int key)
	{
	}

	void keyReleased(int key)
	{
	}
	
	void mouseMoved(int x, int y)
	{
	}

	void mouseDragged(int x, int y, int button)
	{
	}

	void mousePressed(int x, int y, int button)
	{
	}

	void mouseReleased(int x, int y, int button)
	{
	}
	
	void windowResized(int w, int h)
	{
	}
};


int main(int argc, const char** argv)
{
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp);
	return 0;
}
