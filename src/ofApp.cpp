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
    newW=1024;
    newH=768;
    
    //BTNdim = ofGetWidth() / 5.0f;
    
    SAMPLE_RATE = 44100;
    setBPM(120.0);//123
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
    
    bang.loadSound("sounds/cazzette-sleepless-hotel-garuda-thero-remix.mp3");
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
        //cout << command << endl;
        
        // Get the full (nasty) URL of the raw video
        //string vid_url = ofSystemCall(command);
        //cout << vid_url << endl;
        
        string vid_url0 = "https://r11---sn-o097znel.googlevideo.com/videoplayback?ip=67.161.9.40&ms=au&mv=m&initcwndbps=2253750&ipbits=0&dur=376.465&id=o-AAdrZrA9Si8vlGiBvNAQBWan7k85S1mHKfZjgPf-vQPW&key=yt5&signature=1C45B030AC339B4ADBC81BC153FCC0F050C5FA4E.9CA6BA028DA1E4D6559DAB7DD42C4E1DD27FD6E6&mm=31&mt=1418206605&sver=3&sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmm%2Cms%2Cmv%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire&upn=u1vxV68103g&requiressl=yes&source=youtube&itag=18&expire=1418228256&fexp=904849%2C907259%2C927622%2C931352%2C932404%2C936118%2C941004%2C943917%2C947209%2C948124%2C952302%2C952605%2C952901%2C953912%2C957103%2C957105%2C957201&ratebypass=yes";
        
        string vid_url1 = "https://r19---sn-nwj7kner.googlevideo.com/videoplayback?sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmm%2Cms%2Cmv%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire&key=yt5&fexp=907259%2C913430%2C916635%2C927622%2C932404%2C936118%2C943917%2C947209%2C948124%2C952005%2C952302%2C952605%2C952901%2C953912%2C955301%2C957103%2C957105%2C957201&expire=1418227564&upn=V8NxzIv25yA&ipbits=0&dur=161.935&signature=B33F37873AF08266214047E5352C7A36ECB7AE2B.247500948A7F5C128239F51996D9732425F83954&itag=18&source=youtube&ms=au&ip=67.161.9.40&mv=m&ratebypass=yes&mt=1418205885&requiressl=yes&id=o-ADdKpwdChRNRk9_pn-u8x91UfO2_FUIzxGCImJt8qM4c&sver=3&mm=31&initcwndbps=2228750";
    
        string vid_url2 = "https://r19---sn-o097znel.googlevideo.com/videoplayback?requiressl=yes&ip=67.161.9.40&ms=au&mv=m&sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmm%2Cms%2Cmv%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire&mt=1418205956&signature=A87A8491111A37D282558B6BC908A2BE0E7BCA8A.0939C7143352411F37E57447A7ED54817D0F9D6B&id=o-AO1oZnSYqid5u94DYnciYWom8x2fE7qfQAYjHDg4hfJV&mm=31&itag=18&initcwndbps=2217500&fexp=901496%2C907259%2C927622%2C929305%2C932404%2C935693%2C941004%2C943917%2C947209%2C947601%2C948124%2C952302%2C952605%2C952901%2C953912%2C957103%2C957105%2C957201&source=youtube&upn=O9voNnNjXJI&sver=3&dur=161.935&ipbits=0&key=yt5&expire=1418227614&ratebypass=yes";
        
        string vid_url3 = "https://r1---sn-o097znel.googlevideo.com/videoplayback?mv=m&pcm2=no&mt=1418206033&ms=au&source=youtube&expire=1418227691&sver=3&dur=85.124&key=yt5&requiressl=yes&mm=31&ratebypass=yes&itag=18&initcwndbps=2222500&upn=qOz1uPONQLc&id=o-ALW-E8Coxeut4lnWN71SyI0hgY08s82bMuceGImAt2jL&ip=67.161.9.40&fexp=905639%2C907259%2C917000%2C927622%2C932404%2C938644%2C941004%2C943915%2C943917%2C947209%2C948124%2C948207%2C952302%2C952605%2C952901%2C953912%2C955301%2C957103%2C957105%2C957201&signature=49F21DD21B3C5012CA9D8BE456740033C38928B4.90118FF562461EFF9CC9011C27F1B8AE158FE930&sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmm%2Cms%2Cmv%2Cpcm2%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire&ipbits=0";
    
        //Load the video (from a url!)
        vids[0].loadMovie(vid_url0);
        vids[1].loadMovie(vid_url1);
        vids[2].loadMovie(vid_url2);
        vids[3].loadMovie(vid_url3);
    
    for(int i=0; i<4; i++){
        vids[i].setVolume(.9);
        vids[i].setSpeed(1);
    }
    
//    gui = new ofxUICanvas(newW/2, newH/2, 32, 32);
//    gui->setDrawBack(true);
//    gui->addMultiImageToggle("NUDGE BACK", "GUI/nudgeBk.png", false );
//    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);

    
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
    bpmTapper.update();
    //vids[1].update();
    for(int i=0; i<4; i++)
    {
        vids[i].update();
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(red, green, blue, 255);
    
    float widthDiv = ofGetWidth() / 3.0f;
    
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(255,255,255);
    ofDrawBitmapString("BPM: "+ofToString(BPM), 100, 110);
    
    if (currentVid == 0){
    vids[currentVid].draw((newW/2-newW/1.3/2), 20, newW/1.3, newH/1.3);
    }
    if (currentVid == 1){
        vids[currentVid].draw((newW/2-newW/1.3/2), 20, newW/1.3, newH/1.3);
    }
    if (currentVid == 2){
        vids[currentVid].draw((newW/2-newW/1.3/2), 20, newW/1.3, newH/1.3);
    }
    if (currentVid == 3){
        vids[currentVid].draw((newW/2-newW/1.3/2), 20, newW/1.3, newH/1.3);
    }
    if (vidPanels == true){
    vids[0].draw(newW/2, 0, newW/2, newH/2);
    vids[1].draw( 0 , newH/2, newW/2, newH/2);
    vids[2].draw( 0 , 0 , newW/2, newH/2);
    vids[3].draw(newW/2,newH/2, newW/2, newH/2);
    }

    
    //ofDrawBitmapString("Pos: "+ofToString(pos), 100, 130);
    //ofDrawBitmapString("Samples per beat: "+ofToString(lengthOfOneBeatInSamples), 100, 150);
    ofSetColor(0);
    ofDrawBitmapString("Hit Spacebar to tap BPM", 50, 550);
    ofDrawBitmapString("BPM: " + ofToString(bpmTapper.bpm()), 50, 500);


    
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
    else if(name == "resetBPM")
    {
        bpmTapper.startFresh();
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
        vidPanels = !vidPanels;
        
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
    gui3->saveSettings("gui2Settings.xml");
    

    delete gui1;
    delete gui2;
    delete gui3;
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
            if(currentVid<4){
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
            gui1->toggleVisible();
            vids[currentVid].getCurrentFrame();
            break;
            
        case '2':
            gui2->toggleVisible();
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
        case 'l':
        {
            
            vidSpeed =  .01*(100*BPM/(bpmTapper.bpm()/2));
            vids[currentVid].setSpeed(vidSpeed);
            break;
        }
        case 'k':
        {
            vidSpeed =  .01*(100*BPM/(bpmTapper.bpm()*2));
            vids[currentVid].setSpeed(vidSpeed);
            break;
        }

        //reverse clip
        case 'r':
        {
            vidSpeed =-vidSpeed;
            vids[currentVid].setSpeed(vidSpeed);
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
    gui2->addButton("resetBPM", false)->setLabelVisible(true);
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