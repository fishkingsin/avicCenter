/*
 *  Page.h
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

#ifndef Zone4Main_Page_h
#define Zone4Main_Page_h
#include "MyImages.h"
class Page{
public:
    Page()
    {
        index = 0;
    }
    void update(ofPoint p)
    {
        
        point = p;
        vector<MyImages*>::iterator it;    
        for(it = images.begin() ; it!=images.end() ; it++)
        {
            MyImages* img= (*it);
            img->layerPosition = p;
            ofPoint curX = img->position + img->layerPosition;
            int imgWidth = 4096;


            if(curX.x < ofGetWidth()+imgWidth && curX.x > -imgWidth)
            {
//                            ofLogVerbose() << img->fn << " " << curX << "Draw";
                img->bDraw = true;

            }
            else if(curX.x < ofGetWidth()+imgWidth*1.1 && curX.x > -imgWidth*1.1)
            {
//                            ofLogVerbose() << img->fn << " " << curX << "dont Draw";
                img->bDraw = false;
                {
                    img->load();
                    break;
                }

            }
            else
            {
                img->unload();
            }
        }
    }
    void draw()
    {
//        vector<MyImages*>::iterator it;
        
        
        int x = 0;
        if(index<images.size())images[index]->draw();

//        for(it = images.begin() ; it!=images.end() ; it++)
//        {
//            MyImages* img= (*it);
//            
//            
//            img->draw();
//            
//        }
        
        
    }
public:
    int index;
    ofPoint point;
    vector <MyImages*> images;

    
};


#endif
