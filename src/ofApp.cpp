#include "ofApp.h"

//        string vid_url0 = getYoutubeDlStream("https://www.youtube.com/watch?v=yxGIHwpbKFU");
//        string vid_url1 = getYoutubeDlStream("https://www.youtube.com/watch?v=qXbP4JBf8To");
//        string vid_url2 = getYoutubeDlStream("https://www.youtube.com/watch?v=rw_M-ai1I0k");
//        string vid_url3 = getYoutubeDlStream("https://www.youtube.com/watch?v=5P-TXxoWTSE");
//B&W
//string url0 =("https://www.youtube.com/watch?v=ahoJReiCaPk");
//string url1 =("https://www.youtube.com/watch?v=OeE1amkKuBU");
//string url2 =("https://www.youtube.com/watch?v=WwKFALb6Vw8");
//string url3 =("https://www.youtube.com/watch?v=8S3Yt-NxY0E");

//1920s ryan gosling James brown


string url0 =("https://www.youtube.com/watch?v=jTR6xBeC2xA");
//string url1 =("https://www.youtube.com/watch?v=i_ENjos_PpE");
string url2 =("https://www.youtube.com/watch?v=NGFRG09sHag");
string url3 =("https://www.youtube.com/watch?v=Zdz88MBWomo");


////90's 80's
//    string url0 = "https://www.youtube.com/watch?v=fA_r88JVAKE";
//    string url1 = "https://www.youtube.com/watch?v=otCpCn0l4Wo";
//    string url2 = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";
//    string url3 = "https://www.youtube.com/watch?v=WW659rsCYGQ";

//Funny
//    string url0 = "https://www.youtube.com/watch?v=3eRBFkxgG7g";
//    string url1 = "https://www.youtube.com/watch?v=IxPbgnO81sQ";
//    string url2 = "https://www.youtube.com/watch?v=jQM32a6JqJo";
//    string url3 = "https://www.youtube.com/watch?v=YPhXZQFBeEE";


//    string url0 = "https://www.youtube.com/watch?v=aD1euxkzE9I";
//    string url1 = "https://www.youtube.com/watch?v=v2Zbs56AKYs";
//    string url2 = "https://www.youtube.com/watch?v=gx-NLPH8JeM";
//    string url3 = "https://www.youtube.com/watch?v=E8pja8RGvYc";

//Rave
//string url0 = "https://www.youtube.com/watch?v=JqYhuwu614Y";
//string url1 = "https://www.youtube.com/watch?v=hWJFfnHNOWI";
//string url2 = "https://www.youtube.com/watch?v=Wq00WtzbcDU";
//string url3 = "https://www.youtube.com/watch?v=5L1tr0PIx20";

//Twist + ELvis
//string url0 = "https://www.youtube.com/watch?v=im9XuJJXylw";
//string url1 = "https://www.youtube.com/watch?v=MggQSspSGU8";
//string url2 = "https://www.youtube.com/watch?v=qka6JrKUM5U";
//string url3 = "https://www.youtube.com/watch?v=uEsXC1X9u50";

//JitterBug
//string url0 = "https://www.youtube.com/watch?v=SXHxxj1oBeI";
//string url1 = "https://www.youtube.com/watch?v=c56gRLmDwUs";
//string url2 = "https://www.youtube.com/watch?v=2tQnMwmhdrY";
//string url3 = "https://www.youtube.com/watch?v=CowekQq6kY8";

////Tapping
//string url0 = "https://www.youtube.com/watch?v=uJzYKm1_Bvo";
//string url1 = "https://www.youtube.com/watch?v=mxPgplMujzQ";
//string url2 = "https://www.youtube.com/watch?v=p3YWWfnWBJM";
//string url3 = "https://www.youtube.com/watch?v=1caGN5No89w";
//https://www.youtube.com/watch?v=3i53_qIC31c
//https://www.youtube.com/watch?v=Dq_sGy4H_-k


