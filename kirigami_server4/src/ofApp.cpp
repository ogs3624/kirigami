#include "ofApp.h"

ofTrueTypeFont font,font2,font3;

struct S{
    string text;
    int x, y;
    ofRectangle rect;
};
S num,num2,num3;

inline int GetNextString(const string &src, string &sub, int pos)
{
    bool extracted = false;
    
    size_t i = src.find(" ", pos);
    if(i == string::npos){
        if(!extracted) sub = src.substr(pos, string::npos);
        return (int)string::npos;
    }
    else{
        int cnt = 1;
        while(src[i+cnt] == ' '){
            cnt++;
        }
        if(!extracted) sub = src.substr(pos, i-pos);
        return (int)(i+cnt >= src.size() ? (int)string::npos : i+cnt);
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableAntiAliasing();
    ofSetFrameRate(30);
    ofBackgroundHex(0x1F2C30);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetCircleResolution(64); //円の解像度設定
    
    ofToggleFullscreen();
    
    box2d.init();
    box2d.setGravity(0, 0);
    box2d.setFPS(30.0);
    //box2d.registerGrabbing();
    
    
    //フォントデータの読み込み
    font.loadFont("fonts/KozGoPro-Regular.otf", 80);
    num.text = "Hello!!";
    num.x = 0;
    num.y = 0;
    //フォントを取り囲む長方形を取得
    num.rect = font.getStringBoundingBox("Hello!!.", 0, 0);
    
    font2.loadFont("fonts/KozGoPro-Regular.otf", 24);
    num2.text = "Hello!!";
    num2.x = 0;
    num2.y = 0;
    //フォントを取り囲む長方形を取得
    num2.rect = font2.getStringBoundingBox("Hello!!.", 0, 0);
    
    font3.loadFont("fonts/scriptpersonaluseonly.otf", 60);
    num3.text = "Hello!!";
    num3.x = 0;
    num3.y = 0;
    //フォントを取り囲む長方形を取得
    num3.rect = font3.getStringBoundingBox("Hello!!.", 0, 0);
    
    
    //load Image
    dir.listDir("pics/");
    dir.sort();
    //allocate the vector to have as many ofImages as file
    /*
    if( dir.size() ){
        images.assign(dir.size(), ofImage());
    }
    */
    
    int imgcount=0;
    for(int i = 0; i < (int)dir.size(); i++){
        ofImage pre_img;
        if(pre_img.loadImage(dir.getPath(i))){
            //printf("correct!! %s \n",dir.getPath(i).c_str());
            
            //色を取得して、グレーの閾値より低かったらgraycheckがtrueになる
            ofImage pre_img_;
            pre_img_=pre_img;
            pre_img_.setImageType(OF_IMAGE_GRAYSCALE);
            
            unsigned char *data= pre_img_.getPixels();
            int posX=0, posY=0;
            int width = pre_img_.width;
            int height = pre_img_.height;
            Boolean escape=false;
            
            for(int j=0;j<width;j++){
                for(int k=0;k<height;k++){
                    if(data[k * width + j]!=0){
                        //printf("%s,%d\n",dir.getPath(i).c_str(),data[k * width + j]);
                        Boolean graycheck_=false;
                        if(data[k * width + j]<70){
                            graycheck_=true;
                        }
                        graycheck.push_back(graycheck_);
                        
                        //data[k * width + j]=255;
                        escape=true;
                        break;
                    }
                }
                if(escape){
                    break;
                }
            }
            
            /*
            for(int j=0;j<width;j++){
                for(int k=0;k<height;k++){
                    if(data[(k * width + j) * 3]==0&&
                       data[(k * width + j) * 3 + 1]==0&&
                       data[(k * width + j) * 3 + 2]==0){
                    }else{
                        
                        printf("%s,%d,%d,%d,%f\n",dir.getPath(i).c_str(),
                               data[(k * width + j) * 3],
                               data[(k * width + j) * 3 + 1],
                               data[(k * width + j) * 3 + 2],
                               0.299*data[(k*width+j)*3]+0.587*data[(k*width+j)*3+1]+0.114*data[(k*width+j)*3+2]);
                        
                        //グレーの閾値70より低かったら判定
                        Boolean graycheck_=false;
                        if(0.299*data[(k*width+j)*3]+0.587*data[(k*width+j)*3+1]+0.114*data[(k*width+j)*3+2]<70){
                            graycheck_=true;
                        }
                        graycheck.push_back(graycheck_);
                        escape=true;
                        break;
                    }
                }
                if(escape){
                    break;
                }
            }
            */

            
            images.push_back(pre_img);
        }else{
            printf("incorrect!! %s \n",dir.getPath(i).c_str());
        }
    }
    
    //場の線等
    float r=360;
    for(float i=0;i<=6.3;i+=0.01){
        float x=cos(i)*r+ofGetWidth()/2;
        float y=sin(i)*r+ofGetHeight()/2;
        ground1.addVertex(x, y);
    }
    ground1.create(box2d.getWorld());
    
    float rr=60;
    for(float i=0;i<=6.3;i+=0.01){
        float x=cos(i)*rr+ofGetWidth()/2;
        float y=sin(i)*rr+ofGetHeight()/2;
        ground2.addVertex(x, y);
    }
    ground2.create(box2d.getWorld());
    
    ground3.addVertex(ofGetWidth()/2-360,ofGetHeight()/2);
    ground3.addVertex(ofGetWidth()/2-60,ofGetHeight()/2);
    ground3.create(box2d.getWorld());
    
    ground4.addVertex(ofGetWidth()/2+60,ofGetHeight()/2);
    ground4.addVertex(ofGetWidth()/2+360,ofGetHeight()/2);
    ground4.create(box2d.getWorld());
    
    
    //ftp setting
    client.setup("kgmsc2.sakura.ne.jp","kgmsc2","rmiyader2");
    client.setVerbose(true);
    
    
    //初期化
    for (int i=0; i<10; i++) {
        show_number[i]=0;
    }
    game_scene=0;
    scenetime1=0;
    scenetime2=0;
    correct_answer=0;
    randpic=0;
    scene3check=false;
    first_check=false;
    team1point=0;
    team2point=0;
    scenetime4=0;
    team1flag=false;
    team2flag=false;
    settle=false;
    winner=0;
    sound3check=false;
    sound4check=false;
    showimages=false;
    gray=false;
    showrects=5;
    particle_tf=true;
    rect_w=80;
    rect_h=80;
    quarter=false;
    show_w=70;
    show_h=70;
    resentNum=0;
    
    hosi.loadImage("hosi.png");
    hosiwaku.loadImage("hosiwaku.png");
    
    sound[0].loadSound("sounds/button03a.mp3");
    sound[0].setVolume(0.3f);
    sound[0].setMultiPlay(true);
    sound[1].loadSound("sounds/powerup10.mp3");
    sound[1].setVolume(0.3f);
    sound[1].setMultiPlay(true);
    sound[2].loadSound("sounds/onepoint14.mp3");
    sound[2].setVolume(0.3f);
    sound[2].setMultiPlay(true);
    sound[3].loadSound("sounds/system46.mp3");
    sound[3].setVolume(0.3f);
    sound[3].setMultiPlay(true);
    sound[4].loadSound("sounds/decision13.mp3");
    sound[4].setVolume(0.3f);
    sound[4].setMultiPlay(true);
    sound[5].loadSound("sounds/muci_fan_05.mp3");
    sound[5].setVolume(0.3f);
    sound[5].setMultiPlay(true);
    
    bgm.loadSound("sounds/river_019.mp3");
    bgm.setVolume(0.5f);
    bgm.setMultiPlay(true);
    bgm.setLoop(true);
    
    
    //カメラ関係
    vidGrabber.setDeviceID(1);
    //vidGrabber.initGrabber(ofGetWidth(), ofGetHeight());
    vidGrabber.initGrabber(600, 400);
    
    vidGrabber.setVerbose(true);
    //vidGrabber.listDevices();
    
    //使用する画像の領域を確保
    colorImg.allocate(600, 400);
    grayImage.allocate(600, 400);
    grayBg.allocate(600, 400);
    grayDiff.allocate(600, 400);
    
    showCvAnalysis=true;
    threshold=230;
    
    videowidth=ofGetWidth();
    videoheight=ofGetHeight();
    /*
    videowidth=600;
    videoheight=400;
    */
    videoscale=1.0;
    videoX=0;
    videoY=0;
    video_reX=0;
    video_reY=0;
    
    CustomRect *rect = new CustomRect();
    //物理特性の設定-重さ、反発力、摩擦力
    rect->setPhysics(0.8, 1, 0);
    rect->setImage(images[0]);
    rect->setup(box2d.getWorld(), ofGetWidth()/2+200, ofGetHeight()/2, rect_h,rect_w);
    rect->setsize(rect_w,rect_h);
    rect->setNumbers(0, 1, correct_answer);
    //力をランダムに加える
    float force = 700;
    rect->addForce(ofPoint(ofRandom(-force, force), ofRandom(-force, force)), 1);
    
    rects.push_back(rect);
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    box2d.update();
    
    for(vector <CustomRect *>::iterator it = rects.begin(); it != rects.end();){
        //(*it)->update();
        //もし寿命が尽きていたら、消去
        if ((*it)->alive==false) {
            (*it)->destroy();
            //オブジェクトを解放
            delete *it;
            //動的配列から、オブジェクトを削除
            it = rects.erase(it);
        } else {
            ++it;
        }
    }
    
    for(vector <Particle *>::iterator it = particle.begin(); it != particle.end();){
        //MoveCircleの状態を更新
        (*it)->update();
        //もし寿命が尽きていたら、MoveCircleを消去
        if ((*it)->dead) {
            //形態をBox2Dの世界から消去(今消してるけどいるかもしれん)
            //(*it)->destroy();
            //オブジェクトを解放
            delete *it;
            //動的配列から、オブジェクトを削除
            it = particle.erase(it);
        } else {
            ++it;
        }
    }
    
    if(particle_tf==true){
        for(int j=0;j<contourFinder.nBlobs;j++){
            //半径をランダムに決める
            float r = ofRandom(5, 7);
            //Box2Dの円のインスタンスを生成
            Particle *c = new Particle();
            //物理特性の設定-重さ、反発力、摩擦力
            c->setPhysics(1.0, 0.7, 0.5);
            //生存時間
            c->setdeadtime(10);
            //世界に追加
            c->setup(box2d.getWorld(),
                     ofMap(contourFinder.blobs[j].centroid.x, 0, 600, 0, videowidth*videoscale)+videoX,
                     ofMap(contourFinder.blobs[j].centroid.y, 0, 400, 0, videoheight*videoscale)+videoY,r);
            //色
            c->setccolor(ofColor(255,255,255));
            //力をランダムに加える
            float force = 20;
            c->addForce(ofPoint(ofRandom(-force, force), ofRandom(-force, force)), 1);
            //ベクターに追加
            particle.push_back(c);
        }
    }
    
    //半径をランダムに決める
    float r = ofRandom(5, 7);
    //Box2Dの円のインスタンスを生成
    Particle *c = new Particle();
    //物理特性の設定-重さ、反発力、摩擦力
    c->setPhysics(1.0, 0.7, 0.5);
    //生存時間
    c->setdeadtime(10);
    //世界に追加
    c->setup(box2d.getWorld(),
             mouseX,
             mouseY,r);
    //色
    c->setccolor(ofColor(255,255,255));
    //力をランダムに加える
    float force = 20;
    c->addForce(ofPoint(ofRandom(-force, force), ofRandom(-force, force)), 1);
    //ベクターに追加
    particle.push_back(c);
    
    
    
    //新規フレームの取り込みをリセット
    bool bNewFrame = false;
    //カメラ使用の場合はカメラから新規フレーム取り込み
    vidGrabber.update();
    //新規にフレームが切り替わったか判定
    bNewFrame = vidGrabber.isFrameNew();
    
    //フレームが切り替わった際のみ画像を解析
    if (bNewFrame){
        //取り込んだフレームを画像としてキャプチャ
        colorImg.setFromPixels(vidGrabber.getPixels(), 600,400);
        //左右反転
        //colorImg.mirror(false, true);
        
        //カラーのイメージをグレースケールに変換
        grayImage = colorImg;
        
        //まだ背景画像が記録されていなければ、現在のフレームを背景画像とする
        if (bLearnBakground == true){
            grayBg = grayImage;
            bLearnBakground = false;
        }
        
        //グレースケールのイメージと取り込んだ背景画像との差分を算出
        grayDiff.absDiff(grayBg, grayImage);
        //画像を2値化(白と黒だけに)する
        grayDiff.threshold(threshold);
        //2値化した画像から輪郭を抽出する
        contourFinder.findContours(grayDiff, 25, grayDiff.width * grayDiff.height, 10, false, false);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    //myImage.draw(0, 0);
    
    if(showvideo==true){
        colorImg.draw((videowidth*videoscale)/2+videoX, (videoheight*videoscale)/2+videoY,
                      videowidth*videoscale, videoheight*videoscale);
    }
    
    
    //検出した解析結果を表示
    for (int i = 0; i < contourFinder.nBlobs; i++){
        ofPushMatrix();
        
        glTranslatef(videoX,videoY,0);
        //画面サイズいっぱいに表示されるようリスケール
        glScalef((float)videowidth*videoscale / (float)grayDiff.width,
                (float)videoheight*videoscale / (float)grayDiff.height,
                1.0f);
        if (showCvAnalysis) {
            contourFinder.blobs[i].draw(0,0);
        }
        ofFill();
        ofSetColor(255, 0, 0);
        ofEllipse(contourFinder.blobs[i].centroid.x,
                    contourFinder.blobs[i].centroid.y, 4, 4);
        ofPopMatrix();
    }
    
    
    
    //決着時の赤(一番裏側に書きたいのでここ)
    if(settle==true){
        ofSetColor(255, 0, 0, 100);
        ofCircle(ofGetWidth()/2,ofGetHeight()/2,360);
        ofSetHexColor(0x1F2C30);
        ofCircle(ofGetWidth()/2,ofGetHeight()/2,60);
        //ofSetColor(0,255,0);
        if(winner==1){
            ofRect( ofGetWidth()/2, ofGetHeight()/2-200, 1000, 400);
        }else if(winner==2){
            ofRect( ofGetWidth()/2, ofGetHeight()/2+200, 1000, 400);
        }
    }
    
    
    
    ofSetColor(255);
    
    //ゲームシーン
    //真ん中にランダム(スピード調整必要)、初期化
    if(game_scene==1){
        //初期化
        correct_answer=ofRandom(0, (int)images.size());
        
        //足した
        if(resentNum!=0){
            correct_answer=(int)images.size()-1;
            
            //resentNum=0;
        }
        
        
        
        
        for (int i=1;i<showrects; i++) {
            int n=ofRandom(0, (int)images.size());
            if(n==correct_answer){
                if(correct_answer==(int)images.size()-1){
                    n--;
                }else{
                    n++;
                }
            }
            show_number[i]=n;
        }
        
        show_number[0]=correct_answer;
        
        if(first_check==false){
            for (int i=0; i<rects.size(); i++) {
                rects[i]->alive=false;
            }
            scene3check=false;
            first_check=true;
            
            
        }
        
        
        if(scenetime1<45){
            randpic=ofRandom(0, (int)images.size());
            //images[randpic].draw(ofGetWidth()/2-25, ofGetHeight()/2-25, 50, 50);
            if(quarter==true){
                images[randpic].draw(ofGetWidth()/2-show_w/2, ofGetHeight()/2-show_h/2, show_w*2, show_h*2);
                ofSetHexColor(0x1F2C30);
                ofRect(ofGetWidth()/2-show_w, ofGetHeight()/2-show_h/2, show_w,show_h*2);
                ofRect(ofGetWidth()/2, ofGetHeight()/2-show_h, show_w,show_h);
                ofSetColor(255);
            }else{
                images[randpic].draw(ofGetWidth()/2, ofGetHeight()/2, show_w, show_h);
            }
            sound[0].play();
        }else if(scenetime1>=45&&scenetime1<120){
            if(scenetime1%3==0){
                randpic=ofRandom(0, (int)images.size());
                sound[0].play();
            }
            //images[randpic].draw(ofGetWidth()/2-25, ofGetHeight()/2-25, 50, 50);
            if(quarter==true){
                images[randpic].draw(ofGetWidth()/2-show_w/2, ofGetHeight()/2-show_h/2, show_w*2, show_h*2);
                ofSetHexColor(0x1F2C30);
                ofRect(ofGetWidth()/2-show_w, ofGetHeight()/2-show_h/2, show_w,show_h*2);
                ofRect(ofGetWidth()/2, ofGetHeight()/2-show_h, show_w,show_h);
                ofSetColor(255);
            }else{
                images[randpic].draw(ofGetWidth()/2, ofGetHeight()/2, show_w, show_h);
            }
            
        }else if(scenetime1>=120&&scenetime1<180){
            if(scenetime1%10==0){
                randpic=ofRandom(0, (int)images.size());
                sound[0].play();
            }
            //images[randpic].draw(ofGetWidth()/2-25, ofGetHeight()/2-25, 50, 50);
            if(quarter==true){
                images[randpic].draw(ofGetWidth()/2-show_w/2, ofGetHeight()/2-show_h/2, show_w*2, show_h*2);
                ofSetHexColor(0x1F2C30);
                ofRect(ofGetWidth()/2-show_w, ofGetHeight()/2-show_h/2, show_w,show_h*2);
                ofRect(ofGetWidth()/2, ofGetHeight()/2-show_h, show_w,show_h);
                ofSetColor(255);
            }else{
                images[randpic].draw(ofGetWidth()/2, ofGetHeight()/2, show_w, show_h);
            }
            
        }else if(scenetime1==180){
            //images[correct_answer].draw(ofGetWidth()/2-25, ofGetHeight()/2-25, 50, 50);
            //images[correct_answer].draw(ofGetWidth()/2, ofGetHeight()/2, show_w, show_h);
            if(quarter==true){
                images[correct_answer].draw(ofGetWidth()/2-show_w/2, ofGetHeight()/2-show_h/2, show_w*2, show_h*2);
                ofSetHexColor(0x1F2C30);
                ofRect(ofGetWidth()/2-show_w, ofGetHeight()/2-show_h/2, show_w,show_h*2);
                ofRect(ofGetWidth()/2, ofGetHeight()/2-show_h, show_w,show_h);
                ofSetColor(255);
            }else{
                images[correct_answer].draw(ofGetWidth()/2, ofGetHeight()/2, show_w, show_h);
            }
            
            //足した
            //printf("%d!!!!!,%d",resentNum,(int)images.size());
            if(resentNum!=0){
                if(resentNum>8){
                    resentNum=8;
                }
                for(int i=0;i<resentNum;i++){
                    show_number[i+1]=(int)images.size()-(i+2);
                }
                resentNum=0;
            }
            
            
            sound[1].play();
            scenetime1=0;
            game_scene=2;
        }
        scenetime1++;
    }else{
        if(quarter==true){
            images[correct_answer].draw(ofGetWidth()/2-show_w/2, ofGetHeight()/2-show_h/2, show_w*2, show_h*2);
            ofSetHexColor(0x1F2C30);
            ofRect(ofGetWidth()/2-show_w, ofGetHeight()/2-show_h/2, show_w,show_h*2);
            ofRect(ofGetWidth()/2, ofGetHeight()/2-show_h, show_w,show_h);
            ofSetColor(255);
        }else{
            images[correct_answer].draw(ofGetWidth()/2, ofGetHeight()/2, show_w, show_h);
        }
    }
    
    //カウントダウン
    if (game_scene==2) {
        if (scenetime2>=60&&scenetime2<90) {
            string str;
            str="3";
            num.rect = font.getStringBoundingBox(str, 0, 0);
            if (sound3check==false) {
                sound[2].play();
                sound3check=true;
            }
            
            ofPushMatrix();
            ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
            ofRotateZ(0);
            font.drawString(str, -num.rect.width/2, num.rect.height/2+200);
            ofRotateZ(180);
            font.drawString(str, -num.rect.width/2, num.rect.height/2+200);
            ofPopMatrix();
            
        }else if (scenetime2>=90&&scenetime2<120){
            string str;
            str="2";
            //ofDrawBitmapString(str, 10,240);
            num.rect = font.getStringBoundingBox(str, 0, 0);
            if (sound3check==true) {
                sound[2].play();
                sound3check=false;
            }
            
            ofPushMatrix();
            ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
            ofRotateZ(0);
            font.drawString(str, -num.rect.width/2, num.rect.height/2+200);
            ofRotateZ(180);
            font.drawString(str, -num.rect.width/2, num.rect.height/2+200);
            ofPopMatrix();
        }else if (scenetime2>=120&&scenetime2<150){
            string str;
            str="1";
            //ofDrawBitmapString(str, 10,240);
            num.rect = font.getStringBoundingBox(str, 0, 0);
            if (sound3check==false) {
                sound[2].play();
                sound3check=true;
            }
            
            ofPushMatrix();
            ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
            ofRotateZ(0);
            font.drawString(str, -num.rect.width/2, num.rect.height/2+200);
            ofRotateZ(180);
            font.drawString(str, -num.rect.width/2, num.rect.height/2+200);
            ofPopMatrix();
        }else if (scenetime2>=150&&scenetime2<200){
            string str;
            ofSetColor(255, 160, 122);
            str="Start!!";
            //ofDrawBitmapString(str, 10,240);
            num.rect = font.getStringBoundingBox(str, 0, 0);
            if (sound3check==true) {
                sound[3].play();
                sound3check=false;
            }
            
            ofPushMatrix();
            ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
            ofRotateZ(0);
            font.drawString(str, -num.rect.width/2, num.rect.height/2+200);
            ofRotateZ(180);
            font.drawString(str, -num.rect.width/2, num.rect.height/2+200);
            ofPopMatrix();
            ofSetColor(255, 255, 255);
        }else if(scenetime2==200){
            scenetime2=0;
            game_scene=3;
            sound3check=false;
        }
        
        scenetime2++;
    }
    
    //ゲーム開始
    if (game_scene==3) {
        if(scene3check==false){
            //チーム1に四角形5つ
            for(int i=0;i<showrects;i++){
                
                CustomRect *rect = new CustomRect();
                //物理特性の設定-重さ、反発力、摩擦力
                rect->setPhysics(0.8, 1, 0);
                ofImage image_;
                image_=images[show_number[i]];
                if(gray==true){
                    image_.setImageType(OF_IMAGE_GRAYSCALE);
                        if(graycheck[show_number[i]]==true){
                            unsigned char *data= image_.getPixels();
                            int posX=0, posY=0;
                            int width = image_.width;
                            int height = image_.height;
                            printf("%d,%d\n",width,height);
                            for(int j=0;j<width;j++){
                                for(int k=0;k<height;k++){
                                    //pixels[k*3 * width + j]
                                    if(data[k * width + j]==0){
                                        data[k * width + j]=255;
                                    }
                                }
                            }
                        }
                }
                rect->setImage(image_);
                rect->setup(box2d.getWorld(), ofGetWidth()/2+ofRandom(-5,5), ofGetHeight()/2+200, rect_w, rect_h);
                rect->setsize(rect_w, rect_h);
                rect->setNumbers(show_number[i], 1, correct_answer);
                //力をランダムに加える
                float force = 700;
                rect->addForce(ofPoint(ofRandom(-force, force), ofRandom(-force, force)), 1);
                
                rects.push_back(rect);
            }
            //チーム2に四角形5つ
            for(int i=0;i<showrects;i++){
                
                CustomRect *rect = new CustomRect();
                //物理特性の設定-重さ、反発力、摩擦力
                rect->setPhysics(0.8, 1, 0);
                ofImage image_;
                image_=images[show_number[i]];
                if(gray==true){
                    image_.setImageType(OF_IMAGE_GRAYSCALE);
                    if(graycheck[show_number[i]]==true){
                        unsigned char *data= image_.getPixels();
                        int posX=0, posY=0;
                        int width = image_.width;
                        int height = image_.height;
                        printf("%d,%d\n",width,height);
                        for(int j=0;j<width;j++){
                            for(int k=0;k<height;k++){
                                //pixels[k*3 * width + j]
                                if(data[k * width + j]==0){
                                    data[k * width + j]=255;
                                }
                            }
                        }
                    }
                }
                rect->setImage(image_);
                rect->setup(box2d.getWorld(), ofGetWidth()/2+ofRandom(-5,5), ofGetHeight()/2-200, rect_w, rect_h);
                rect->setsize(rect_w, rect_h);
                rect->setNumbers(show_number[i], 2, correct_answer);
                //力をランダムに加える
                float force = 700;
                rect->addForce(ofPoint(ofRandom(-force, force), ofRandom(-force, force)), 1);
                
                rects.push_back(rect);
            }
            scene3check=true;
            first_check=true;
            bgm.play();
        }
        for (int i=0; i<rects.size(); i++) {
            if(rects[i]->ans==true){
                for (int j=0; j<rects.size(); j++) {
                    if(j!=i){
                        rects[j]->alive=false;
                    }
                }
                if(rects[i]->team==1){
                    team1flag=true;
                }
                if(rects[i]->team==2){
                    team2flag=true;
                }
                game_scene=4;
                bgm.stop();
            }
        }
    }
    
    //ゲーム続行or終了
    if (game_scene==4) {
        scenetime4++;
        if(scenetime4==70){
            if(team1flag==true){
                team1point++;
                team1flag=false;
                sound[4].play();
            }
            if(team2flag==true){
                team2point++;
                team2flag=false;
                sound[4].play();
            }
        }
        if(scenetime4==110){
            if(team1point>=3){
                winner=1;
            }
            if(team2point>=3){
                winner=2;
            }
            if(winner==0){
                for (int i=0; i<rects.size(); i++) {
                    rects[i]->alive=false;
                }
            }
        }
        if(scenetime4>=110&&scenetime4<120){
            if(winner==0){
            string str;
            str="Loading...";
            //ofDrawBitmapString(str, 10,240);
            num2.rect = font2.getStringBoundingBox(str, 0, 0);
            
            ofPushMatrix();
            ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
            ofRotateZ(0);
            font2.drawString(str, -num2.rect.width/2, num2.rect.height/2+200);
            ofRotateZ(180);
            font2.drawString(str, -num2.rect.width/2, num2.rect.height/2+200);
            ofPopMatrix();
            }
        }
        if(scenetime4==115){
            //サーバーから取得
            if(winner==0){
                vector<string> str;
                str = client.list("/home/kgmsc2/www/kirigami_pics");
                
                if(str[0]!="notconnected"){
                    //名前検索(重複削除など)
                    for(int i=0;i<str.size();i++){
                        //printf("%s\n",str[i].c_str());
                        string sub;
                        int pos = 0;
                        do{
                            pos = GetNextString(str[i], sub, pos);
                            //cout << sub << endl;
                            unsigned int loc = sub.find( ".png", 0 );
                            if( loc != string::npos ){
                                //printf("%s\n",sub.c_str());
                                Boolean samecheck=false;
                                for(int i=0;i<picname.size();i++){
                                    if(picname[i]==sub){
                                        samecheck=true;
                                    }
                                }
                                if(samecheck==false){
                                    picname.push_back(sub);
                                    //printf("%s",sub.c_str());
                                }
                                
                            }
                        }while(pos != string::npos);
                    }
                    
                    //その名前の切り紙をサーバーから取得
                    for(int i=0;i<picname.size();i++){
                        Boolean samedir=false;
                        for(int j = 0; j < (int)dir.size(); j++){
                            if(picname[i]==dir.getPath(j).erase(0, 5)){
                                //printf("%s!!,%s!!\n",picname[i].c_str(),dir.getPath(j).erase(0, 5).c_str());
                                samedir=true;
                            }else{
                                //printf("%s,%s\n",picname[i].c_str(),dir.getPath(j).erase(0, 5).c_str());
                                //samedir=true;
                            }
                        }
                        if(samedir==false){
                            printf("%s\n",picname[i].c_str());
                            
                            client.get(picname[i],"pics/","/home/kgmsc2/www/kirigami_pics");
                            dir.listDir("pics/");
                            dir.sort();
                            //printf("%d",dir.size());
                            ofImage newimage;
                            if(newimage.loadImage("pics/"+picname[i])){
                                //printf("correct!! %s \n",dir.getPath(i).c_str());
                                images.push_back(newimage);
                                
                                
                                //色を取得して、グレーの閾値より低かったらgraycheckがtrueになる
                                ofImage pre_img_;
                                pre_img_=newimage;
                                pre_img_.setImageType(OF_IMAGE_GRAYSCALE);
                                
                                unsigned char *data= pre_img_.getPixels();
                                int posX=0, posY=0;
                                int width = pre_img_.width;
                                int height = pre_img_.height;
                                Boolean escape=false;
                                
                                for(int j=0;j<width;j++){
                                    for(int k=0;k<height;k++){
                                        if(data[k * width + j]!=0){
                                            //printf("%s,%d\n",dir.getPath(i).c_str(),data[k * width + j]);
                                            Boolean graycheck_=false;
                                            if(data[k * width + j]<70){
                                                graycheck_=true;
                                            }
                                            graycheck.push_back(graycheck_);
                                            
                                            //data[k * width + j]=255;
                                            escape=true;
                                            break;
                                        }
                                    }
                                    if(escape){
                                        break;
                                    }
                                }
                                
                                
                                
                                
                                resentNum++;
                            }else{
                                printf("incorrect!! %s \n",dir.getPath(i).c_str());
                            }
                            
                            /*
                             newimage.loadImage("pics/"+picname[i]);
                             images.push_back(newimage);
                             */
                        }
                    }
                
                }
                
            }
        }
        
        if(scenetime4>=130){
            //game_scene=1;
            //scenetime4=0;
            if(winner!=0){
                if(sound4check==false){
                    sound4check=true;
                    sound[5].play();
                }
                string str,str2;
                str="WIN!!";
                str2="LOSE...";
                //ofDrawBitmapString(str, 10,240);
                num3.rect = font3.getStringBoundingBox(str, 0, 0);
                quarter=false;
                
                ofPushMatrix();
                ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
                ofRotateZ(0);
                if(winner==1){
                    font3.drawString(str, -num3.rect.width/2, num3.rect.height/2+200);
                    //winner=1;
                }else if(winner==2){
                    font3.drawString(str2, -num3.rect.width/2, num3.rect.height/2+200);
                    //winner=2;
                }
                ofRotateZ(180);
                if(winner==1){
                    font3.drawString(str2, -num3.rect.width/2, num3.rect.height/2+200);
                }else if(winner==2){
                    font3.drawString(str, -num3.rect.width/2, num3.rect.height/2+200);
                }
                ofPopMatrix();
                
                settle=true;
                
                
            }else{
                game_scene=1;
                first_check=false;
                scenetime4=0;
            }
        }
    }
    
    
    ofSetColor(255);
    //切り絵描画
    for(int i=0; i<rects.size(); i++){
        ofFill();
        //ofSetHexColor(0xe63b8b);
        rects[i]->draw();
    }
    
    
    ofSetColor(255);
    
    //得点、左がteam1、右がteam2
    string str1;
    string str2;
    str1="1:"+ofToString(team1point);
    str2="2:"+ofToString(team2point);
    //ofDrawBitmapString(str, 10,240);
    num2.rect = font2.getStringBoundingBox(str1, 0, 0);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofRotateZ(0);
    //font2.drawString(str1, 320, 40);
    if(team1point==0){
        hosiwaku.draw(260,20, 32, 32);hosiwaku.draw(295,20, 32, 32);hosiwaku.draw(330,20, 32, 32);
    }else if(team1point==1){
        hosi.draw(260,20, 32, 32);hosiwaku.draw(295,20, 32, 32);hosiwaku.draw(330,20, 32, 32);
    }else if(team1point==2){
        hosi.draw(260,20, 32, 32);hosi.draw(295,20, 32, 32);hosiwaku.draw(330,20, 32, 32);
    }else if(team1point>=3){
        hosi.draw(260,20, 32, 32);hosi.draw(295,20, 32, 32);hosi.draw(330,20, 32, 32);
    }
    ofRotateZ(180);
    //font2.drawString(str2, 320, 40);
    if(team2point==0){
        hosiwaku.draw(260,20, 32, 32);hosiwaku.draw(295,20, 32, 32);hosiwaku.draw(330,20, 32, 32);
    }else if(team2point==1){
        hosi.draw(260,20, 32, 32);hosiwaku.draw(295,20, 32, 32);hosiwaku.draw(330,20, 32, 32);
    }else if(team2point==2){
        hosi.draw(260,20, 32, 32);hosi.draw(295,20, 32, 32);hosiwaku.draw(330,20, 32, 32);
    }else if(team2point>=3){
        hosi.draw(260,20, 32, 32);hosi.draw(295,20, 32, 32);hosi.draw(330,20, 32, 32);
    }
    
    ofPopMatrix();
    
    ofSetColor(255,255,255);
    
    
    Boolean allcheck=false;
    //四角形に対し、円形で判定
    //printf("%f,%f\n",rects[1]->getPosition().x,rects[1]->getPosition().y);
    for(int i=0; i<rects.size(); i++){
        allcheck=false;
        
        //マウスカーソル
        if(sqrt((rects[i]->getPosition().x-mouseX)*(rects[i]->getPosition().x-mouseX)+
                (rects[i]->getPosition().y-mouseY)*(rects[i]->getPosition().y-mouseY))<(40*sqrt(2))){
            rects[i]->on_cursor=true;
            allcheck=true;
        }else{
            rects[i]->on_cursor=false;
        }
        //printf("%d,%d\n",mouseX,mouseY);
        //LED棒
        for(int j=0;j<contourFinder.nBlobs;j++){
            if(allcheck==false){
                //printf("%f,%f\n",ofMap(contourFinder.blobs[j].centroid.x, 0, 600, 0, ofGetWidth())
                //       ,ofMap(contourFinder.blobs[j].centroid.y, 0, 400, 0, ofGetHeight()));
                
                if(sqrt((rects[i]->getPosition().x-(ofMap(contourFinder.blobs[j].centroid.x, 0, 600, 0, videowidth*videoscale)+videoX))*
                        (rects[i]->getPosition().x-(ofMap(contourFinder.blobs[j].centroid.x, 0, 600, 0, videowidth*videoscale)+videoX))+
                        (rects[i]->getPosition().y-(ofMap(contourFinder.blobs[j].centroid.y, 0, 400, 0, videoheight*videoscale)+videoY))*
                        (rects[i]->getPosition().y-(ofMap(contourFinder.blobs[j].centroid.y, 0, 400, 0, videoheight*videoscale)+videoY)))
                   <((rect_h/2)*sqrt(2))){
                    
                    rects[i]->on_cursor=true;
                    allcheck=true;
                }else{
                    rects[i]->on_cursor=false;
                }
            }
        }
        
    }
    /*
    string info5 = "";
    if(allcheck){
        info5 += "allcheck";
    }
    ofSetHexColor(0x444342);
    ofDrawBitmapString(info5, 30, 70);
    */
    
    //マウスカーソル
    ofSetColor(255,0,0);
    ofCircle(mouseX,mouseY,4);
    
    
    
    for(vector <Particle *>::iterator it = particle.begin(); it != particle.end(); ++it) {
        (*it)->draw();
    }
    
    string info = "";
    //info += "Total Bodies: "+ofToString(rects.size())+"\n";
    info += "show_number: ";
    for (int i=0; i<showrects; i++) {
        info += ofToString(show_number[i])+",";
    }
    
    ofSetHexColor(0x444342);
    ofDrawBitmapString(info, 30, 30);
    
    /*
    string info2 = "";
    info2 += "server_pics: ";
    for(int i=0;i<picname.size();i++){
        info2+=picname[i]+",";
    }
    ofSetHexColor(0x444342);
    ofDrawBitmapString(info2, 30, 40);
    */
    
    //gray確認
    if(gray==true){
        string info4 = "";
        info4 += "gray!";
        ofSetHexColor(0x444342);
        ofDrawBitmapString(info4, 30, 50);
    }
    
    
    //images出す
    ofSetColor(255, 255, 255);
    if(showimages==true){
        string info3 = "";
        for(int i=0;i<images.size();i++){
            int j=i/10;
            if(gray==true){
                images[i].setImageType(OF_IMAGE_GRAYSCALE);
                
                if(graycheck[i]==true){
                    unsigned char *data= images[i].getPixels();
                    int posX=0, posY=0;
                    int width = images[i].width;
                    int height = images[i].height;
                    printf("%d,%d\n",width,height);
                    for(int j=0;j<width;j++){
                        for(int k=0;k<height;k++){
                            //pixels[k*3 * width + j]
                            if(data[k * width + j]==0){
                                data[k * width + j]=255;
                            }
                        }
                    }
                }
                
                
            }
            images[i].draw(20+(i%10)*40, 20+j*40, 35, 35);
            info3+=dir.getPath(i)+"\n";
        }
        info3+="images.size= "+ofToString(images.size())+" dir.size="+ofToString(dir.size())+"\n";
        ofSetHexColor(0x444342);
        ofDrawBitmapString(info3, 600, 30);
    }
    
    ofSetColor(255);
    //線
    ground1.draw();
    ground2.draw();
    ground3.draw();
    ground4.draw();
    //ground5.draw();
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'b') {
        
    }else if(key=='f'){
        ofToggleFullscreen();
    }else if(key=='1'){
        showrects=5;
        rect_h=80;
        rect_w=80;
    }else if(key=='2'){
        showrects=10;
        rect_h=70;
        rect_w=70;
    }else if(key=='3'){
        quarter ? quarter=false : quarter=true;
    }else if(key=='z'){
        game_scene=1;
        scenetime4=0;
        team1point=0;
        team2point=0;
        settle=false;
        winner=0;
        first_check=false;
        sound4check=false;
        bgm.stop();
    }else if(key=='x'){
        game_scene=0;
    }else if(key=='c'){
        vector<string> str;
        str = client.list("/home/kgmsc2/www/kirigami_pics");
        //printf("%s",str[0].c_str());
        if(str[0]=="notconnected"){
            for(int i=0;i<str.size();i++){
                //printf("%s\n",str[i].c_str());
                
                string sub;
                int pos = 0;
                do{
                    pos = GetNextString(str[i], sub, pos);
                    //cout << sub << endl;
                    unsigned int loc = sub.find( ".png", 0 );
                    if( loc != string::npos ){
                        //printf("%s\n",sub.c_str());
                        Boolean samecheck=false;
                        for(int i=0;i<picname.size();i++){
                            if(picname[i]==sub){
                                samecheck=true;
                            }
                        }
                        if(samecheck==false){
                            picname.push_back(sub);
                            //printf("%s",sub.c_str());
                        }
                        
                    }
                }while(pos != string::npos);
            }
        }
        
        
        
        
    }else if(key=='v'){
        if(showimages==false){
            showimages=true;
        }else{
            showimages=false;
        }
    }else if(key=='g'){
        if(gray==false){
            gray=true;
        }else{
            gray=false;
        }
    }else if(key=='a'){
        showvideo ? showvideo=false : showvideo=true;
    }else if(key=='s'){
        showCvAnalysis ? showCvAnalysis=false : showCvAnalysis=true;
    }else if(key=='q'){
        videoscale+=0.01;
    }else if(key=='w'){
        videoscale-=0.01;
        
    }else if(key==OF_KEY_UP){
        videoY-=5;
    }else if(key==OF_KEY_DOWN){
        videoY+=5;
    }else if(key==OF_KEY_LEFT){
        videoX-=5;
    }else if(key==OF_KEY_RIGHT){
        videoX+=5;
        
    }else if(key=='p'){
        particle_tf ? particle_tf=false : particle_tf=true;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
