
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Mars HiRise Project - startup scene
//
//  This is an openFrameworks 3D scene that includes an EasyCam
//  and example 3D geometry which I have reconstructed from Mars
//  HiRis photographs taken the Mars Reconnaisance Orbiter
//
//  You will use this source file (and include file) as a starting point
//  to implement assignment 5  (Parts I and II)
//
//  Please do not modify any of the keymappings.  I would like
//  the input interface to be the same for each student's
//  work.  Please also add your name/date below.

//  Please document/comment all of your work !
//  Have Fun !!
//
//  Student Name:   < Gaurav Gupta >
//  Date: <Apr,15,2018>


#include "ofApp.h"
#include "Util.h"



//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
    
    bWireframe = false;
    bPointSelectedNotWithMesh = false;
    bDisplayPoints = false;
    bAltKeyDown = false;
    bCtrlKeyDown = false;
    bRoverLoaded = false;
    bTerrainSelected = true;
    //    ofSetWindowShape(1024, 768);
    cam.setDistance(10);
    cam.setNearClip(.1);
    cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
    ofSetVerticalSync(true);
    cam.disableMouseInput();
    ofEnableSmoothing();
    ofEnableDepthTest();
    
    // setup rudimentary lighting
    //
    initLightingAndMaterials();
    
    mars.loadModel("geo/mars-low-v2.obj");
    mars.setScaleNormalization(false);
    meshDataForMars = mars.getMesh(0);
    boundingBox = meshBounds(meshDataForMars);
    isDragged = false;
    bShiftKeyDown = false;
    path = ofPath();
    
    //Add colors to the vector
    colors.push_back(ofColor::yellow);
    colors.push_back(ofColor::green);
    colors.push_back(ofColor::dimGrey);
    colors.push_back(ofColor::indianRed);
    colors.push_back(ofColor::skyBlue);
}

//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    uint64_t st = ofGetSystemTimeMicros();
    //    ofBackgroundGradient(ofColor(20), ofColor(0));   // pick your own background
    ofBackground(ofColor::black);
    //        cout << ofGetFrameRate() << endl;
    
    
    cam.begin();
    ofPushMatrix();
    if (bWireframe) {                    // wireframe mode  (include axis)
        ofDisableLighting();
        ofSetColor(ofColor::slateGray);
        mars.drawWireframe();
        if (bRoverLoaded) {
            rover.drawWireframe();
            if (!bTerrainSelected) drawAxis(rover.getPosition());
        }
        if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
    }
    else {
        ofEnableLighting();              // shaded mode
        mars.drawFaces();
        
        if (bRoverLoaded) {
            rover.drawFaces();
            if (!bTerrainSelected) drawAxis(rover.getPosition());
        }
        if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
    }
    
    
    if (bDisplayPoints) {                // display points as an option
        glPointSize(3);
        ofSetColor(ofColor::green);
        mars.drawVertices();
    }
    
    
    
    // highlight selected point (draw sphere around selected point)
    //
    if (bPointSelected) {
        ofSetColor(ofColor::blue);
        ofDrawSphere(selectedPoint, .1);
    }
    if(pathPoints.size()>0){
        for(int i=0; i<pathPoints.size(); i++){
            ofSetColor(ofColor::yellow);
            ofDrawSphere(pathPoints[i], .1);
        }
    }
    /*
     Professors Code to Draw the point for the given mesh. Uncomment this to check the draw functionality.
     
     if (bPointSelectedNotWithMesh) {
     ofSetColor(ofColor::green);
     ofDrawSphere(selectedPointWithoutMesh, .1);
     }
     */
    ofNoFill();
    ofSetColor(ofColor::white);
    drawBox(boundingBox);
    int colorsSize = colors.size();
    for(int i =0; i<subLevelBoxes.size(); i++){
        //        if(i%2==0)
        //            ofSetColor(ofColor::red);
        //        else
        //            ofSetColor(ofColor::green);
        ofSetColor(colors[i%colorsSize]);
        for (int j=0; j < subLevelBoxes[i].size(); j++){
            drawBox(subLevelBoxes[i][j]);
        }
    }
    
    ofPopMatrix();
    cam.end();
    uint64_t et = ofGetSystemTimeMicros();
    cout<<"Time elapsed in entire draw function : "<<et-st<<endl;
}

