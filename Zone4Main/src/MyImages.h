/*
 *  MyImages.h
 *
 *  Copyright (c) 3/27/13, James Kong
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of 16b.it nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef Zone4Main_MyImages_h
#define Zone4Main_MyImages_h
#include "ofMain.h"
class MyImages
{
public:
    MyImages()
    {
		sequence = NULL;
        images = NULL;
        bLoad = false;
        position.set(0,0);
    }
    void setup(int _index , string filename , ofPoint _pos)
    {
        index = _index;
        position = _pos;
        fn = filename;
        load();
    }
	void setupSequence(string file ,ofPoint point)
	{
		sequence= new ofxImageSequence();
		seqPt.set(point);
		sequence->loadSequence(file);
	}
    void load()
    {
        if(!bLoad)
        {
            
            ofLogVerbose("MyImages") << fn << " position : "<< position;
            ofImage img(fn);
            images = new ofTexture();
            images->allocate(img.getWidth(),img.getHeight(),GL_RGBA);
            images->loadData(img.getPixelsRef());
            bLoad = true;
            ofLogVerbose("MyImages")<< fn <<" load";
        }
    }
    void draw()
    {
        if(bDraw)
        {
            if(sequence!=NULL )
			{
				sequence->getFrameForTime(ofGetElapsedTimef())->draw(seqPt+layerPosition);
			}

            if(images!=NULL && ofGetLogLevel()==OF_LOG_VERBOSE)
            {
                ofPushStyle();
                ofNoFill();
                ofSetLineWidth(3);
                ofSetColor(255,0,0);
                ofRect(position+layerPosition,images->getWidth(),images->getHeight());
                ofPopStyle();
            }
            
            
            
            if(bLoad && images!=NULL)
            {
                
                images->draw(position+layerPosition);
            }
            
            

        }
    }
    
    void unload()
    {
        if(bLoad)
        {
//            ofLogVerbose("MyImages")<< fn <<" unload";
//            images->clear();
//            images = NULL;
//            bLoad = false;
        }
    }
    string fn;
    bool bLoad ;
    ofTexture *images;
	
    ofxImageSequence *sequence;
    bool bDraw;
    ofPoint position,seqPt;
    ofPoint layerPosition;
    int index;
};


#endif
