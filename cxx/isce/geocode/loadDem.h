#pragma once
//#include <isce/core/forward.h>
#include <isce/geometry/DEMInterpolator.h>
#include <isce/io/Raster.h>
#include <isce/product/GeoGridParameters.h>

namespace isce { namespace geocode {

/** returns a DEM interpolator for a block of geocoded grid
 * Note the geocoded grid and the inpit raster of the DEM can be in
 * different or same projection systems
 * @param[in] demRaster a raster of the DEM
 * @param[in] geoGrid  parameters of the geocoded grid
 * @param[in] lineStart start line of the block of interest in the eocoded grid
 * @param[in] blockLength length of the block of interest in the eocoded grid
 * @param[in] blockWidth  width of the block of interest in the eocoded grid
 * @param[in] demMargin  extra margin for the dem relative to the geocoded grid
 */
isce::geometry::DEMInterpolator
loadDEM(isce::io::Raster& demRaster,
        const isce::product::GeoGridParameters& geoGrid, int lineStart,
        int blockLength, int blockWidth, double demMargin);
}} // namespace isce::geocode