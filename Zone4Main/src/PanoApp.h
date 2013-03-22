//
//  PanoApp.h
//  Zone4Main
//
//  Created by james KONG on 6/3/13.
//
//
#pragma once

#include "ofMain.h"

#include "ofxImageSequence.h"
#define MAX_PAGE 5
#define DEFAULT_DURATION 3
#define WIDTH 5120
#include "ParticleSystem.h"
#define FPS 1/60.0f
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
		step = totalWidth/MAX_PAGE;
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
	MyImagesWithAnimation(string folder , float _duration)
	{
		setup(folder);
		animation.setRepeatType(PLAY_ONCE);
		animation.setCurve(QUADRATIC_EASE_OUT);
		animation.setDuration(_duration);
	}
		

	ofxAnimatableFloat animation;

};
class PanoApp
{
public:
	PanoApp()
	{
        seqPoint.set(0, 0);
		bSetup = false;
		duration = DEFAULT_DURATION;
	}
	void setup()
	{
		particleSystem.setup();
	}
	void init()
	{
		
		while(!myimage1.empty())
		{
			myimage1.pop_back();
		}
		while(!myimage2.empty())
		{
			myimage2.pop_back();
		}
		currentPage = 0;
		myimage1.push_back(MyImagesWithAnimation("images/layer1" , duration));
		myimage1.push_back(MyImagesWithAnimation("images/layer2" , duration));
		currentPageX.setRepeatType(PLAY_ONCE);
		currentPageX.setCurve(QUADRATIC_EASE_OUT);
		currentPageX.setDuration(duration);
		glDisable(GL_DEPTH_TEST);
		
		myimage2.push_back(MyImages("images/category/cat01"));
		myimage2.back().position.x = 0;
		myimage2.push_back(MyImages("images/category/cat02"));
		myimage2.back().position.x = WIDTH;
		myimage2.push_back(MyImages("images/category/cat03"));
		myimage2.back().position.x = WIDTH*2;
		
		myimage2.push_back(MyImages("images/category/cat04"));
		myimage2.back().position.x = WIDTH*3;
		//		myimage3 = (MyImagesWidthAnimation*)&myimage2.back();
		//		ofDirectory dir;
		int totalWidth = 0;
		ofDirectory dir2;
		int nFiles = dir2.listDir("images/category/cat04/imagesequence");
		
		if(nFiles) {
			nSequence = nFiles;
			sequences = new ofxImageSequence[nFiles];
			for(int i=0; i<dir2.numFiles(); i++) {
				sequences[i].loadSequence(dir2.getPath(i));
			}
		}
		
		myimage2.push_back(MyImages("images/category/cat05"));
		myimage2.back().position.x = WIDTH*4;
		
		category=0;
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
		particleSystem.update();
	}
	void draw()
	{
		if(bSetup)
		{
			//	image.draw(0,0);
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
				if(layer==0)particleSystem.draw();
				layer++;
			}
			if(currentPage==3 )
			{
				
				if(category<nSequence && isCoolDown()){
					sequences[category].getFrameForTime(ofGetElapsedTimef())->draw(seqPoint.x,seqPoint.y);
				}
			}
//			if(myimage2[prevPage].images.size()>1)myimage2[prevPage].images[0].draw(myimage2[prevPage].position.x+currentPageX, 0);
			if(category<myimage2[currentPage].images.size())myimage2[currentPage].images[category].draw(myimage2[currentPage].position.x+currentPageX.getCurrentValue(), 0);
			
		}
		else{
			ofEnableAlphaBlending();
			particleSystem.draw();
			ofDrawBitmapString("Loading...", ofGetWidth()*0.5,ofGetHeight()*0.5);
		}
	}
	void keyPressed(int key)
	{
		if(key==OF_KEY_LEFT || key==OF_KEY_RIGHT)
		{
			
			if( bSetup)
			{
//				if(isCoolDown() )
				{
					if(key==OF_KEY_RIGHT)
					{
						category++;
						if(category>=myimage2[currentPage].images.size())
						{
							
							if(currentPage<MAX_PAGE-1)
							{
								prevPage = currentPage;
								currentPage++;
								particleSystem.keyPressed(key);
								coolDown = ofGetElapsedTimef();
								ofLogVerbose("PanoApp") << "currentPage " << currentPage;
								vector<MyImagesWithAnimation>::iterator it1;
								
								for (it1=myimage1.begin(); it1!=myimage1.end(); it1++) {
									MyImagesWithAnimation *a = &(*it1);
									a->animation.animateTo( -currentPage*a->step);
//									Tweener.addTween(a->position.x, -currentPage*a->step, DURTATION);
								}
								currentPageX.animateTo(-currentPage*WIDTH);
//								Tweener.addTween(currentPageX, -currentPage*WIDTH, DURTATION);
							}
							category	= 0;
							
							prevCategory = category;
							
						}
						
						
					}
					
					else if(key==OF_KEY_LEFT)
					{
						category--;
						if(category<0)
						{
							if(currentPage>0)
							{
								prevPage = currentPage;
								currentPage--;
								coolDown = ofGetElapsedTimef();
								particleSystem.keyPressed(key);
								vector<MyImagesWithAnimation>::iterator it1;
								ofLogVerbose("PanoApp") << "currentPage " << currentPage;
								for (it1=myimage1.begin(); it1!=myimage1.end(); it1++) {
									MyImagesWithAnimation *a = &(*it1);
									
									a->animation.animateTo( -currentPage*a->step);
//									Tweener.addTween(a->position.x, -currentPage*a->step, DURTATION);
								}
//								Tweener.addTween(currentPageX, -currentPage*WIDTH, DURTATION);
								currentPageX.animateTo(-currentPage*WIDTH);
							}
							
							prevCategory = category;
							category	= myimage2[currentPage].images.size()-1;
						}
					}
				}
			}
			else
			{
				particleSystem.keyPressed(key);
			}
			
			
		}
		if(key=='r')
		{
			init();
		}
	}
	bool isCoolDown()
	{
		return  abs(coolDown -ofGetElapsedTimef())>duration;
	}
	void gotoPage(int page_)
	{
		int index = 0;

		for(int j =0 ; j < myimage2.size(); j++)
		{
			for(int  i = 0 ; i < myimage2[j].images.size() ;i ++)
			{
				
				if(index==page_)
				{
					if(currentPage<j) particleSystem.keyPressed(OF_KEY_RIGHT);
					else if(currentPage>j)particleSystem.keyPressed(OF_KEY_LEFT);
					prevPage = currentPage;
					prevCategory = category;

					currentPage = j;
					category = i;
					coolDown = ofGetElapsedTimef();
					ofLogVerbose("PanoApp") << "currentPage " << currentPage;
					vector<MyImagesWithAnimation>::iterator it1;

					for (it1=myimage1.begin(); it1!=myimage1.end(); it1++) {
						MyImagesWithAnimation *a = &(*it1);
						a->animation.animateTo( -currentPage*a->step);

					}
					currentPageX.animateTo(-currentPage*WIDTH);
				}
				index++;
			}
		}
		
	}

	vector<MyImagesWithAnimation> myimage1;

	vector<MyImages> myimage2;
	ofPoint seqPoint;
	int currentPage;
	int category;
	int prevCategory;
	int prevPage;
	ofxAnimatableFloat currentPageX;
	
	float coolDown;
	bool bSetup;
	ParticleSystem particleSystem;
	
	ofxImageSequence* sequences;
	int nSequence;
	float duration;
	
};