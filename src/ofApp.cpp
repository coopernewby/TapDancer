#include "ofApp.h"

string ofSystemCall(string command)
{
    cerr << "here" << endl;
    FILE* pipe = popen(command.c_str(), "r");
    cerr << "through the pipe" << endl;
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    result.erase(remove(result.begin(), result.end(), '\n'), result.end());
    return result;
}


//--------------------------------------------------------------
void ofApp::setup(){
    
    //Background color
    red = 40; blue = 40; green = 40;
    hideGUI = false;
    bdrawPadding = false;
    //loop off at init
    loop = false;
    newW=640;
    newH=480;
    
    //BTNdim = ofGetWidth() / 5.0f;
    
    SAMPLE_RATE = 44100;
    setBPM(123.0);//121
    pos = 0; // running sample count
    ofSoundStreamSetup(1,0,this); // sound stream initialised so samples can be counted
    
    //Define Loop length in beats (i.e. 16 = 4 bars long loop)
    loopLength = 48; //4 bar loops
    
    fftSmoothed = new float[8192];
    for (int i = 0; i < 8192; i++){
        fftSmoothed[i] = 0;
    }
    
    textInput = NULL;
    buffer = new float[256];
    for(int i = 0; i < 256; i++) { buffer[i] = ofNoise(i/100.0); }

    setGUI1();
    setGUI2();
    setGUI3();
    
    gui1->loadSettings("gui1Settings.xml");
    gui2->loadSettings("gui2Settings.xml");
    gui2->loadSettings("gui3Settings.xml");
    
    //Sound setup
    
    bang.loadSound("sounds/Bang Sandy Rivera.mp3");
    bang.setVolume(0.75f);
    bang.setMultiPlay(false);
    
//    skirt.loadSound("sounds/Bang Sandy Rivera.mp3");
//    skirt.setVolume(0.75f);
//    skirt.setMultiPlay(false);
    
    
    
    string youtube_dl = ofToDataPath("youtube-dl", true);
    // Open a Youtube video feed
    // http://code.google.com/apis/youtube/2.0/developers_guide_protocol_video_feeds.html
    // http://gdata.youtube.com/feeds/api/standardfeeds/most_popular?v=2&alt=json
    // http://gdata.youtube.com/feeds/api/videos?q=skateboarding+dog&alt=json
    
//    ofxJSONElement youtube;
//    youtube.open("http://gdata.youtube.com/feeds/api/videos?q=skateboarding+dog&alt=json");
//    // Loop through all of the feed->entry items in the feed
//    int numVideos = min(4, (int)youtube["feed"]["entry"].size());
//    for(int i=0; i<2; i++)
//    {
        // use ofToDataPath to get the complete path to the youtube-dl program
        // https://github.com/rg3/youtube-dl
        // In each one, there will be a "link" item that contains multiple "href" strings
        // We want the first href string inside the link item
        //string youtube_url = youtube["feed"]["entry"][i]["link"][UInt(0)]["href"].asString();
        //cout << youtube_url << endl;
        
        string youtube_url = "https://www.youtube.com/watch?v=dSAOV6XEjXA";
        
        // Assemble a command just like the one you would use on the command line
        // Format 18 = H264  (see http://en.wikipedia.org/wiki/YouTube#Quality_and_codecs)
        string command = youtube_dl+" --get-url --format 18 "+youtube_url;
        cout << command << endl;
        
        // Get the full (nasty) URL of the raw video
        //string vid_url = ofSystemCall(command);
        //cout << vid_url << endl;
        
        //string vid_url = "https://redirector.googlevideo.com/videoplayback?dur=180.813&itag=18&key=yt5&ip=2601%3A9%3A4980%3A920%3Acc6f%3A3115%3Ae9f2%3Ad20f&ipbits=0&source=youtube&sver=3&id=1d9253818cdff451&expire=1417612855&sparams=dur%2Cgcr%2Cid%2Cip%2Cipbits%2Citag%2Crequiressl%2Csource%2Cupn%2Cexpire&requiressl=yes&fexp=907259%2C916944%2C927622%2C932404%2C934601%2C936112%2C943909%2C943917%2C947209%2C948124%2C949415%2C952302%2C952605%2C952901%2C953912%2C957103%2C957105%2C957201&gcr=us&upn=Unek05bZMtE&signature=63F7BF076A7977F7BD6C81B889130E5777023F1A.14277638A474783AE16923300A7D2575E340776E&ratebypass=yes";
        //string vid_url = "https://r19---sn-o097zne7.googlevideo.com/videoplayback?source=youtube&ip=171.66.209.5&signature=D97C1A2F2296793336615F4A2D09A879A91487FF.F9E05E11F4A5A46E031B611BAA1D4BBDF027E529&sparams=dur%2Cgcr%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmm%2Cms%2Cmv%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire&id=o-AFiltfs435typ4jsFsjsvOR5DvgQP2X1omSH9lWeHkZe&ratebypass=yes&sver=3&mv=m&requiressl=yes&mm=31&expire=1417846532&gcr=us&ipbits=0&initcwndbps=9477500&upn=lFKFLQPSip4&key=yt5&dur=180.813&mt=1417824874&itag=18&fexp=907259%2C927622%2C932404%2C934947%2C935639%2C938692%2C943917%2C947209%2C948124%2C949427%2C951501%2C952302%2C952605%2C952901%2C953912%2C957103%2C957105%2C957201&ms=au";
    
    string vid_url = "https://r11---sn-nwj7kner.googlevideo.com/videoplayback?ms=au&fexp=900245%2C901493%2C907259%2C927622%2C932404%2C940939%2C943917%2C947209%2C948124%2C952302%2C952605%2C952901%2C953912%2C955301%2C957103%2C957105%2C957201&itag=18&mt=1418011603&dur=376.465&key=yt5&upn=OcGBBRh-4Dc&initcwndbps=1832500&ipbits=0&expire=1418033242&mm=31&mv=m&requiressl=yes&sver=3&ratebypass=yes&id=o-AM2Y88G4RJV9whGmJeEDdZ2aDju8W2umW60bFSYLMsKv&sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmm%2Cms%2Cmv%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire&source=youtube&signature=5AAF09AE2396B1E026E25C1AB8E4BD54E95A502C.071828CB6DFFBFD6768F2B5645FE6BEB959F3524&ip=67.161.9.40";
    
        //Load the video (from a url!) and start playing it
        vids.loadMovie(vid_url);
        vids.setVolume(.9);
        vids.setSpeed(1);
        vids.play();
    
    gui = new ofxUICanvas(newW/2, newH/2, 32, 32);
    gui->setDrawBack(true);
    gui->addMultiImageToggle("NUDGE BACK", "GUI/nudgeBk.png", false );
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);

    
}

