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
    img = new ofImage();
    img->loadImage("ColorWheel.png");
    
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
    setGUI3();
    
    gui1->loadSettings("gui1Settings.xml");
    gui2->loadSettings("gui2Settings.xml");
    gui2->loadSettings("gui3Settings.xml");
    
    //Sound setup
    
    bang.loadSound("sounds/All I See (TCTS Remix).mp3");
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

    
    //B&W VIDEO
    string vid_url0 = "https://r9---sn-nwj7kned.googlevideo.com/videoplayback?key=yt5&id=o-AIgLpcJvssqf92tFzoo4R208vh6jEgnHbzHSwtgdCnfL&fexp=902904%2C907259%2C916631%2C927622%2C932404%2C935024%2C943917%2C947209%2C947228%2C948124%2C952302%2C952605%2C952901%2C953912%2C955105%2C955301%2C957103%2C957105%2C957201&ipbits=0&mm=31&mt=1418239353&initcwndbps=2222500&mime=video%2Fmp4&ip=2601%3A9%3A4980%3A920%3Aa9fa%3A52fc%3Abda3%3A2fb0&ms=au&mv=m&dur=77.577&source=youtube&signature=E053C1FB12ADC0A023D1EC4557C7E5A97DD06087.7F4D82CA388ADAF927624CADF3F3504DA1CF1F6E&ratebypass=yes&sver=3&requiressl=yes&sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmime%2Cmm%2Cms%2Cmv%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire&upn=7F5XNGdoWdk&itag=18&expire=1418260988";
    
    string vid_url1 = "https://r19---sn-o097znel.googlevideo.com/videoplayback?signature=429E3294E2C7EBA05ED051BE32D88B0078F7EAA9.1B1E187F6242DDBC78271F40F5216E9056C2C2FC&ms=au&mv=m&source=youtube&ratebypass=yes&initcwndbps=2182500&dur=161.935&upn=EtiEWReFCEs&requiressl=yes&mm=31&key=yt5&expire=1418252800&mt=1418231140&id=o-ALkvmzKy3wUFyk2yACqilwMt-h4YFDoWrrvpxDu9Mmba&itag=18&fexp=907259%2C913440%2C913587%2C916631%2C927622%2C932404%2C936117%2C936932%2C938692%2C941004%2C943917%2C947209%2C948124%2C952302%2C952605%2C952901%2C953000%2C953912%2C955301%2C957103%2C957105%2C957201&ipbits=0&sver=3&ip=67.161.9.40&sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmm%2Cms%2Cmv%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire";
    
    string vid_url2 = "https://r11---sn-nwj7kner.googlevideo.com/videoplayback?id=o-AFRceS5doO22C7cpeLHB-vsY43-djyP5D_Dmn_jPP0Je&initcwndbps=2052500&key=yt5&dur=91.533&mt=1418231140&signature=E007FE48EADDFB3A9937704EA162693F039B90C1.ED422E213051958CE7E7A941A6B61695C39CFA48&ms=au&source=youtube&mv=m&sver=3&ipbits=0&mm=31&requiressl=yes&itag=18&sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmm%2Cms%2Cmv%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire&ratebypass=yes&expire=1418252832&ip=67.161.9.40&upn=r9b8tkSJlZI&fexp=907259%2C922247%2C923349%2C924637%2C927622%2C932404%2C934947%2C942622%2C943917%2C947209%2C948124%2C952302%2C952605%2C952901%2C953912%2C957103%2C957105%2C957201";
    
    string vid_url3 = "https://r15---sn-o097znle.googlevideo.com/videoplayback?sver=3&expire=1418261458&dur=209.443&source=youtube&ms=au&mv=m&mt=1418239782&ratebypass=yes&mm=31&requiressl=yes&mime=video%2Fmp4&upn=dOC-V7a3dPI&id=o-AJy1nixO7HXNfuqngssW7ljhKN-TXvqmvR5Uaapu6WCq&initcwndbps=2246250&itag=18&ipbits=0&key=yt5&sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmime%2Cmm%2Cms%2Cmv%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire&signature=CC36E621C325BBB139F82F598104826FD1EC7F02.E84FEB9F5847040E23D66B870F5E58EF9277EC05&fexp=901441%2C901802%2C907259%2C916943%2C927622%2C932404%2C937435%2C939109%2C941004%2C943917%2C947209%2C948124%2C952302%2C952605%2C952901%2C953912%2C955105%2C955301%2C957103%2C957105%2C957201&ip=67.161.9.40";
    
    //COLOR VIDEO
