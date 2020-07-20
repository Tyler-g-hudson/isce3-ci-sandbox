#cython: language_level=3
#
# Author: Bryan Riel
# Copyright 2017-2018
#

from libcpp.string cimport string
from libcpp cimport bool

# Cython declaration for isce3::io objects
from isceextension cimport LUT2d
from isceextension cimport Poly2d
from isceextension cimport Product
from isceextension cimport Raster
from isceextension cimport Swath

cdef extern from "isce3/cuda/image/ResampSlc.h" namespace "isce3::cuda::image":

    # ResampSlc class
    cdef cppclass ResampSlc:

        # Default constructor with a Product (no flattening)
        ResampSlc(const Product & product, char) except +

        # Constructor from an isce3::product::Product and reference product (flattening)
        ResampSlc(const Product & product,
                  const Product & refProduct,
                  char) except +

        # Constructor from individual components (no flattening)
        ResampSlc(const LUT2d[double] & doppler,
                  double startingRange, double rangePixelSpacing,
                  double sensingStart, double prf, double wvl) except +

        # Constructor from individual components (flattening)
        ResampSlc(const LUT2d[double] & doppler,
                  double startingRange, double rangePixelSpacing,
                  double sensingStart, double prf, double wvl,
                  double refStartingRange, double refRangePixelSpacing,
                  double refWvl) except +

        # Polynomial getters
        Poly2d rgCarrier()
        Poly2d azCarrier()

        # Polynomial setters
        void rgCarrier(Poly2d &)
        void azCarrier(Poly2d &)

        # Doppler reference
        LUT2d[double] & doppler()
        void doppler(const LUT2d[double] & lut)

        # Set reference product
        void referenceProduct(const Product & refProduct, char freq)
        
        # Get/set number of lines per processing tile
        size_t linesPerTile()
        void linesPerTile(size_t)

        # Main product-based resamp entry point
        void resamp(const string &, const string &, const string &,
                    const string &, bool, bool, int)

        # Generic resamp entry point: use filenames to create rasters
        void resamp(const string &, const string &, const string &,
                    const string &, int, bool, bool, int)

        # Generic resamp entry point from externally created rasters
        void resamp(Raster &, Raster &, Raster &, Raster &,
                    int, bool, bool, int)
            
# end of file
