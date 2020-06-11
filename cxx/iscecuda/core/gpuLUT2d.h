#pragma once

#include "forward.h"
#include <isce/core/forward.h>
#include <isce/cuda/core/forward.h>

#include <cmath>

#include <isce/core/Common.h>
#include <isce/core/Constants.h>

namespace isce { namespace cuda { namespace core {

template<typename T>
class gpuLUT2d {
public:
    /** Deep copy constructor from CPU LUT1d */
    gpuLUT2d(const isce::core::LUT2d<T>&);

    /** Deep copy constructor */
    gpuLUT2d(const gpuLUT2d<T>&);

    /** Shallow copy constructor on device */
    CUDA_HOSTDEV gpuLUT2d(gpuLUT2d<T>&);

    /** Shallow assignment operator on device */
    CUDA_HOSTDEV gpuLUT2d& operator=(gpuLUT2d<T>&);

    /** Destructor */
    ~gpuLUT2d();

    /** Get starting X-coordinate */
    CUDA_HOSTDEV double xStart() const { return _xstart; }

    /** Get starting Y-coordinate */
    CUDA_HOSTDEV double yStart() const { return _ystart; }

    /** Get X-spacing */
    CUDA_HOSTDEV double xSpacing() const { return _dx; }

    /** Get Y-spacing */
    CUDA_HOSTDEV double ySpacing() const { return _dy; }

    /** Get LUT length (number of lines) */
    CUDA_HOSTDEV size_t length() const { return _length; }

    /** Get LUT width (number of samples) */
    CUDA_HOSTDEV size_t width() const { return _width; }

    /** Get the reference value */
    CUDA_HOSTDEV T refValue() const { return _refValue; }

    /** Get flag for having data */
    CUDA_HOSTDEV bool haveData() const { return _haveData; }

    /** Get bounds error flag */
    CUDA_HOSTDEV bool boundsError() const { return _boundsError; }

    /** Get interp method */
    CUDA_HOSTDEV
    isce::core::dataInterpMethod interpMethod() const { return _interpMethod; }

    /** Get pointer to interpolator */
    CUDA_HOSTDEV gpuInterpolator<T>** interp() const { return _interp; }

    /** Access to data values */
    CUDA_HOSTDEV T* data() { return _data; }

    /** Read-only access to data values */
    CUDA_HOSTDEV const T* data() const { return _data; }

    /** Set the data values pointer */
    CUDA_HOSTDEV void data(T* v) { _data = v; }

    /**
     * Evaluate the LUT
     *
     * \param[in] y Y-coordinate for evaluation
     * \param[in] x X-coordinate for evaluation
     * \returns     Interpolated value
     */
    CUDA_DEV T eval(double y, double x) const;

    /** Evaluate the LUT from host (test function) */
    T eval_h(double y, double x);

private:
    // Flags
    bool _haveData;
    bool _boundsError;
    T _refValue;
    // Coordinates
    double _xstart, _ystart, _dx, _dy;
    // LUT data
    size_t _length, _width;
    T* _data;
    // Interpolator pointer
    isce::core::dataInterpMethod _interpMethod;
    gpuInterpolator<T>** _interp;
    // Do I own data?
    bool _owner;

    // Interpolation pointer handlers

    /** Initialize interpolation object on device. */
    void _initInterp();

    /** Finalize/delete interpolation object on device. */
    void _finalizeInterp();
};

}}} // namespace isce::cuda::core
