/*
 *  PanoApp.h
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
//
//#ifndef Zone4Main_PanoApp_h
//#define Zone4Main_PanoApp_h
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ParticleSystem.h"
#include "ofxImageSequence.h"
#include "Layer.h"
#define DEFAULT_DURATION 3
class PanoApp
{
public:
    void init()
    {
        
    }
    void setup()
    {
        particleSystem.setup();
        if(xml.loadFile("pano.xml"))
        {
            if(xml.pushTag("DATA"))
            {
                int numLayer = xml.getNumTags("LAYER");
                
                //                string str;
                for(int i = 0 ; i < numLayer ; i++)
                {
                    if(xml.pushTag("LAYER",i))
                    {
                        
                        Layer * layer = new Layer();
                        layer->width = xml.getValue("WIDTH",2048);
                        layer->page_width = xml.getValue("PAGE_WIDTH",2048);
                        layers.push_back(layer);
                        int numPage = xml.getNumTags("PAGE");
                        
                        for( int pageIdx = 0 ; pageIdx < numPage ; pageIdx++)
                        {
                            Page * new_page = new Page();
                            layer->pages.push_back(new_page);
                            if(xml.pushTag("PAGE",pageIdx))
                            {
                                if(xml.pushTag("IMAGES"))
                                {
                                    
                                    int numImage = xml.getNumTags("IMAGE");
                                    
                                    //                                    new_page->images.assign(numImage , new MyImages());
                                    for(int imgIdx = 0 ; imgIdx < numImage ; imgIdx++)
                                    {
                                        if(xml.pushTag("IMAGE",imgIdx))
                                        {
                                            MyImages * new_images = new MyImages();
                                            new_page->images.push_back(new_images);
                                            string fn = xml.getValue("FILE","");
                                            int index = xml.getValue("INDEX",-1);
                                            ofPoint p (xml.getValue("X",0),xml.getValue("Y",0));
                                            
                                            ofLogVerbose("XML TAG IMAGE")  << index << " " << fn << " " << p << " " ;
                                            new_images->setup(index , fn , p);
                                            
                                            //                                            all_images.push_back(new_images->images);
                                            xml.popTag();
                                        }
                                    }
                                    xml.popTag();
                                }
                                if(xml.pushTag("SEQUENCES"))
                                {
                                    int numSeq = xml.getNumTags("SEQUENCE");
                                    for(int segIdx = 0 ; segIdx < numSeq ; segIdx++)
                                    {
                                        if(xml.pushTag("SEQUENCE",segIdx))
                                        {
                                            string fn = xml.getValue("FILE","");
                                            ofLogVerbose("XML TAG SEQUENCES") << xml.getValue("FILE","") << " " <<
                                            xml.getValue("X",0) << " " <<
                                            xml.getValue("Y",0) << " " ;
                                            ofxImageSequence *newSeq = new ofxImageSequence();
                                            newSeq->loadSequence(fn);
                                            sequences.push_back(newSeq);
                                            
                                            xml.popTag();
                                        }
                                    }
                                    xml.popTag();
                                }
                                
                                
                                xml.popTag();
                            }
                            
                        }
                        
                        xml.popTag();
                    }
                }
                xml.popTag();
            }
            
        }
        
    }
    void update()
    {
        particleSystem.update();
        vector<Layer*>::iterator it;
        
        for(it = layers.begin() ; it!=layers.end() ; it++)
        {
            (*it)->update();
        }
        
    }
    void draw()
    {
        
        ofPushStyle();
        
        ofEnableAlphaBlending();
        vector<Layer*>::iterator it;
        int layer = 0;
        for(it = layers.begin() ; it!=layers.end() ; it++)
        {
            ofSetColor(255);
            (*it)->draw();
            if(layer==0)particleSystem.draw();
            layer++;
        }
        vector<ofxImageSequence*>::iterator it2;

        
        int x = 0;
        for(it2 = sequences.begin() ; it2!=sequences.end() ; it2++)
        {
            ofSetColor(255);
            (*it2)->getFrameForTime(ofGetElapsedTimef())->draw(x+=50,0,50,200);
        }
        
        
        
        ofPopStyle();
        
        
    }
    void keyPressed(int key)
    {
        switch(key)
        {
            case OF_KEY_LEFT:
            {
                if(pageIndex>0)
                {
                    if(layers[2]->shiftCategory(-1))
                    {
                        pageIndex--;
                        vector<Layer*>::iterator it;
                        
                        for(it = layers.begin() ; it!=layers.end() ; it++)
                        {
                            
                            (*it)->animationToPage(pageIndex);
                        }
                    }
                }
            }
                break;
            case OF_KEY_RIGHT:
            {
                if(pageIndex<5)
                {
                    if(layers[2]->shiftCategory(1))
                    {
                        if(pageIndex<4)pageIndex++;
                        vector<Layer*>::iterator it;
                        
                        for(it = layers.begin() ; it!=layers.end() ; it++)
                        {
                            
                            (*it)->animationToPage(pageIndex);
                        }
                    }
                    
                    
                }
            }
                break;
        }
    }
    void gotoPage(int index)
    {
//        page;
        
        
        
        for(int i = 0 ; i < layers.size() ;i++)
        {
            vector<Page*> p = layers[i]->pages;
            
            for(int j = 0; j < p.size() ; j++)
            {
                
                vector<MyImages*>im = p[j]->images;

                for(int k=0 ; k<im.size() ; k++)
                {
                    
                    
                    if (im[k]->index ==index) {
                        p[j]->index = k;
                        ofLogVerbose() <<"im[k]->index " << im[k]->index;
                        ofLogVerbose() <<"p[j]->index " << p[j]->index;
                        vector <Layer*> :: iterator it;
                        for(it = layers.begin() ; it!=layers.end() ; it++)
                        {
                            
                            (*it)->animationToPage(j);
                        }
                        break;
                    }

                }
                
            }

        }
    }
    vector<Layer*>layers;
    ofxXmlSettings xml;
    int duration;
    ofPoint seqPoint;
    ParticleSystem particleSystem;
    int pageIndex;
    vector<ofImage*>all_images;
    vector<ofxImageSequence*>sequences;
};


//#endif