//        string vid_url0 = "https://r11---sn-nwj7kner.googlevideo.com/videoplayback?fexp=900218%2C903903%2C907259%2C927622%2C931343%2C932404%2C940000%2C943917%2C947209%2C948124%2C949007%2C952302%2C952605%2C952901%2C953912%2C957103%2C957105%2C957201&mm=31&ip=67.161.9.40&dur=376.465&id=o-ADWw09b8h87ZUSrQ4rTKg5lEcORbHN_SBnl7DKyGI2s_&sver=3&key=yt5&ms=au&sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmm%2Cms%2Cmv%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire&source=youtube&initcwndbps=2087500&mv=m&ratebypass=yes&itag=18&signature=1F1718E5FD36C6FC9B43634A23F0D6497CE7BC03.B7E9D40DFF23EB2A6DC39035356FC57AABB51F98&upn=hd2CKz-csos&requiressl=yes&mt=1418231050&expire=1418252746&ipbits=0";
//        
//        string vid_url1 = "https://r16---sn-nwj7knl7.googlevideo.com/videoplayback?source=youtube&signature=4D93D92D2B79C61D92C5F61B573E22A204EC1CF3.A463CE55247A418D9C0BAAC480C14CC449DADF6A&ip=2601%3A9%3A4980%3A920%3Aa9fa%3A52fc%3Abda3%3A2fb0&sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmm%2Cms%2Cmv%2Crequiressl%2Csource%2Cupn%2Cexpire&sver=3&requiressl=yes&id=o-AOy-VBUlbSNIHDr2Y4q5uF0ar5aeWUqzCQ-zd2mQu2Zz&initcwndbps=2250000&ipbits=0&mv=m&expire=1418260055&mm=31&itag=18&mt=1418238371&ms=au&fexp=907259%2C912328%2C913440%2C917000%2C927622%2C929305%2C932404%2C934045%2C936109%2C937236%2C939109%2C941004%2C943917%2C947209%2C948124%2C952302%2C952605%2C952901%2C953912%2C955301%2C957103%2C957105%2C957201&key=yt5&upn=aG7srP5MNnM&dur=182.671&ratebypass=yes";
//    
//        string vid_url2 = "https://r10---sn-o097znek.googlevideo.com/videoplayback?upn=qzC5KNv1t-8&mv=m&mt=1418239108&ms=au&fexp=907259%2C927622%2C932404%2C936117%2C943917%2C947209%2C948124%2C952302%2C952605%2C952901%2C953913%2C955301%2C957103%2C957105%2C957201&id=o-ABaEYolz_0PBrJEkN56lqoQxpqjfn3uR12BPCNoJgq_A&sver=3&ratebypass=yes&mm=31&ip=67.161.9.40&itag=18&dur=255.582&initcwndbps=2153750&sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmime%2Cmm%2Cms%2Cmv%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire&source=youtube&requiressl=yes&expire=1418260738&mime=video%2Fmp4&key=yt5&ipbits=0&signature=8E5BBD1EC733CDE64A7AEFB2234BBF2E6A5000D2.39448D3CA0D7E4D039FD37E75846A8AA32C7CD24";
//        
//        string vid_url3 = "https://r1---sn-o097znee.googlevideo.com/videoplayback?mv=m&fexp=907259%2C927622%2C932404%2C938648%2C941427%2C942626%2C943917%2C947209%2C947601%2C948124%2C952302%2C952605%2C952901%2C953912%2C955301%2C957103%2C957105%2C957201&mt=1418231232&ms=au&id=o-ALUXpYWF8F1Cx8D5p6ECEiuA8fJFf4MgmrkAYRH0jc6C&expire=1418252927&signature=0B3A29546B436A8E19BDD1E050902F9F213CF6DD.F2B28A75E3DB1F7C1D6B9FF19800EEA20DC0094C&sver=3&ip=67.161.9.40&ipbits=0&ratebypass=yes&source=youtube&mm=31&sparams=dur%2Cid%2Cinitcwndbps%2Cip%2Cipbits%2Citag%2Cmm%2Cms%2Cmv%2Cratebypass%2Crequiressl%2Csource%2Cupn%2Cexpire&upn=MKycpsO6784&itag=18&requiressl=yes&initcwndbps=2067500&key=yt5&dur=85.124";
    
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
    ofSetColor(red,green,blue);
    
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

    ofSetColor(0);
    ofDrawBitmapString("BPM: " + ofToString(bpmTapper[currentVid].bpm()), 50, 560);


    
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
            vidFull=!vidFull;
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
        case 'l':
        {
            
            vidSpeed =  .01*(100*BPM/(bpmTapper[currentVid].bpm()/2));
            vids[currentVid].setSpeed(vidSpeed);
            break;
        }
        case 'k':
        {
            vidSpeed =  .01*(100*BPM/(bpmTapper[currentVid].bpm()*2));
            vids[currentVid].setSpeed(vidSpeed);
            break;
        }

        //reverse clip
        case 'r':
        {
            vidSpeed =-vidSpeed;
            for(int all=0; all<4; all++){
            vids[all].setSpeed(vidSpeed);
            }
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
    
    gui2->setPosition(212*5,0);
    gui2->autoSizeToFitWidgets();
    
    ofAddListener(gui2->newGUIEvent,this,&ofApp::guiEvent);
}

void ofApp::setGUI3()
{
    gui3 = new ofxUISuperCanvas("");
    gui3->setDrawBack(false);
    gui3->setGlobalButtonDimension(24);
    vector<string> items;
    items.push_back("Bang - EDX"); items.push_back("EDM Trend Machine - Knife Party"); items.push_back("Okay - Shiba San");
    items.push_back("All I See - Bondax"); items.push_back("This Could Be Love - Borgeous"); items.push_back("Milking - Croatia Squad"); items.push_back("Drop That Skirt - Croatia Squad");items.push_back("True - Nora En Pure");items.push_back("Sleepless - Cazzette");
    
   
    gui3->addLabel("Filter Color", OFX_UI_FONT_MEDIUM);
    gui3->addImageSampler("SAMPLER", img);
    gui3->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    ddl = gui3->addDropDownList("Select Song", items);
    ddl->setAllowMultiple(false);
    gui3->setGlobalButtonDimension(OFX_UI_GLOBAL_BUTTON_DIMENSION);
    
    gui3->setPosition(212*2, 0);
    gui3->autoSizeToFitWidgets();
    
    
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