//

// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {
    
    ofPushMatrix();
    ofTranslate(location);
    
    ofSetLineWidth(1.0);
    
    // X Axis
    ofSetColor(ofColor(255, 0, 0));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
    
    
    // Y Axis
    ofSetColor(ofColor(0, 255, 0));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));
    
    // Z Axis
    ofSetColor(ofColor(0, 0, 255));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));
    
    ofPopMatrix();
}


void ofApp::keyPressed(int key) {
    
    switch (key) {
        case 'C':
        case 'c':
            if (cam.getMouseInputEnabled()) cam.disableMouseInput();
            else cam.enableMouseInput();
            break;
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case 'H':
        case 'h':
            break;
        case 'r':
            cam.reset();
            break;
        case 's':
            savePicture();
            break;
        case 't':
            setCameraTarget();
            break;
        case 'u':
            break;
        case 'v':
            togglePointsDisplay();
            break;
        case 'V':
            break;
        case 'w':
            toggleWireframeMode();
            break;
        case OF_KEY_ALT:
            cam.enableMouseInput();
            bAltKeyDown = true;
            break;
        case OF_KEY_CONTROL:
            bCtrlKeyDown = true;
            break;
        case OF_KEY_SHIFT:
            bShiftKeyDown = true;
            break;
        case OF_KEY_DEL:
            break;
        default:
            break;
    }
}