string getYoutubeDlStream(string _link) //_link is the complete url, just like above.
{
    string command =  "/usr/local/bin/youtube-dl --get-url --format 18 " + _link; //youtube_dl is my local path to the youtube-dl executable.
    
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    result.erase(remove(result.begin(), result.end(), '\n'), result.end());
    string dlStream = result;
    
    ofLogNotice(command + "---> " + dlStream); // Check result
    
    bool success = (ofIsStringInString(dlStream, "http://") || ofIsStringInString(dlStream, "https://"));
    
    return ( success? dlStream : ""); // return valid URL when found, else return empty string.
}


//--------------------------------------------------------------
void ofApp::setup(){
    
    hideGUI = false;
    bdrawPadding = false;
    //loop off at init
    loop = false;
    newW=1024;
    newH=768;
    img = new ofImage();
    
    //BTNdim = ofGetWidth() / 5.0f;
    
    SAMPLE_RATE = 44100;
    setBPM(122.0);//123
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
    
    //gui1->loadSettings("gui1Settings.xml");
    //gui2->loadSettings("gui2Settings.xml");
    
    //Sound setup
    
    bang.loadSound("sounds/All I See (TCTS Remix).mp3");
    bang.setVolume(0.75f);
    bang.setMultiPlay(false);
    
    ofxXmlSettings *XML = new ofxXmlSettings();
    XML->loadFile("sounds/videoMeta.xml");
    int videoTags = XML->getNumTags("Video");
    for(int i = 0; i < videoTags; ++i) {
        XML->pushTag("Video", i);
        string url1 = XML->getValue("YouTube", "");
        cout<< url1<<endl;
        vidbpm = XML->getValue("BPM", 0.0);
        bpmTapper[i].setBpm(vidbpm);
        cout<< vidbpm<<endl;
    }
    XML->popTag();
    delete XML;
    
    // Open a Youtube video feed
    // http://code.google.com/apis/youtube/2.0/developers_guide_protocol_video_feeds.html
    // http://gdata.youtube.com/feeds/api/standardfeeds/most_popular?v=2&alt=json
    // http://gdata.youtube.com/feeds/api/videos?q=skateboarding+dog&alt=json
    
//    ofxJSONElement youtube;
//    youtube.open("http://gdata.youtube.com/feeds/api/videos?q=skateboarding+dog&alt=json");
//    // Loop through all of the feed->entry items in the feed
//    int numVideos = min(4, (int)youtube["feed"]["entry"].size());
        // use ofToDataPath to get the complete path to the youtube-dl program
        // https://github.com/rg3/youtube-dl
        // In each one, there will be a "link" item that contains multiple "href" strings
        // We want the first href string inside the link item
        //string youtube_url = youtube["feed"]["entry"][i]["link"][UInt(0)]["href"].asString();
        //cout << youtube_url << endl;
//for(int i=0; i<4; i++){
//    .asString()
    
    
    string vid_url0 = getYoutubeDlStream(url0);
    //string vid_url1 = getYoutubeDlStream(url1);
    string vid_url2 = getYoutubeDlStream(url2);
    string vid_url3 = getYoutubeDlStream(url3);
    //https://www.youtube.com/watch?v=okvRdTYWkDI
    //https://www.youtube.com/watch?v=7RiJtODykRQ
  //https://www.youtube.com/watch?v=1E9IU1fO89U
    //https://www.youtube.com/watch?v=WW659rsCYGQ
//Disco
//    string vid_url0 = getYoutubeDlStream("https://www.youtube.com/watch?v=9EcjWd-O4jI");
//    string vid_url1 = getYoutubeDlStream("https://www.youtube.com/watch?v=qkc8YduPnOM");
//    string vid_url2 = getYoutubeDlStream("https://www.youtube.com/watch?v=glM0XaGhYl8");
//    string vid_url3 = getYoutubeDlStream("https://www.youtube.com/watch?v=FX5HcB4cCzU");
    
    
        //Load the video (from a url!)
        vids[0].loadMovie(vid_url0);
        vids[1].loadMovie(vid_url2);
        vids[2].loadMovie(vid_url2);
        vids[3].loadMovie(vid_url3);
    
    for(int i=0; i<4; i++){
        vids[i].setVolume(.9);
        vids[i].setSpeed(1);
    }
    
//    vids[currentVid].setVolume(0);
//    vidSpeed =  .01*(100*BPM/bpmTapper[currentVid].bpm());
//    vids[currentVid].setSpeed(vidSpeed);
//    gui1->setVisible(false);
    
    
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
    for(int i = 0; i < bufferSize; i++) {
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
    
    //vids[1].update();
    for(int i=0; i<4; i++)
    {
        bpmTapper[i].update();
        vids[i].update();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(50,50,50, 255);
    
    float widthDiv = ofGetWidth() / 3.0f;
    
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    if (currentVid == 0){
        if(vidFull == true){
            vids[currentVid].draw(0, 0, newW, newH);
        }
        else vids[currentVid].draw((newW/2-newW/1.3/2), 20, newW/1.3, newH/1.3);
    }
    if (currentVid == 1){
        if(vidFull == true){
            vids[currentVid].draw(0, 0, newW, newH);
        }
        else vids[currentVid].draw((newW/2-newW/1.3/2), 20, newW/1.3, newH/1.3);
    }
    if (currentVid == 2){
        if(vidFull == true){
            vids[currentVid].draw(0, 0, newW, newH);
        }
        else vids[currentVid].draw((newW/2-newW/1.3/2), 20, newW/1.3, newH/1.3);
    }
    if (currentVid == 3){
        if(vidFull == true){
            vids[currentVid].draw(0, 0, newW, newH);
        }
        else vids[currentVid].draw((newW/2-newW/1.3/2), 20, newW/1.3, newH/1.3);
    }
    if (vidPanels == true){
    vids[0].draw(newW/2, 0, newW/2, newH/2);
    vids[1].draw( 0 , newH/2, newW/2, newH/2);
    vids[2].draw( 0 , 0 , newW/2, newH/2);
    vids[3].draw(newW/2,newH/2, newW/2, newH/2);
    }

    //ofSetColor(0);
    ofDrawBitmapString("BPM: " + ofToString(bpmTapper[currentVid].bpm()), 50, 560);


    
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
    string name = e.getName();
    int kind = e.getKind();
    cout << "got event from: " << name << endl;
    
    //Stops all songs and audio streams if things get crazy
    if(name == "Stop")
    {
        ofxUIButton *button = (ofxUIButton *) e.getButton();
        ofSoundStopAll();
    }
    else if(name == "resetBPM")
    {
        bpmTapper[currentVid].startFresh();
    }
    //Toggles the loop mode on
    else if(name == "Loop")
    {
       ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
        loop = (toggle->getValue());
        
    }
    //If the image toggles are initiated, the cue_________ sends a variable trigger to the audio requested funtion to trigger samples on the down beat. RIght now all samples are quarternote quantized
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
        if(bang.getIsPlaying() == true){
            ofSoundStopAll();
        }
        else cuebang=loopLength;
    }
    else if(name == "VIDPANEL")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        vidPanels = !vidPanels;
        
    }
    else if(name == "MUTE")
    {
        ofxUIImageToggle *toggle = (ofxUIImageToggle *) e.getToggle();
        
    }
    else if(name == "EDM Trend Machine - Knife Party")
    {
        bang.loadSound("sounds/EDM Trend Machine.mp3");
        setBPM(124.0);
        bang.setVolume(0.75f);
        bang.setMultiPlay(false);
    }
    else if(name == "Bang - EDX")
    {
        bang.loadSound("sounds/Bang Sandy Rivera.mp3");
        setBPM(123.0);
        bang.setVolume(0.75f);
        bang.setMultiPlay(false);
    }
    else if(name == "Drop That Skirt - Croatia Squad")
    {
        bang.loadSound("sounds/Drop_That_Skirt.mp3");
        setBPM(121.0);
        bang.setVolume(0.75f);
        bang.setMultiPlay(false);
    }
    else if(name == "This Could Be Love - Borgeous")
    {
        bang.loadSound("sounds/borgeous-shaun-frank-this-could-be-love-feat-delaney-jane.mp3");
        setBPM(126.0);
        bang.setVolume(0.75f);
        bang.setMultiPlay(false);
    }
    else if(name == "Milking - Croatia Squad")
    {
        bang.loadSound("sounds/Milking.mp3");
        setBPM(122.0);
        bang.setVolume(0.75f);
        bang.setMultiPlay(false);
    }
    else if(name == "True - Nora En Pure")
    {
        bang.loadSound("sounds/True (Mark Lower Remix).mp3");
        setBPM(122.0);
        bang.setVolume(0.75f);
        bang.setMultiPlay(false);
    }
    else if(name == "Sleepless - Cazzette")
    {
        bang.loadSound("sounds/cazzette-sleepless-hotel-garuda-thero-remix.mp3");
        setBPM(120.0);
        bang.setVolume(0.75f);
        bang.setMultiPlay(false);
    }
    else if(name == "All I See - Bondax")
    {
        bang.loadSound("sounds/All I See (TCTS Remix).mp3");
        setBPM(123.0);
        bang.setVolume(0.75f);
        bang.setMultiPlay(false);
    }
    else if(name == "Okay - Shiba San")
    {
        bang.loadSound("sounds/Okay.mp3");
        setBPM(123.0);
        bang.setVolume(0.75f);
        bang.setMultiPlay(false);
    }
    else if(name == "Till I Heard You Talking")
    {
        bang.loadSound("sounds/Till I Heard You Talking.m4a");
        setBPM(123.0);
        bang.setVolume(0.75f);
        bang.setMultiPlay(false);
    }
    else if(name == "Take Me Over")
    {
        bang.loadSound("sounds/Take Me Over.m4a");
        setBPM(123.0);
        bang.setVolume(0.75f);
        bang.setMultiPlay(false);
    }
    else if(name == "Living Without You")
    {
        bang.loadSound("sounds/07 Living Without You (MK Letting Go Remix).m4a");
        setBPM(123.0);
        bang.setVolume(0.75f);
        bang.setMultiPlay(false);
    }
    
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
            vidFull=!vidFull;
            gui2->toggleVisible();
            break;
            
        case 'P':
            bdrawPadding = !bdrawPadding;
            gui1->setDrawWidgetPaddingOutline(bdrawPadding);
            gui2->setDrawWidgetPaddingOutline(bdrawPadding);
            break;
        
        case OF_KEY_LEFT:
            if(currentVid>0){
                currentVid--;
            }
            else currentVid = 3;
            break;
        
        case OF_KEY_RIGHT:
            if(currentVid<3){
            currentVid++;
            }
            else currentVid = 0;
            break;
            
        case 'p':
            //video play
            vids[currentVid].play();
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
            vids[currentVid].getCurrentFrame();
            break;
            
        case '2':
            vids[currentVid].getCurrentFrame();
            break;
            
        case '3':
            vids[currentVid].getCurrentFrame();
            break;
            
        case '4':
            vids[currentVid].getCurrentFrame();
            break;
        case ' ':
        {
            bpmTapper[currentVid].tap();
            cout << vidSpeed << endl;
            break;
        }
        case 's':
        {
            vids[currentVid].setVolume(0);
            vidSpeed =  .01*(100*BPM/bpmTapper[currentVid].bpm());
            vids[currentVid].setSpeed(vidSpeed);
            gui1->setVisible(false);
            break;
        }
        //Nudge video backwards
        case ',':
        {
            vids[currentVid].previousFrame();
            vids[currentVid].play();
            ofAddListener(gui2->newGUIEvent,this,&ofApp::guiEvent);
            
            break;
        }
        //Nudge video forward
        case '.':
        {
            vids[currentVid].nextFrame();
            vids[currentVid].play();
            break;
        }
            
        //double speed
//        case 'l':
//        {
//            
//            vidSpeed =  .01*(100*BPM/(bpmTapper[currentVid].bpm()/2));
//            vids[currentVid].setSpeed(vidSpeed);
//            break;
//        }
//        case 'k':
//        {
//            vidSpeed =  .01*(100*BPM/(bpmTapper[currentVid].bpm()*2));
//            vids[currentVid].setSpeed(vidSpeed);
//            break;
//        }

        //reverse clip
        case 'r':
        {
            vids[currentVid].setSpeed(-vidSpeed);
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
    BTNdim =  60;
    gui2->setGlobalButtonDimension(BTNdim);
        vector<string> items;
        items.push_back("Bang - EDX"); items.push_back("EDM Trend Machine - Knife Party"); items.push_back("Okay - Shiba San");
    items.push_back("All I See - Bondax"); items.push_back("This Could Be Love - Borgeous"); items.push_back("Milking - Croatia Squad"); items.push_back("Drop That Skirt - Croatia Squad");items.push_back("True - Nora En Pure");items.push_back("Sleepless - Cazzette");items.push_back("Till I Heard You Talking");items.push_back("Take Me Over");items.push_back("Living Without You");
    
    gui2->addMultiImageToggle("NUDGE BACK", "GUI/nudgeBk.png", false);
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui2->addMultiImageToggle("NUDGE FORWARD", "GUI/nudgeFwd.png", false);
    gui2->addMultiImageToggle("PLAY", "GUI/Play.png", false);
    gui2->addMultiImageToggle("PAUSE", "GUI/pause.png", false);
    gui2->addMultiImageToggle("MUTE", "GUI/Mute.png", false);
    gui2->addMultiImageToggle("VIDPANEL", "GUI/vidPanel.png", false);
    
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui2->setGlobalButtonDimension(32);
    gui2->addButton("Stop", false)->setLabelVisible(true);
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui2->addButton("resetBPM", false)->setLabelVisible(true);
    gui2->addToggle("Loop", false)->setLabelVisible(true);
    gui2->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    ddl = gui2->addDropDownList("Select Song", items);
    ddl->setAllowMultiple(false);

    
    gui2->setPosition(212*5,0);
    gui2->autoSizeToFitWidgets();
    
    ofAddListener(gui2->newGUIEvent,this,&ofApp::guiEvent);
}

//void ofApp::setGUI3()
//{
//    gui3 = new ofxUISuperCanvas("");
//    gui3->setDrawBack(false);
//    gui3->setGlobalButtonDimension(24);
//    vector<string> items;
//    items.push_back("Bang - EDX"); items.push_back("EDM Trend Machine - Knife Party"); items.push_back("Okay - Shiba San");
//    items.push_back("All I See - Bondax"); items.push_back("This Could Be Love - Borgeous"); items.push_back("Milking - Croatia Squad"); items.push_back("Drop That Skirt - Croatia Squad");items.push_back("True - Nora En Pure");items.push_back("Sleepless - Cazzette");
//    
//   
//    gui3->addLabel("Filter Color", OFX_UI_FONT_MEDIUM);
//    gui3->addImageSampler("SAMPLER", img);
//    gui3->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
//    ddl = gui3->addDropDownList("Select Song", items);
//    ddl->setAllowMultiple(false);
//    gui3->setGlobalButtonDimension(OFX_UI_GLOBAL_BUTTON_DIMENSION);
//    
//    gui3->setPosition(212*2, 0);
//    gui3->autoSizeToFitWidgets();
//    
//    
//    ofAddListener(gui3->newGUIEvent,this,&ofApp::guiEvent);
//
//    
//    
//}


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

//--------------------------------------------------------------
void ofApp::exit()
{
    gui1->saveSettings("gui1Settings.xml");
    gui2->saveSettings("gui2Settings.xml");
    
    //Save BPM information
    ofxXmlSettings *XML = new ofxXmlSettings();
    //int len = widgetsWithState.size();
    for(int i = 0; i < 4; ++i) {
        int index = XML->addTag("Video");
        if(XML->pushTag("Video", index)) {
            XML->setValue("YouTube", url0, 0);
            XML->setValue("BPM", bpmTapper[i].bpm(), 0);
        }
        XML->popTag();
    }
    XML->saveFile("sounds/videoMeta.xml");
    delete XML;
    
    delete gui1;
    delete gui2;
    delete[] buffer;
}