// -*- C++ -*-
// -*- coding: utf-8 -*-
//
// Author: Joshua Cohen, Bryan Riel
// Copyright 2017-2018
//

#ifndef ISCE_IMAGE_RESAMPSLC_H
#define ISCE_IMAGE_RESAMPSLC_H

#include <cstdint>
#include <cstdio>
#include <complex>
#include <valarray>

// isce::core
#include "isce/core/Interpolator.h"
#include "isce/core/Poly2d.h"

// isce::io
#include "isce/io/Raster.h"

// isce::product
#include "isce/product/Product.h"

// isce::image
#include "Constants.h"
#include "Tile.h"

// Declarations
namespace isce {
    namespace image {
        class ResampSlc;
    }
}

// Definition
class isce::image::ResampSlc {

    // Public data members
    public:
        typedef Tile<std::complex<float>> Tile_t;

    // Meta-methods
    public:
        // Default constructor
        inline ResampSlc(const isce::product::Product &);
        // Constructor from isce::core objects
        inline ResampSlc(const isce::core::Poly2d, const isce::core::Metadata);
        // Destructor
        inline ~ResampSlc();

        // Polynomial getters
        inline isce::core::Poly2d rgCarrier() const;
        inline isce::core::Poly2d azCarrier() const;
        inline isce::core::Poly2d doppler() const;
        // Polynomial setters
        inline void rgCarrier(isce::core::Poly2d &);
        inline void azCarrier(isce::core::Poly2d &);
        inline void doppler(isce::core::Poly2d &);

        // Set reference product for flattening
        inline void referenceProduct(const isce::product::Product &);

        // Get image mode
        inline isce::product::ImageMode imageMode() const;
        inline isce::product::ImageMode refImageMode() const;
        // Set image mode
        inline void imageMode(const isce::product::ImageMode &);
        inline void refImageMode(const isce::product::ImageMode &);

        // Get/set number of lines per processing tile
        inline size_t linesPerTile() const;
        inline void linesPerTile(size_t);
                
        // Convenience functions
        inline void declare(int, int, int, int) const;

        // Alternative generic resamp entry point
        void resamp(const std::string & inputFilename, const std::string & outputFilename,
                    const std::string & rgOffsetFilename, const std::string & azOffsetFilename,
                    int inputBand, bool flatten=false, bool isComplex=true, int rowBuffer=40);

        // Main product-based resamp entry point
        void resamp(const std::string & outputFilename, const std::string & polarization,
                    const std::string & rgOffsetFilename, const std::string & azOffsetFilename,
                    bool flatten=false, bool isComplex=true, int rowBuffer=40);

    // Data members
    private:
        // Number of lines per tile
        size_t _linesPerTile = 1000;
        // Band number
        int _inputBand;
        // Filename of the input product
        std::string _filename;
        // Flag indicating if we have a reference mode
        bool _haveRefMode;

        // Polynomials
        isce::core::Poly2d _rgCarrier;            // range carrier polynomial
        isce::core::Poly2d _azCarrier;            // azimuth carrier polynomial
        isce::core::Poly2d _dopplerPoly;          // Doppler polynomial

        // ImageMode
        isce::product::ImageMode _mode;       // image mode for image to be resampled
        isce::product::ImageMode _refMode;    // image mode for reference master image

        // Array of sinc coefficient
        isce::core::Matrix<float> _fintp;

        // Tile initialization
        void _initializeTile(Tile_t &, isce::io::Raster &, isce::io::Raster &, int);

        // Tile transformation
        void _transformTile(Tile_t &, isce::io::Raster &, isce::io::Raster &,
                            isce::io::Raster &, int, bool, int &);

        // Convenience functions
        inline int _computeNumberOfTiles(int, int);

        // Resampling interpolation methods
        void _prepareInterpMethods(int);
        inline std::complex<float> _interpolateComplex(
            isce::core::Matrix<std::complex<float>> &,
            int, int, double, double, int, int
        );
};

// Get inline implementations for ResampSlc
#define ISCE_IMAGE_RESAMPSLC_ICC
#include "ResampSlc.icc"
#undef ISCE_IMAGE_RESAMPSLC_ICC

#endif

// end of file