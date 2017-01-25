//
//  Particle.h
//  kirigami_server3
//
//  Created by Wataru Ogasa on 2015/08/05.
//
//

#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

//ofxBox2dCircleを継承したクラスParticleを定義
class Particle : public ofxBox2dCircle {
public:
    Particle(); //コンストラクタ
    void update(); //カウンタ更新
    void draw(); //円を描画する
    void setccolor(ofColor col);
    void setdeadtime(int time);
    float counter; //カウンタ
    float phase; //初期位相
    int lifeTime; //表示される長さ
    bool dead;//生死の判定
    ofColor ccolor;
    
};