//--------------------------------------------------------------
void ofApp::setBPM(float targetBPM){
    // NB. Currently the target BPM might not actually be achieved,
    // because permitted BPMs are limited to divisible by a whole number of samples.
    lengthOfOneBeatInSamples = (int)((SAMPLE_RATE*60.0f)/targetBPM);
    BPM=(SAMPLE_RATE*60.0f)/lengthOfOneBeatInSamples;
}

//--------------------------------------------------------------
void ofApp::audioRequested(float *output, int bufferSize, int numChannels) {
    bool startBeatDetected=false;
    int i;
    for(i = 0; i < bufferSize; i++) {
        pos++; // this gets incremented with every sample
        // when lengthOfOneBeatInSamples goes into
        // pos a whole number of times, we've entered a new quarter beat
        if(fmod(pos,lengthOfOneBeatInSamples)==0) {
            startBeatDetected=true;
        }
    }
    // was a new beat region entered during the last frame?
    if(startBeatDetected){
        //Cues are quantized to quarter notes then reset to zero after they have been triggered
        if(cuebang==loopLength){
            bang.play();
            cuebang = 0;
        }
        if (loop == true){
            if (bang.getIsPlaying()){
                cuebang++;
            }

        }
//        if(cueskirt==loopLength){
//            skirt.play();
//            cueskirt= 0;
//        }
//        if (loop == true){
//            if (skirt.getIsPlaying()){
//                cueskirt++;
//            }
//            
//        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSoundUpdate();
    bpmTapper.update();
    vids.update();
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(red, green, blue, 255);
   
    float widthDiv = ofGetWidth() / 3.0f;
    
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(255,255,255);
    ofDrawBitmapString("BPM: "+ofToString(BPM), 100, 110);
    //vids.draw(0,0,640,480);
    vids.draw((newW/2-newW/1.5/2), 0, newW/1.5, newH/1.5);
    
    //ofDrawBitmapString("Pos: "+ofToString(pos), 100, 130);
    //ofDrawBitmapString("Samples per beat: "+ofToString(lengthOfOneBeatInSamples), 100, 150);
    ofSetColor(0);
    ofDrawBitmapString("Hit Spacebar to tap BPM", 50, 550);
    ofDrawBitmapString("BPM: " + ofToString(bpmTapper.bpm()), 50, 500);
    bpmTapper.draw(50, 600, 10);


    


}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.getName();
    int kind = e.getKind();
    cout << "got event from: " << name << endl;
    
    if(name == "SAMPLER")
    {
        ofxUIImageSampler *is = (ofxUIImageSampler *) e.widget;
        ofColor clr = is->getColor();
        red = clr.r;
        blue = clr.b;
        green = clr.g;
    }
    //Stops all songs and audio streams if things get crazy
    else if(name == "Stop")
    {
        ofxUIButton *button = (ofxUIButton *) e.getButton();
        ofSoundStopAll();
    }
    //Toggles the loop mode on
    else if(name == "Loop")
    {
       ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
        loop = (toggle->getValue());
        
    }
    //If the image toggles are initiated, the cue_________ sends a variable trigger to the audio requested funtion to trigger samples on the down beat. RIght now all samples are quarternote quantized
    else if(name == "REWIND")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        
    }
    else if(name == "NUDGE BACK")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        
    }
    else if(name == "NUDGE FORWARD")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        
    }
    else if(name == "PLAY")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        cuebang=loopLength;
    }
    else if(name == "FFWD")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        //cueskirt=loopLength;
        
    }
    else if(name == "SEARCH")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        
    }
    else if(name == "VIDPANEL")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        
    }
    else if(name == "MUTE")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        
    }
    else if(name == "VOLUME")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        
    }
    else if(name == "PAUSE")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        
    }
    else if(name == "MATRIX2")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        
    }
    
    else if(name == "RADIO VERTICAL")
    {
        ofxUIRadio *radio = (ofxUIRadio *) e.widget;
        cout << radio->getName() << " value: " << radio->getValue() << " active name: " << radio->getActiveName() << endl;
    }

}

