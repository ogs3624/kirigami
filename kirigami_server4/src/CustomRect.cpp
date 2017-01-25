//
//  CustomRect.cpp
//  kirigami_server
//
//  Created by Wataru Ogasa on 2015/06/20.
//
//

#include "CustomRect.h"

CustomRect::CustomRect() {
    maru.loadImage("maru.png");
    batu.loadImage("batu.png");
    on_cursor=false;
    on_time=0;
    alive=true;
    team=0;
    c_answer=0;
    correct=false;
    picnum=0;
    check=false;
    check_time=0;
    ans=false;
    c_ans=false;
    on_cur=false;
    secheck=false;
    sound[0].loadSound("sounds/button02b.mp3");
    sound[0].setVolume(0.3f);
    sound[0].setMultiPlay(true);
    sound[1].loadSound("sounds/correct1.mp3");
    sound[1].setVolume(0.3f);
    sound[1].setMultiPlay(true);
    sound[2].loadSound("sounds/incorrect1.mp3");
    sound[2].setVolume(0.3f);
    sound[2].setMultiPlay(true);
}

void CustomRect::draw() {
    //座標を変更
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    glPushMatrix();
    //物体の位置に座標を移動
    glTranslatef(getPosition().x, getPosition().y, 0);
    ofFill(); //色を塗り潰す
    ofRotate(getRotation());
    ofSetColor(255,255,255);
    //ofRect(0,0,customRectWidth,customRectHeight);
    img.draw(0, 0, customRectWidth, customRectHeight);
    //座標を復元
    
    ofSetColor(255, 255, 0);
    
    if (on_cursor) {
        ofLine(-customRectWidth/2, -customRectHeight/2, -customRectWidth/2, customRectHeight/2);
        ofLine(-customRectWidth/2, customRectHeight/2, customRectWidth/2, customRectHeight/2);
        ofLine(customRectWidth/2, customRectHeight/2, customRectWidth/2, -customRectHeight/2);
        ofLine(customRectWidth/2, -customRectHeight/2, -customRectWidth/2, -customRectHeight/2);
        if(ans==false){
            on_time++;
        }
        
        if(on_cur==false){
            on_cur=true;
            sound[0].play();
        }
        
        //printf("%d",picnum);
    }else{
        if(on_time>0){
            on_time--;
        }
        on_cur=false;
    }
    if(on_time<=20){
        ofRect(-customRectWidth/2+(ofMap(on_time, 0, 20, 0, customRectWidth)/2), -customRectHeight/2,ofMap(on_time, 0, 20, 0, customRectWidth), 3);
    }else if(on_time>20&&on_time<=40){
        ofRect(0, -customRectHeight/2,customRectWidth, 3);
        ofRect(customRectHeight/2,-customRectWidth/2+(ofMap(on_time, 20, 40, 0, customRectWidth)/2),3,ofMap(on_time, 20, 40, 0, customRectWidth));
    }else if(on_time>40&&on_time<=60){
        ofRect(0, -customRectHeight/2,customRectWidth, 3);
        ofRect(customRectHeight/2, 0,3,customRectWidth);
        ofRect(customRectWidth/2-(ofMap(on_time, 40, 60, 0, customRectWidth)/2),customRectHeight/2,
               ofMap(on_time, 40, 60, 0, customRectWidth),3);
    }else if(on_time>60&&on_time<=80){
        ofRect(0, -customRectHeight/2,customRectWidth, 3);
        ofRect(customRectHeight/2, 0,3,customRectWidth);
        ofRect(0, customRectHeight/2,customRectWidth, 3);
        ofRect(-customRectWidth/2,customRectHeight/2-(ofMap(on_time, 60, 80, 0, customRectWidth)/2),
               3,ofMap(on_time, 60, 80, 0, customRectWidth));
    }else if(on_time==81){
        //alive=false;
        check=true;
        on_time=0;
    }
    
    if(check==true){
        /*
         ofRect(0, -customRectHeight/2,customRectWidth, 3);
         ofRect(customRectHeight/2, 0,3,customRectWidth);
         ofRect(0, customRectHeight/2,customRectWidth, 3);
         ofRect(-customRectHeight/2, 0,3,customRectWidth);
         */
        
        check_time++;
        if(c_answer==picnum){
            ans=true;
            if(secheck==false){
                secheck=true;
                sound[1].play();
            }
            
            if(check_time<=5){
                ofSetColor(255,255,255);
                maru.draw(0,0, 100, 100);
            }else if(check_time>8&&check_time<=12){
                ofSetColor(255,255,255);
                maru.draw(0,0, 100, 100);
            }else if(check_time>16&&check_time<=45){
                ofSetColor(255,255,255);
                maru.draw(0,0, 100, 100);
            }else if(check_time==45){
                check=false;
                check_time=0;
                secheck=false;
            }
        }else{
            if(secheck==false){
                secheck=true;
                sound[2].play();
            }
            if(check_time<=30){
                ofSetColor(255,255,255);
                batu.draw(0,0, 100, 100);
            }else if(check_time==31){
                check=false;
                check_time=0;
                secheck=false;
            }
        }
        
    }
    
    glPopMatrix();
    
    
    
}

void CustomRect::setImage(ofImage _img){
    img = _img;
}

ofImage CustomRect::getImage(){
    return img;
}

void CustomRect::setsize(int w,int h){
    customRectHeight=h;
    customRectWidth=w;
}
void CustomRect::setNumbers(int n,int t,int c){
    picnum=n;
    team=t;
    c_answer=c;
}
