//
//  CustomRect.h
//  kirigami_server
//
//  Created by Wataru Ogasa on 2015/06/20.
//
//

#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

#include <cstdlib>
#include <dirent.h>

class CustomRect : public ofxBox2dRect {
    
private:
    ofImage img;
    
public:
    CustomRect();
    void draw();
    float customRectWidth, customRectHeight;
    void setImage(ofImage img);
    void setsize(int h,int w);
    ofImage getImage();
    Boolean on_cursor=false;
    int on_time=0;
    Boolean alive=true;
    int team=0;
    void setNumbers(int n,int t,int c);
    
    int c_answer=0;
    Boolean correct=false;
    int picnum=0;
    
    Boolean check=false;
    int check_time=0;
    
    ofImage maru;
    ofImage batu;
    Boolean ans=false;
    Boolean c_ans=false;
    
    Boolean on_cur=false;
    ofSoundPlayer  sound[3];
    Boolean secheck=false;
};


