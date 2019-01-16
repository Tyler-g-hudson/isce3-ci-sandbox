#cython: language_level=3
#
# Author: Bryan V. Riel
# Copyright 2018
#

from libcpp.vector cimport vector
from libcpp.string cimport string
from Cartesian cimport cartesian_t, cartmat_t

cdef extern from "isce/core/Attitude.h" namespace "isce::core":
    cdef cppclass Attitude:
        cartesian_t ypr()
        cartmat_t rotmat(string)

cdef extern from "isce/core/EulerAngles.h" namespace "isce::core":
    cdef cppclass EulerAngles(Attitude):
        # Getter functions for attitude angles
        double yaw()
        double pitch()
        double roll()
        # Setter functions for attitude angles
        void yaw(double)
        void pitch(double)
        void roll(double)
        # Constructor 
        EulerAngles(double, double, double, string) except +
        # Convert to quaternion
        vector[double] toQuaternionElements()

# end of file