//
//  PanoApp.h
//  Zone4Sub
//
//  Created by james KONG on 11/3/13.
//
//

#pragma once

#include "ofMain.h"

#include "ofxImageSequence.h"
#define MAX_PAGE 5
#define DURATION 3
#define WIDTH 1024
#include "ofxAnimatableFloat.h"
class MyImages
{
public:
	MyImages()
	{
		
	}
	MyImages(string folder)
	{
		setup(folder);
		
	}
	virtual void setup(string folder)
	{
		ofDirectory dir;
		int totalWidth = 0;
		int nFiles = dir.listDir(folder);
		if(nFiles) {
			
			for(int i=0; i<dir.numFiles(); i++) {
				
				// add the image to the vector
				string filePath = dir.getPath(i);
				images.push_back(ofImage());
				images.back().loadImage(filePath);
				totalWidth+=images.back().getWidth();
			}
			
		}
		ofLogVerbose("MyImages") << folder << ": width " << totalWidth;
		step = totalWidth/images.size();
		ofLogVerbose("MyImages") << folder << ": step " << step;
	}
	int step;
	int width;
	ofVec2f position;
	vector<ofImage>images;
};
class MyImagesWithAnimation : public MyImages
{
public:
	MyImagesWithAnimation(string folder)
	{
		setup(folder);
		animation.setRepeatType(PLAY_ONCE);
		animation.setCurve(EASE_OUT);
		animation.setDuration(DURATION);
	}
	
	
	ofxAnimatableFloat animation;
	
};
class PanoApp
{
public:
	PanoApp()
	{
		bSetup = false;
		
	}
	void setup()
	{
		
	}
	void init()
	{
		while(!myimage1.empty())
		{
			myimage1.pop_back();
		}
		currentPage = 0;
		myimage1.push_back(MyImagesWithAnimation("images/layer"));
		
		currentPageX.setRepeatType(PLAY_ONCE);
		currentPageX.setCurve(EASE_OUT);
		currentPageX.setDuration(DURATION);
		glDisable(GL_DEPTH_TEST);
		bSetup = true;
		
	}
	
	void update()
	{
		float dt = 1.0f / ofGetFrameRate();
		
		vector<MyImagesWithAnimation>::iterator it1;
		for (it1=myimage1.begin(); it1!=myimage1.end(); it1++) {
			MyImagesWithAnimation *a = &(*it1);
			a->animation.update(dt);
		}
		currentPageX.update(dt);
	}
	void draw()
	{
		if(bSetup)
		{

			vector<MyImagesWithAnimation>::iterator it1;
			vector<ofImage>::iterator it2;
			
			int layer = 0;
			for (it1=myimage1.begin(); it1!=myimage1.end(); it1++) {
				MyImagesWithAnimation *a = &(*it1);
				
				int x = 0;
				for (it2=a->images.begin(); it2!=a->images.end(); it2++) {
					ofImage *img = &(*it2);
					float _x  = x+a->animation.getCurrentValue();
					if(_x>-100 || _x < ofGetWidth())
					{
						img->draw(_x,0);
					}
					x+=img->getWidth();
					
				}
				layer++;
			}
			
		}
		else{
			ofEnableAlphaBlending();

			ofDrawBitmapString("Loading...", ofGetWidth()*0.5,ofGetHeight()*0.5);
		}
	}
	void keyPressed(int key)
	{
		if(key==OF_KEY_LEFT || key==OF_KEY_RIGHT)
		{
			
			if( bSetup)
			{
				if(isCoolDown() )
				{
					if(key==OF_KEY_RIGHT)
					{
						{
							
							if(currentPage<MAX_PAGE-1)
							{
								prevPage = currentPage;
								currentPage++;

								coolDown = ofGetElapsedTimef();
								ofLogVerbose("PanoApp") << "currentPage " << currentPage;
								vector<MyImagesWithAnimation>::iterator it1;
								
								for (it1=myimage1.begin(); it1!=myimage1.end(); it1++) {
									MyImagesWithAnimation *a = &(*it1);
									a->animation.animateTo( -currentPage*a->step);
								}
								currentPageX.animateTo(-currentPage*WIDTH);
							}
						}
						
						
					}
					
					else if(key==OF_KEY_LEFT)
					{
						{
							if(currentPage>0)
							{
								prevPage = currentPage;
								currentPage--;
								coolDown = ofGetElapsedTimef();
								vector<MyImagesWithAnimation>::iterator it1;
								ofLogVerbose("PanoApp") << "currentPage " << currentPage;
								for (it1=myimage1.begin(); it1!=myimage1.end(); it1++) {
									MyImagesWithAnimation *a = &(*it1);
									
									a->animation.animateTo( -currentPage*a->step);
								}
								currentPageX.animateTo(-currentPage*WIDTH);
							}
							
						}
					}
				}
			}

			
			
		}
		if(key=='r')
		{
			init();
		}
	}
	bool isCoolDown()
	{
		return abs(coolDown -ofGetElapsedTimef())>DURATION;
	}

	vector<MyImagesWithAnimation> myimage1;
	
	int currentPage;
//	int category;
	int prevCategory;
	int prevPage;
	ofxAnimatableFloat currentPageX;
	
	float coolDown;
	bool bSetup;
	int nSequence;
	
};