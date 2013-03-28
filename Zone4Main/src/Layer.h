/*
 *  Header.h
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

#ifndef Zone4Main_Header_h
#define Zone4Main_Header_h
#include "Page.h"
#include "ofxAnimatableFloat.h"
class Layer
{
public:
    Layer()
    {
        currentPage = 0;

        animation.setRepeatType(PLAY_ONCE);
		animation.setCurve(QUADRATIC_EASE_OUT);
		animation.setDuration(3);
    }
    bool animationToPage(int dir)
    {

        
            animation.animateTo( -dir * page_width);
        return true;
    }
    bool shiftCategory(int dir)
    {
        
        bool ret = false;
//        if(currentPage<pages.size())
        {
            if(dir>0)
            {
                
                if(pages[currentPage]->index+dir > pages[currentPage]->images.size()-1)
                {
                    currentPage++;

                    ret = true;
                }
                else
                {
                    pages[currentPage]->index += dir;
                }
            }
            else if(dir<0)
            {
                
                if(pages[currentPage]->index+dir < 0)
                {
                    if(currentPage>0)currentPage--;
                    
                    ret = true;
                }
                else
                {
                    pages[currentPage]->index += dir;
                }
                
            }
            if(currentPage>=pages.size())
            {
                currentPage = pages.size()-1;
                
            }
            else if (currentPage>pages.size())
            {
                currentPage = 0;
            }
            pages[currentPage]->index = (pages[currentPage]->index > pages[currentPage]->images.size())?
                                            pages[currentPage]->images.size()-1:
                                            pages[currentPage]->index;
            ofLogVerbose() << "currentPage " <<currentPage << " pages[currentPage]->index " << pages[currentPage]->index;
        }
        
        return ret;
    }
    
    void update()
    {
        float dt = 1.0f / ofGetFrameRate();
        animation.update(dt);
        vector<Page*>::iterator it;
        
        position.x = animation.getCurrentValue();
        int pageIndex = 0;
        for(it = pages.begin() ; it!=pages.end() ; it++)
        {
            Page * p = *it;
            p->update(position);
//            if(pageIndex==currentPage && p->images.size()>1)
//            {
//                for(int i = 0 ; i < p->images.size() ; i++)
//                {
//                    if(currentIndex == i)
//                    {
//                        p->images[i]->bDraw = true;
//                    }
//                    else
//                    {
//                        p->images[i]->bDraw = false;
//                    }
//                }
//                
//            }
            pageIndex++;
        }
        
    }
    void draw()
    {
        vector<Page*>::iterator it;
        //        ofPushMatrix();
        //        ofTranslate(position);
        for(it = pages.begin() ; it!=pages.end() ; it++)
        {
            
            (*it)->draw();
            
        }
        //        ofPopMatrix;
        
        
    }

    int currentPage;
    vector<Page*> pages;
    int width;
    ofPoint position;
    int page_width;
    ofxAnimatableFloat animation;
    
    
};


#endif
