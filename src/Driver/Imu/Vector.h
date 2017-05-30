#ifndef Driver_Imu_Vector_h_
#define Driver_Imu_Vector_h_
/*----------------------------------------------------------------------------- 
* This file is part of the Arduino Project.  The Arduino Project is an   
* open source project with a BSD type of licensing agreement.  See the license  
* agreement (license.txt) in the top/ directory or on the Internet at           
* http://integerfox.com/arduino/license.txt
*                                                                               
* Copyright (c) 2017  John T. Taylor                                        
*                                                                               
* Redistributions of the source code must retain the above copyright notice.    
*----------------------------------------------------------------------------*/ 
/** @file */


namespace Driver {
namespace Imu {

/** This template class contains the vector results from an IMU.
 */
template<class T>
class Vector
{
public:
    /// Constructor
    Vector( T initX=0, T initY=0, T initZ=0 ):x(initX),y(initY),z(initZ){}

    /// Assignment operator
    Vector<T>& operator=(const Vector<T>& src)
    {
        x = src.x;
        y = src.y;
        z = src.z;

        return *this;
    }

public:
    /// X-Axis
    T   x;

    /// Y-Axis
    T   y;

    /// Z-Axis
    T   z;
};

/** This template class contains the quaternion results from an IMU.
 */
template<class T>
class Quat
{
public:
    /// Constructor
    Quat( T initW=0, T initX=0, T initY=0, T initZ=0 ):x(initX),y(initY),z(initZ){}

    /// Assignment operator
    Quat<T>& operator=(const Quat<T>& src)
    {
        w = src.w;
        x = src.x;
        y = src.y;
        z = src.z;

        return *this;
    }

public:
    /// W
    T   w;

    /// X-Axis
    T   x;

    /// Y-Axis
    T   y;

    /// Z-Axis
    T   z;
};


};      // end Namespaces
};
#endif  // end Header latch
