#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include  "ofxAssimpModelLoader.h"
#include "box.h"
#include "ray.h"

//This will hold the mesh data for every region given the level
class TreeNode{
public:
    //Since in octree there will be
    vector<ofMesh> verticesOfBoxes;
    void add(ofMesh mesh){
        verticesOfBoxes.push_back(mesh);
    }
};

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
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void drawAxis(ofVec3f);
    void initLightingAndMaterials();
    void savePicture();
    void toggleWireframeMode();
    void togglePointsDisplay();
    void toggleSelectTerrain();
    void setCameraTarget();
    bool  doPointSelection();
    ofVec3f  doPointSelectionForPath();
    void drawBox(const Box &box);
    Box meshBounds(const ofMesh &);
    void subDivideBox8(const Box &b, vector<Box> & boxList);
    bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
    
    
    void updateCam();
    void helperSubLevelBoundingBoxes(const Box &b, int currentLevel, Ray &ray, ofMesh &mesh);
    int indexOfClosestBoundingBox(vector<Box> & boxList, Ray &ray, const TreeNode tree);
    float checkBoxDistanceFromCenter(const Box & b, Ray &ray);
    void buildTreeNodeForBoxes(vector<Box> &boxes, ofMesh verticesList);
    void fetchMeshDataForBox(Box box,ofMesh &mesh, ofMesh verticesList);
    void setSelectedPoint(const Box box);
    void checkIfPointSelected();
    void updateSelectedPoint();
    bool isPointSelected;
    int selectedPointIndex;
    ofEasyCam cam;
    vector<ofEasyCam> cameras;
    int cameraIndex;
    bool changeCameraRequest;
    void setCamerasAfterRoverLoaded();
    ofxAssimpModelLoader mars, rover;
    ofLight light;
    Box boundingBox;
    Box roverBoundingBox;

    vector<vector<Box>> subLevelBoxes;
    vector<TreeNode> sublevelMeshes;
    vector<ofColor> colors;
    bool bAltKeyDown;
    bool bCtrlKeyDown;
    bool bWireframe;
    bool bDisplayPoints;
    bool bPointSelected;
    bool resetRoverPosition;
    bool enterKeyPressed;
    bool bPointSelectedNotWithMesh;
    ofVec3f selectedPointWithoutMesh;
    vector<ofPoint> pathPoints;
    vector<ofPoint> subPoints;
    bool pathChanged;
    bool bRoverLoaded;
    bool bTerrainSelected;
    bool isDragged;
    bool checkIfPointExistInSubPath(ofVec3f point);
    void doPointSelectionForSubPath(ofVec3f start, ofVec3f end, int level);
    void doPointSelectionWithMesh(const ofMesh mesh);
    ofMesh meshDataForMars;
    vector<ofMesh> meshDataForRover;
    bool roverSelected;
    int roverSpeed;
    bool startRover;
    int roverPosition;
    int frameIndex;
    bool showPointsOnly;
    void updateRoverBox(ofVec3f point);
    bool pKeyPressed;
    bool moveForwardDirection;
    bool roverSelectedForDragging;
    bool isRoverSelected();
    ofVec3f selectedPoint;
    ofVec3f intersectPoint;
    ofPolyline path;
    int maxLevel = 10;
    uint64_t startTime, endTime;
    const float selectionRange = 4.0;
    bool bShiftKeyDown;
    bool checkRoverIsOnPath();
    bool checkRoverProximity(int index, ofVec3f centerPoint);
    ofxIntSlider speedSlider;
    ofxPanel gui;
};
