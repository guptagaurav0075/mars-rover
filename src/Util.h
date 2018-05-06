//
//  Util.h
//  3D_Martian_Surface_V3
//
//  Created by Gaurav Gupta on 5/6/18.
//

#pragma once
//  Kevin M. Smith - CS 134 SJSU

#include "ofMain.h"

bool rayIntersectPlane(const ofVec3f &rayPoint, const ofVec3f &raydir, ofVec3f const &planePoint,
                       const ofVec3f &planeNorm, ofVec3f &point);

ofVec3f reflectVector(const ofVec3f &v, const ofVec3f &normal);