//--------------------------------------------------------------
void ofApp::exit()
{
    gui1->saveSettings("gui1Settings.xml");
    gui2->saveSettings("gui2Settings.xml");
    

    delete gui1;
    delete gui2;
    delete[] buffer;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(gui2->hasKeyboardFocus())
    {
        return;
    }
    switch (key)
    {
        case 't':
        {
            if(textInput != NULL)
            {
                textInput->setTextString(ofGetTimestampString());
            }
        }
            break;
            
        case 'T':
        {
            if(tm != NULL)
            {
                int cols = tm->getColumnCount();
                int rows = tm->getRowCount();
                for(int row = 0; row < rows; row++)
                {
                    for(int col = 0; col < cols; col++)
                    {
                        cout << tm->getState(row, col) << "\t";
                    }
                    cout << endl;
                }
            }
        }
            break;
            
        case 'd':
        {
            if(ddl != NULL)
            {
                vector<ofxUIWidget *> selected = ddl->getSelected();
                for(vector<ofxUIWidget *>::iterator it = selected.begin(); it != selected.end(); ++it)
                {
                    ofxUILabelToggle *lt = (ofxUILabelToggle *) (*it);
                    cout << lt->getName() << endl;
                }
            }
        }
            break;
            
        case 'D':
        {
            if(ddl != NULL)
            {
                vector<string> names = ddl->getSelectedNames();
                for(vector<string>::iterator it = names.begin(); it != names.end(); ++it)
                {
                    cout << (*it) << endl;
                }
            }
        }
            break;
            
        case 'f':
            ofToggleFullscreen();
            break;
            
        case 'F':
        {
            if(tm != NULL)
            {
                tm->setDrawOutlineHighLight(!tm->getDrawOutlineHighLight());
                //                tm->setDrawPaddingOutline(!tm->getDrawPaddingOutline());
            }
        }
            break;
            
        case 'h':
            gui1->toggleVisible();
            gui2->toggleVisible();
            gui3->toggleVisible();
            break;
            
        case 'p':
            bdrawPadding = !bdrawPadding;
            gui1->setDrawWidgetPaddingOutline(bdrawPadding);
            gui2->setDrawWidgetPaddingOutline(bdrawPadding);
            break;
            
        case '[':
            gui1->setDrawWidgetPadding(false);
            gui2->setDrawWidgetPadding(false);
            break;
            
        case ']':
            gui1->setDrawWidgetPadding(true);
            gui2->setDrawWidgetPadding(true);
            break;
            
        case '1':
            gui1->toggleVisible();
            break;
            
        case '2':
            gui2->toggleVisible();
            break;
        case ' ':
        {
            bpmTapper.tap();
            cout << vidSpeed << endl;
            break;
        }
        case 's':
        {
            vids.setVolume(0);
            vidSpeed =  .01*(100*BPM/bpmTapper.bpm());
            vids.setSpeed(vidSpeed);
            gui1->setVisible(false);
            break;
        }
        //Nudge video backwards
        case ',':
        {
            vids.previousFrame();
            vids.play();
            ofAddListener(gui2->newGUIEvent,this,&ofApp::guiEvent);
            
            break;
        }
        //Nudge video forward
        case '.':
        {
            vids.nextFrame();
            vids.play();
            break;
        }
            
        //double speed
        case 'l':
        {
            
            vidSpeed =  .01*(100*BPM/(bpmTapper.bpm()/2));
            vids.setSpeed(vidSpeed);
            break;
        }
        case 'k':
        {
            vidSpeed =  .01*(100*BPM/(bpmTapper.bpm()*2));
            vids.setSpeed(vidSpeed);
            break;
        }

        //reverse clip
        case 'r':
        {
            vidSpeed =-vidSpeed;
            vids.setSpeed(vidSpeed);
            break;
        }
        default:
            break;
    }
}


