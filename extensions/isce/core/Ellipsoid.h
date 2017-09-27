//
// Author: Joshua Cohen
// Copyright 2017
//

#ifndef __ISCE_CORE_ELLIPSOID_H__
#define __ISCE_CORE_ELLIPSOID_H__

#include <cmath>
#include <vector>
#include "Constants.h"

namespace isce { namespace core {
    struct Ellipsoid {
        // Major semi-axis
        double a;
        // Eccentricity squared
        double e2;

        Ellipsoid(double maj, double ecc) : a(maj), e2(ecc) {}
        Ellipsoid() : Ellipsoid(0.,0.) {}
        Ellipsoid(const Ellipsoid &e) : a(e.a), e2(e.e2) {}
        inline Ellipsoid& operator=(const Ellipsoid&);

        inline double rEast(double) const;
        inline double rNorth(double) const;
        inline double rDir(double,double) const;
        //void latLon(std::vector<double>&,std::vector<double>&,latLonConvMethod);
        void latLonToXyz(std::vector<double>&,std::vector<double>&);
        void xyzToLatLon(std::vector<double>&,std::vector<double>&);
        void getAngs(std::vector<double>&,std::vector<double>&,std::vector<double>&,double&,
                     double&);
        void getTCN_TCvec(std::vector<double>&,std::vector<double>&,std::vector<double>&,
                          std::vector<double>&);
        void TCNbasis(std::vector<double>&,std::vector<double>&,std::vector<double>&,
                      std::vector<double>&,std::vector<double>&);
    };

    inline Ellipsoid& Ellipsoid::operator=(const Ellipsoid &rhs) {
        a = rhs.a;
        e2 = rhs.e2;
        return *this;
    }

    inline double Ellipsoid::rEast(double lat) const{ return a / sqrt(1. - (e2 * pow(sin(lat), 2))); }

    inline double Ellipsoid::rNorth(double lat) const { return (a * (1. - e2)) / 
                                                          pow((1. - (e2 * pow(lat, 2))), 1.5); }

    inline double Ellipsoid::rDir(double hdg, double lat) const {
        auto re = rEast(lat);
        auto rn = rNorth(lat);
        return (re * rn) / ((re * pow(cos(hdg), 2)) + (rn * pow(sin(hdg), 2)));
    }
}}

#endif
