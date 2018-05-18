//
//  box.h
//  3D_Martian_Surface_V3
//
//  Created by Gaurav Gupta on 5/6/18.
//

#ifndef _BOX_H_
#define _BOX_H_

#include <assert.h>
#include "vector3.h"
#include "ray.h"

/*
 * Axis-aligned bounding box class, for use with the optimized ray-box
 * intersection test described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */

class Box {
public:
    Box() { }
    Box(const Vector3 &min, const Vector3 &max) {
        assert(min < max);
        parameters[0] = min;
        parameters[1] = max;
    }
    // (t0, t1) is the interval for valid hits
    bool intersect(const Ray &, float t0, float t1) const;
    Vector3 getMidPointOfBox();
    float boxArea(){return abs((max().x()-min().x())*(max().y()-min().y())*(max().z()-min().z()));}
    // corners
    Vector3 parameters[2];
    Vector3 min() { return parameters[0]; }
    Vector3 max() { return parameters[1]; }
};

#endif // _BOX_H_