void ofApp::toggleWireframeMode() {
    bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
    bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
    bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {
    
    switch (key) {
            
        case OF_KEY_ALT:
            cam.disableMouseInput();
            bAltKeyDown = false;
            break;
        case OF_KEY_CONTROL:
            bCtrlKeyDown = false;
            break;
        case OF_KEY_SHIFT:
            bShiftKeyDown = false;
            break;
        default:
            break;
            
    }
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    startTime = ofGetSystemTimeMicros();
}


//Check Mouse Ray Collision distance from the center
float ofApp::checkBoxDistanceFromCenter(const Box &box,Ray &ray){
    float distance = -1;
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    distance = center.distance(ray.origin);
    //    cout<<"Distance to the box is :\t"<<distance<<endl;
    return distance;
    
}

//draw a box from a "Box" class
//
void ofApp::drawBox(const Box &box) {
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
    float w = size.x();
    float h = size.y();
    float d = size.z();
    ofDrawBox(p, w, h, d);
}

// return a Mesh Bounding Box for the entire Mesh
//
Box ofApp::meshBounds(const ofMesh & mesh) {
    int n = mesh.getNumVertices();
    ofVec3f v = mesh.getVertex(0);
    ofVec3f max = v;
    ofVec3f min = v;
    for (int i = 1; i < n; i++) {
        ofVec3f v = mesh.getVertex(i);
        
        if (v.x > max.x) max.x = v.x;
        else if (v.x < min.x) min.x = v.x;
        
        if (v.y > max.y) max.y = v.y;
        else if (v.y < min.y) min.y = v.y;
        
        if (v.z > max.z) max.z = v.z;
        else if (v.z < min.z) min.z = v.z;
    }
    return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

//  Subdivide a Box into eight(8) equal size boxes, return them in boxList;
//
void ofApp::subDivideBox8(const Box &box, vector<Box> & boxList) {
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    float xdist = (max.x() - min.x()) / 2;
    float ydist = (max.y() - min.y()) / 2;
    float zdist = (max.z() - min.z()) / 2;
    Vector3 h = Vector3(0, ydist, 0);
    
    //  generate ground floor
    //
    Box b[8];
    b[0] = Box(min, center);
    b[1] = Box(b[0].min() + Vector3(xdist, 0, 0), b[0].max() + Vector3(xdist, 0, 0));
    b[2] = Box(b[1].min() + Vector3(0, 0, zdist), b[1].max() + Vector3(0, 0, zdist));
    b[3] = Box(b[2].min() + Vector3(-xdist, 0, 0), b[2].max() + Vector3(-xdist, 0, 0));
    
    boxList.clear();
    for (int i = 0; i < 4; i++)
        boxList.push_back(b[i]);
    
    // generate second story
    //
    for (int i = 4; i < 8; i++) {
        b[i] = Box(b[i - 4].min() + h, b[i - 4].max() + h);
        boxList.push_back(b[i]);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    isDragged = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    if(bShiftKeyDown){
        subLevelBoxes.clear();
        sublevelMeshes.clear();
        
        ofVec3f mouse(mouseX, mouseY);
        ofVec3f rayPoint = cam.screenToWorld(mouse);
        ofVec3f rayDir = rayPoint - cam.getPosition();
        rayDir.normalize();
        Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
                      Vector3(rayDir.x, rayDir.y, rayDir.z));
        if(boundingBox.intersect(ray, -100, 100)){
            doPointSelectionForPath();
        }
    }else{
        if(!isDragged){
            bPointSelected = false;
            bPointSelectedNotWithMesh=false;
            subLevelBoxes.clear();
            sublevelMeshes.clear();
            
            ofVec3f mouse(mouseX, mouseY);
            ofVec3f rayPoint = cam.screenToWorld(mouse);
            ofVec3f rayDir = rayPoint - cam.getPosition();
            rayDir.normalize();
            Ray ray = Ray(Vector3(rayPoint.x, rayPoint.y, rayPoint.z),
                          Vector3(rayDir.x, rayDir.y, rayDir.z));
            if(boundingBox.intersect(ray, -100, 100)){
                helperSubLevelBoundingBoxes(boundingBox, 1, ray, meshDataForMars);
                doPointSelection();
            }
        }
    }
    isDragged = false;
    endTime = ofGetSystemTimeMicros();
    cout<<endl<<endl<<"Time Elapsed to find all the sub level bounding boxes : "<<endTime-startTime<<endl<<endl;
}
//create path points using the clicks;
bool ofApp::doPointSelectionForPath(){
    
    ofMesh mesh = mars.getMesh(0);
    int n = mesh.getNumVertices();
    float nearestDistance = 0;
    int nearestIndex = 0;
    ofVec3f pointSelected;
    
    bPointSelectedNotWithMesh = false;
    
    ofVec2f mouse(mouseX, mouseY);
    vector<ofVec3f> selection;
    
    // We check through the mesh vertices to see which ones
    // are "close" to the mouse point in screen space.  If we find
    // points that are close, we store them in a vector (dynamic array)
    //
    for (int i = 0; i < n; i++) {
        ofVec3f vert = mesh.getVertex(i);
        ofVec3f posScreen = cam.worldToScreen(vert);
        float distance = posScreen.distance(mouse);
        if (distance < selectionRange) {
            selection.push_back(vert);
            bPointSelectedNotWithMesh = true;
        }
    }
    
    //  if we found selected points, we need to determine which
    //  one is closest to the eye (camera). That one is our selected target.
    //
    if (bPointSelectedNotWithMesh) {
        float distance = 0;
        for (int i = 0; i < selection.size(); i++) {
            ofVec3f point =  cam.worldToCamera(selection[i]);
            
            // In camera space, the camera is at (0,0,0), so distance from
            // the camera is simply the length of the point vector
            //
            float curDist = point.length();
            
            if (i == 0 || curDist < distance) {
                distance = curDist;
                pointSelected = selection[i];
            }
        }
    }
    pathPoints.push_back(pointSelected);
    return bPointSelectedNotWithMesh;
}

//code helps in creating the sub level bounding boxes
void ofApp::helperSubLevelBoundingBoxes(const Box &b, int currentLevel, Ray & ray, ofMesh &mesh){
    //check the level, if it is more than the depth of what we are expecting then leave it
    if((currentLevel>maxLevel && maxLevel>0)||mesh.getNumVertices()<2){
        return;
    }
    //    cout<<currentLevel<<endl;
    //Create vector of sub boxes for the current level
    vector<Box> boxList;
    subDivideBox8(b, boxList);
    subLevelBoxes.push_back(boxList);
    buildTreeNodeForBoxes(boxList, mesh);
    TreeNode currentTree = sublevelMeshes[sublevelMeshes.size()-1];
    int index = indexOfClosestBoundingBox(boxList, ray, currentTree);
    if(index<0){
        //        setSelectedPoint(b);
        return;//this means no more sub boxes could be divided in the reigon.
    }
    helperSubLevelBoundingBoxes(boxList[index], currentLevel+1, ray, currentTree.verticesOfBoxes[index]);
    if(!bPointSelected){
        doPointSelectionWithMesh(currentTree.verticesOfBoxes[index]);
    }
}
//IF the final layer is found then this will draw the sphere around the last box.
void ofApp::setSelectedPoint(const Box box){
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    bPointSelected = true;
    selectedPoint.set(center.x(), center.y(), center.z());
}

//consturcts the sublevel vertice division for a given box
void ofApp::buildTreeNodeForBoxes(vector<Box> &boxes, ofMesh allVertices){
    TreeNode tree;
    
    for (int i=0; i<boxes.size(); i++) {
        ofMesh mesh;
        fetchMeshDataForBox(boxes[i], mesh, allVertices);
        tree.add(mesh);
    }
    sublevelMeshes.push_back(tree);
}

//For each individual box in the octree find the vertices for a given box.
void ofApp::fetchMeshDataForBox(Box box, ofMesh &mesh, ofMesh allVertices){
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    int n = allVertices.getNumVertices();
    for (int i = 0; i < n; i++) {
        ofVec3f v = allVertices.getVertex(i);
        if ((v.x >= min.x() && v.x<=max.x()) && (v.y >= min.y() && v.y<=max.y()) && (v.z >= min.z() && v.z<=max.z()))
            mesh.addVertex(v);
    }
}

// Find the bounding box that is closest for the given ray which intersects at a closest distance
// Initially the max Distance to the closest box is set to maximum value of float,
// if any box intersect the given ray, then the distance is checed with the current maximum distance
// and smaller of the two distance is chosen as closestDistance.

int ofApp::indexOfClosestBoundingBox(vector<Box> &boxList, Ray &ray, const TreeNode tree){
    vector<ofMesh> verticeList = tree.verticesOfBoxes;
    float closestBoxDistance = numeric_limits<float>::max();
    int indexOfMinDistaceBox = -1; // initially set to 1, which depicts that no index of the box is found;
    for (int i=0; i<boxList.size(); i++) {
        if(boxList[i].intersect(ray, -100, 100) && verticeList[i].getNumVertices()>0){
            float tempDistance = checkBoxDistanceFromCenter(boxList[i], ray);
            if(tempDistance<closestBoxDistance){
                closestBoxDistance = tempDistance;
                indexOfMinDistaceBox = i;
            }
        }
    }
    return indexOfMinDistaceBox;
}

void ofApp::doPointSelectionWithMesh(const ofMesh mesh) {
    
    int n = mesh.getNumVertices();
    float nearestDistance = 0;
    int nearestIndex = 0;
    
    bPointSelected = false;
    
    ofVec2f mouse(mouseX, mouseY);
    vector<ofVec3f> selection;
    
    // We check through the mesh vertices to see which ones
    // are "close" to the mouse point in screen space.  If we find
    // points that are close, we store them in a vector (dynamic array)
    //
    for (int i = 0; i < n; i++) {
        ofVec3f vert = mesh.getVertex(i);
        ofVec3f posScreen = cam.worldToScreen(vert);
        float distance = posScreen.distance(mouse);
        if (distance < selectionRange) {
            selection.push_back(vert);
            bPointSelected = true;
        }
    }
    
    //  if we found selected points, we need to determine which
    //  one is closest to the eye (camera). That one is our selected target.
    //
    if (bPointSelected) {
        float distance = 0;
        for (int i = 0; i < selection.size(); i++) {
            ofVec3f point =  cam.worldToCamera(selection[i]);
            
            // In camera space, the camera is at (0,0,0), so distance from
            // the camera is simply the length of the point vector
            //
            float curDist = point.length();
            
            if (i == 0 || curDist < distance) {
                distance = curDist;
                selectedPoint = selection[i];
            }
        }
    }
}

//
//  Select Target Point on Terrain by comparing distance of mouse to
//  vertice points projected onto screenspace.
//  if a point is selected, return true, else return false;
//
bool ofApp::doPointSelection() {
    
    ofMesh mesh = mars.getMesh(0);
    int n = mesh.getNumVertices();
    float nearestDistance = 0;
    int nearestIndex = 0;
    
    bPointSelectedNotWithMesh = false;
    
    ofVec2f mouse(mouseX, mouseY);
    vector<ofVec3f> selection;
    
    // We check through the mesh vertices to see which ones
    // are "close" to the mouse point in screen space.  If we find
    // points that are close, we store them in a vector (dynamic array)
    //
    for (int i = 0; i < n; i++) {
        ofVec3f vert = mesh.getVertex(i);
        ofVec3f posScreen = cam.worldToScreen(vert);
        float distance = posScreen.distance(mouse);
        if (distance < selectionRange) {
            selection.push_back(vert);
            bPointSelectedNotWithMesh = true;
        }
    }
    
    //  if we found selected points, we need to determine which
    //  one is closest to the eye (camera). That one is our selected target.
    //
    if (bPointSelected) {
        float distance = 0;
        for (int i = 0; i < selection.size(); i++) {
            ofVec3f point =  cam.worldToCamera(selection[i]);
            
            // In camera space, the camera is at (0,0,0), so distance from
            // the camera is simply the length of the point vector
            //
            float curDist = point.length();
            
            if (i == 0 || curDist < distance) {
                distance = curDist;
                selectedPointWithoutMesh = selection[i];
            }
        }
    }
    return bPointSelectedNotWithMesh;
}

// Set the camera to use the selected point as it's new target
//
void ofApp::setCameraTarget() {
    
}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}



//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {
    
    static float ambient[] =
    { .5f, .5f, .5, 1.0f };
    static float diffuse[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };
    
    static float position[] =
    {5.0, 5.0, 5.0, 0.0 };
    
    static float lmodel_ambient[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };
    
    static float lmodel_twoside[] =
    { GL_TRUE };
    
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //    glEnable(GL_LIGHT1);
    glShadeModel(GL_SMOOTH);
}

void ofApp::savePicture() {
    ofImage picture;
    picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    picture.save("screenshot.png");
    cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
    
    ofVec3f point;
    mouseIntersectPlane(ofVec3f(0, 0, 0), cam.getZAxis(), point);
    
    if (rover.loadModel(dragInfo.files[0])) {
        rover.setScaleNormalization(false);
        rover.setScale(.005, .005, .005);
        rover.setPosition(point.x, point.y, point.z);
        bRoverLoaded = true;
    }
    else cout << "Error: Can't load model" << dragInfo.files[0] << endl;
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
    ofVec2f mouse(mouseX, mouseY);
    ofVec3f rayPoint = cam.screenToWorld(mouse);
    ofVec3f rayDir = rayPoint - cam.getPosition();
    rayDir.normalize();
    return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}