void ofApp::setGUI1()
{
    gui1 = new ofxUISuperCanvas("");
    gui1->setDrawBack(false);
    gui1->addSpacer();
    gui1->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    gui1->addMultiImageToggle("NUDGE BACK", "GUI/tap.png", false , 389/2,135/2);
    
    gui1->setGlobalButtonDimension(OFX_UI_GLOBAL_BUTTON_DIMENSION);
    
    gui1->setPosition(212*2, 0);
    gui1->autoSizeToFitWidgets();
    
    gui1->addSpacer();
    
    
    ofAddListener(gui1->newGUIEvent,this,&ofApp::guiEvent);
    
    
}

void ofApp::setGUI2()
{
    gui2 = new ofxUISuperCanvas("");
    
    textInput = gui2->addTextInput("TEXT INPUT", "Insert YouTube URL here");
    BTNdim =  70;
    gui2->setGlobalButtonDimension(BTNdim);
    gui2->addMultiImageToggle("REWIND", "GUI/Rwd.png", false);
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui2->addMultiImageToggle("NUDGE BACK", "GUI/nudgeBk.png", false);
    gui2->addMultiImageToggle("NUDGE FORWARD", "GUI/nudgeFwd.png", false);
    gui2->addMultiImageToggle("PLAY", "GUI/Play.png", false);
    gui2->addMultiImageToggle("FFWD", "GUI/FFwd.png", false);
    gui2->addMultiImageToggle("PAUSE", "GUI/pause.png", false);
    gui2->addMultiImageToggle("VOLUME", "GUI/Volume.png", false);
    gui2->addMultiImageToggle("MUTE", "GUI/Mute.png", false);
    gui2->addMultiImageToggle("VIDPANEL", "GUI/vidPanel.png", false);
    gui2->addMultiImageToggle("SEARCH", "GUI/Search.png", false);
    
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui2->setGlobalButtonDimension(32);
    gui2->addButton("Stop", false)->setLabelVisible(true);
    gui2->addToggle("Loop", false)->setLabelVisible(true);
    
    gui2->setPosition(212*3,0);
    gui2->autoSizeToFitWidgets();
    
    ofAddListener(gui2->newGUIEvent,this,&ofApp::guiEvent);
}

void ofApp::setGUI3()
{
    gui3 = new ofxUISuperCanvas("");
    gui3->setDrawBack(false);
    gui3->addSpacer();
    gui3->setGlobalButtonDimension(24);
    gui3->addLabel("Video Loop Matrix", OFX_UI_FONT_MEDIUM);
    tm = gui3->addToggleMatrix("MATRIX2", 3, 6);
    gui3->addToggleMatrix("MATRIX3", 1, 4);
    
    gui3->addSpacer();
    vector<string> items;
    items.push_back("Bang - EDX"); items.push_back("EDM Trend Machine - Knife Party"); items.push_back("THIRD SONG");
    items.push_back("FOURTH ITEM"); items.push_back("FIFTH ITEM"); items.push_back("SIXTH ITEM");
    
    vector<string> names;
    names.push_back("RAD1");
    names.push_back("RAD2");
    names.push_back("RAD3");
    
    gui3->addSpacer();
    gui3->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    ddl = gui3->addDropDownList("DROP DOWN LIST", items);
    ddl->setAllowMultiple(true);
    
    gui3->setGlobalButtonDimension(OFX_UI_GLOBAL_BUTTON_DIMENSION);
    
    gui3->setPosition(212*2, 0);
    gui3->autoSizeToFitWidgets();
    
    gui3->addSpacer();
    gui3->addRadio("RADIO HORIZONTAL", names, OFX_UI_ORIENTATION_HORIZONTAL);
    gui3->addRadio("RADIO VERTICAL", names, OFX_UI_ORIENTATION_VERTICAL);
    
    
    ofAddListener(gui3->newGUIEvent,this,&ofApp::guiEvent);

    
    
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
    newH = h;
    newW = w;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}