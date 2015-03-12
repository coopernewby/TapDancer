#pragma once

#include "ofMain.h"
#include "ofxUI.h"

#include "MSABPMTapper.h"
#include "ofxMidi.h"

class ofApp : public ofBaseApp, public ofxMidiListener{
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    float 	counter;
    
    //Number of video panels
    bool vidPanels = false;
    bool vidFull = false;
    int currentVid = 0;
    ofVideoPlayer vids[4];
    float vidSpeed = 1;

    void audioRequested (float * output, int bufferSize, int nChannels);
    void setBPM(float targetBPM);
    
    //MIDI mesaages
    void newMidiMessage(ofxMidiMessage& eventArgs);
    stringstream text;
    ofxMidiIn midiIn;
    ofxMidiMessage midiMessage;
    
    int messageIndex;
    
    int frameView;
    int secondView;
    int minView;
    int hourView;
    
    bool frameReset;
    
    int testCounter;
    
    int tempoDenominator;
    float ticksPerBar;
    int tempoNumerator;
    int ticksPerqNote;
    int ticks32thNotePerBar;
    int ticksPer32thNote;
    int tempoTicks;
    int ticksfor32thNote;
    int num32thNotes;
    int tempoqNotes;
    int tempoBars;
    bool isPlaying;
    
    int midiTempoTest;
    int midiTempoCal;
    
    
    ofSoundPlayer bang;
//    ofSoundPlayer skirt;
    
    int cuebang = 0;
//    int cueskirt = 0;
    // Loop bool and Loop length in beats (i.e. 16 = 4 bars long loop)
    bool loop;
    int loopLength;

    
    ofTrueTypeFont	font;
    int BTNdim;
    int pos;
    float BPM;
    int SAMPLE_RATE;
    float lengthOfOneBeatInSamples;
    
    //sets GUI and draggable super canvas
    void setGUI1();
    void setGUI2();
    
    ofxUICanvas *gui;
    ofxUISuperCanvas *gui1;
    ofxUISuperCanvas *gui2;
    
    
    ofxUITextInput *textInput;
    
    bool hideGUI;
    
    
    bool bdrawPadding;
    
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUIDropDownList *ddl;
    ofxUIToggleMatrix *tm;
    
    float *buffer;
    
    //FFT bands
    int nBandsToGet;
    float 				* fftSmoothed;
    
    msa::BPMTapper  bpmTapper[4];
    float vidbpm;
    
    int newW;
    int newH;
    
    ofImage *img;
    
    
    

    

};

