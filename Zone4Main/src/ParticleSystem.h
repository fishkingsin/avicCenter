//
//  ParticleSystem.h
//  Zone4Main
//
//  Created by james KONG on 6/3/13.
//
//

#pragma once

#include "ofMain.h"

#define NUM_BILLBOARDS 100
#define PARTICLE_SYSTEM_WIDTH 2048
#define PARTICLE_SYSTEM_HEIGHT 768
class ParticleSystem
{
public:
	void setup()
	{
		decay = 0.95;
		billboards.getVertices().resize(NUM_BILLBOARDS);
		billboards.getColors().resize(NUM_BILLBOARDS);
		billboards.getNormals().resize(NUM_BILLBOARDS,ofVec2f(0));
		float div = 500;
		// ------------------------- billboard particles
		for (int i=0; i<NUM_BILLBOARDS; i++) {
			
			billboardVels[i].set(ofRandomf(), -1.0);//, ofRandomf());
			billboards.getVertices()[i].set((i*1.0f/NUM_BILLBOARDS)*PARTICLE_SYSTEM_WIDTH,(PARTICLE_SYSTEM_HEIGHT*0.5)+
											
											ofSignedNoise((billboards.getVertex(i).x+distance)/div, i*1.0f/NUM_BILLBOARDS, billboards.getVertex(i).z/div)*PARTICLE_SYSTEM_HEIGHT
											,0);
			
			billboards.getColors()[i].set(ofColor::fromHsb(0, 0, 255*((i%2==1)?0.6:0.3)));
			billboardSizeTarget[i] = ofRandom(4, 64);
			
			float n = ((i%2==0 )?5:1) + billboardSizeTarget[i] * ofNoise(i);
			billboards.setNormal(i,ofVec2f(n,0));
			radius[i] = 250;//5*n;
		}
		
		
		billboards.setUsage( GL_DYNAMIC_DRAW );
		billboards.setMode(OF_PRIMITIVE_POINTS);
		//billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
		//billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
		
		// load the bilboard shader
		// this is used to change the
		// size of the particle
		ofLogVerbose() << "Create Billboard shader";
		billboardShader.load("shaders/Billboard");
		
		// we need to disable ARB textures in order to use normalized texcoords
		ofDisableArbTex();
		texture.loadImage("dot.png");
		ofEnableAlphaBlending();
		distance = 0;
	}
	void update()
	{
		float t = (ofGetElapsedTimef()) * 0.9f;
		float div = 250.0;
		
		for (int i=0; i<NUM_BILLBOARDS; i++) {
			ofVec3f vec(0,sinf((billboards.getVertex(i).x/div)+(t*0.9)+TWO_PI*(i*1.0f/NUM_BILLBOARDS)));
			vec *=  ofGetLastFrameTime();
			billboardVels[i] += vec;
			billboards.getVertices()[i] += billboardVels[i];
			billboardVels[i] *= decay;
			
			if(billboards.getVertices()[i].x<-5)
			{
				t = ofGetElapsedTimef()*0.99f;
				billboards.getVertices()[i].x = PARTICLE_SYSTEM_WIDTH+ofRandom(0,100);

				billboards.getVertices()[i].y = (PARTICLE_SYSTEM_HEIGHT*0.5) + ofSignedNoise((billboards.getVertex(i).x)/div, i*1.0f/NUM_BILLBOARDS, billboards.getVertex(i).z/div)*PARTICLE_SYSTEM_HEIGHT*0.5;
			}
			else if(billboards.getVertices()[i].x>PARTICLE_SYSTEM_WIDTH+5)
			{
				t = ofGetElapsedTimef()*0.99f;
				billboards.getVertices()[i].x = -ofRandom(0,100);
				billboards.getVertices()[i].y = (PARTICLE_SYSTEM_HEIGHT*0.5) + ofSignedNoise((billboards.getVertex(i).x)/div, i*1.0f/NUM_BILLBOARDS, billboards.getVertex(i).z/div)*PARTICLE_SYSTEM_HEIGHT*0.5;
			}
				
			
			if(billboards.getVertices()[i].y<-5)billboards.getVertices()[i].y = PARTICLE_SYSTEM_HEIGHT+billboardVels[i].y;
			else if(billboards.getVertices()[i].y>PARTICLE_SYSTEM_HEIGHT+5)billboards.getVertices()[i].y = -abs(billboardVels[i].y);
			
		}
		
		
	}
	void draw()
	{
		ofPushMatrix();
		ofEnableBlendMode(OF_BLENDMODE_ADD);

		
		// bind the shader so that wee can change the
		// size of the points via the vert shader
		
		for (int p=0; p<NUM_BILLBOARDS; p++){

            for (int i=0; i<NUM_BILLBOARDS; i++){
                if (i != p && i%2==0 && p%2==0 || i != p && i%2==1 && p%2==1){
                    
                    float R             = billboards.getVertices()[p].distance(billboards.getVertices()[i]);
                    /*elimination
					 
					*/
                    int _radius = radius[p];
                    if (R < _radius){
                        //numConnect++;
                        if(R<_radius*0.1)
                        {
                            billboardVels[p].x           +=  (billboards.getVertices()[p].x - billboards.getVertices()[i].x) / R*0.8;
                            billboardVels[p].y           +=  (billboards.getVertices()[p].y - billboards.getVertices()[i].y) / R*0.8;

							billboards.getVertices()[p] += billboardVels[p];
                        }
                        ofPushStyle();
                        ofEnableBlendMode(OF_BLENDMODE_ADD);
						float brightness = ((p%2==1)?0.6:0.3);
                        ofSetColor(255, ofMap(R,0,_radius,255,0)*brightness);
                        ofSetLineWidth(1);
                        ofLine(billboards.getVertices()[i].x, billboards.getVertices()[i].y, billboards.getVertices()[i].z, billboards.getVertices()[p].x, billboards.getVertices()[p].y, billboards.getVertices()[p].z);
                        ofPopStyle();
                    }
                }
            }
        }
		
		billboardShader.begin();
		
		ofEnablePointSprites();
        ofSetColor(255);
		texture.getTextureReference().bind();
		billboards.draw();
		texture.getTextureReference().unbind();
		ofDisablePointSprites();
		
		billboardShader.end();
		
		ofPopMatrix();
		ofDisableBlendMode();
		ofEnableAlphaBlending();
		
	}
	void keyPressed(int key)
	{
		ofVec2f vec;
		if(key==OF_KEY_RIGHT)
		{
			vec.set(-10,0);
			distance-=2048;
		}
		if(key==OF_KEY_LEFT)
		{
			distance+=2048;
			vec.set(10,0);
			
		}
		
		for (int i=0; i<NUM_BILLBOARDS; i++) {
							
			billboardVels[i] += vec*((i%2==0)?3:15);//billboards.getNormals()[i];
		}
	}
	float distance;
	// billboard particles
	float billboardSizeTarget[NUM_BILLBOARDS];
	int radius[NUM_BILLBOARDS];
	ofShader billboardShader;
	ofImage texture;
	
	ofVboMesh billboards;
	ofVec2f billboardVels[NUM_BILLBOARDS];
	float decay;
};
