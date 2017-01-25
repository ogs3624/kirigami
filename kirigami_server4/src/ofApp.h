#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "CustomRect.h"
#include "Particle.h"
#include "ofxFTP.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxBox2d                                box2d;
    ofxBox2dEdge                 ground1,ground2,ground3,ground4,ground5;
    
    ofDirectory dir;
    vector<ofImage> images;
    vector<Boolean> graycheck;
    
    vector <CustomRect *> rects;
    vector <Particle *> particle;
    
    int game_scene=0;
    int scenetime1=0,scenetime2=0,scenetime4=0;
    int correct_answer=0;
    int randpic=0;
    Boolean scene3check=false;
    Boolean first_check=false;
    Boolean sound3check=false;
    Boolean sound4check=false;
    
    int show_number[10];
    int showrects=5;
    int rect_w=80;
    int rect_h=80;
    
    int team1point=0,team2point=0;
    Boolean team1flag=false,team2flag=false;
    Boolean settle=false;
    int winner=0;
    
    ofImage hosi;
    ofImage hosiwaku;
    
    ofxFTPClient client;
    vector<string> picname;
    
    ofSoundPlayer  sound[10];
    ofSoundPlayer  bgm;
    
    Boolean showimages=false;
    Boolean gray=false;
    Boolean particle_tf=true;
    
    
    //カメラ関係
    ofVideoGrabber vidGrabber;
    
    ofxCvColorImage colorImg; //オリジナルのカラー映像
    ofxCvGrayscaleImage grayImage; //グレースケールに変換後
    ofxCvGrayscaleImage grayBg; //キャプチャーした背景画像
    ofxCvGrayscaleImage grayDiff; //現在の画像と背景との差分
    ofxCvContourFinder contourFinder; //輪郭抽出のためのクラス
    
    ofPixels pixels;
    
    bool bLearnBakground; //背景画像を登録したか否か
    bool showCvAnalysis; //解析結果を表示するか
    int threshold; //2値化の際の敷居値
    bool showvideo; //表示する画像を選択
    int videowidth,videoheight;
    
    float videoscale;
    int videoX,videoY;
    int video_reX,video_reY;
    
    Boolean quarter;
    
    int show_w,show_h;
    
    int resentNum;
    
};
