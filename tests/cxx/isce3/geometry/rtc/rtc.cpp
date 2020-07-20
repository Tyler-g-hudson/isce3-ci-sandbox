#include <gtest/gtest.h>
#include <isce3/core/Constants.h>
#include <isce3/core/Orbit.h>
#include <isce3/core/Serialization.h>
#include <isce3/geometry/RTC.h>
#include <isce3/geometry/Serialization.h>
#include <isce3/io/IH5.h>
#include <isce3/io/Raster.h>
#include <isce3/product/Product.h>
#include <isce3/product/RadarGridParameters.h>
#include <string>

// Create set of RadarGridParameters to process
std::set<std::string> radar_grid_str_set = {"cropped", "multilooked"};

// Create set of rtcAlgorithms
std::set<isce3::geometry::rtcAlgorithm> rtc_algorithm_set = {
        isce3::geometry::rtcAlgorithm::RTC_DAVID_SMALL,
        isce3::geometry::rtcAlgorithm::RTC_AREA_PROJECTION};

TEST(TestRTC, RunRTC) {
    // Open HDF5 file and load products
    isce3::io::IH5File file(TESTDATA_DIR "envisat.h5");
    isce3::product::Product product(file);
    char frequency = 'A';

    // Open DEM raster
    isce3::io::Raster dem(TESTDATA_DIR "srtm_cropped.tif");

    // Create radar grid parameter
    isce3::product::RadarGridParameters radar_grid_sl(product, frequency);

    // Crop original radar grid parameter
    isce3::product::RadarGridParameters radar_grid_cropped =
            radar_grid_sl.offsetAndResize(30, 135, 128, 128);

    // Multi-look original radar grid parameter
    int nlooks_az = 5, nlooks_rg = 5;
    isce3::product::RadarGridParameters radar_grid_ml =
            radar_grid_sl.multilook(nlooks_az, nlooks_rg);

    // Create orbit and Doppler LUT
    isce3::core::Orbit orbit = product.metadata().orbit();
    isce3::core::LUT2d<double> dop =
            product.metadata().procInfo().dopplerCentroid(frequency);

    dop.boundsError(false);

    // Set input parameters
    isce3::geometry::rtcInputRadiometry inputRadiometry =
            isce3::geometry::rtcInputRadiometry::BETA_NAUGHT;

    isce3::geometry::rtcAreaMode rtc_area_mode =
            isce3::geometry::rtcAreaMode::AREA_FACTOR;

    for (auto radar_grid_str : radar_grid_str_set) {

        isce3::product::RadarGridParameters radar_grid;

        // Open DEM raster
        if (radar_grid_str == "cropped")
            radar_grid = radar_grid_cropped;
        else
            radar_grid = radar_grid_ml;

        for (auto rtc_algorithm : rtc_algorithm_set) {

            double geogrid_upsampling = 1;

            std::string filename;
            // test removed because it requires high geogrid upsampling (too
            // slow)
            if (rtc_algorithm ==
                        isce3::geometry::rtcAlgorithm::RTC_DAVID_SMALL &&
                radar_grid_str == "cropped") {
                continue;
            } else if (rtc_algorithm ==
                       isce3::geometry::rtcAlgorithm::RTC_DAVID_SMALL) {
                filename = "./rtc_david_small_" + radar_grid_str + ".bin";
            } else {
                filename = "./rtc_area_proj_" + radar_grid_str + ".bin";
            }
            std::cout << "generating file: " << filename << std::endl;

            // Create output raster
            isce3::io::Raster out_raster(filename, radar_grid.width(),
                                        radar_grid.length(), 1, GDT_Float32,
                                        "ENVI");

            // Call RTC
            isce3::geometry::facetRTC(radar_grid, orbit, dop, dem, out_raster,
                                     inputRadiometry, rtc_area_mode,
                                     rtc_algorithm, geogrid_upsampling);
        }
    }
}

TEST(TestRTC, CheckResults) {

    for (auto radar_grid_str : radar_grid_str_set) {

        for (auto rtc_algorithm : rtc_algorithm_set) {

            double max_rmse;

            std::string filename;

            // test removed because it requires high geogrid upsampling (too
            // slow)
            if (rtc_algorithm ==
                        isce3::geometry::rtcAlgorithm::RTC_DAVID_SMALL &&
                radar_grid_str == "cropped") {
                continue;
            } else if (rtc_algorithm ==
                       isce3::geometry::rtcAlgorithm::RTC_DAVID_SMALL) {
                max_rmse = 0.7;
                filename = "./rtc_david_small_" + radar_grid_str + ".bin";
            } else {
                max_rmse = 0.1;
                filename = "./rtc_area_proj_" + radar_grid_str + ".bin";
            }

            std::cout << "evaluating file: " << filename << std::endl;

            // Open computed integrated-area raster
            isce3::io::Raster testRaster(filename);

            // Open reference raster
            std::string ref_filename =
                    TESTDATA_DIR "rtc/rtc_" + radar_grid_str + ".bin";
            isce3::io::Raster refRaster(ref_filename);
            std::cout << "reference file: " << ref_filename << std::endl;

            ASSERT_TRUE(testRaster.width() == refRaster.width() and
                        testRaster.length() == refRaster.length());

            double square_sum = 0; // sum of square difference
            int nnan = 0;          // number of NaN pixels
            int nneg = 0;          // number of negative pixels

            // Valarray to hold line of data
            std::valarray<double> test(testRaster.width()),
                    ref(refRaster.width());
            int nvalid = 0;
            for (size_t i = 0; i < refRaster.length(); i++) {
                // Get line of data
                testRaster.getLine(test, i, 1);
                refRaster.getLine(ref, i, 1);
                // Check each value in the line
                for (size_t j = 0; j < refRaster.width(); j++) {
                    if (std::isnan(test[j]) or std::isnan(ref[j])) {
                        nnan++;
                        continue;
                    }
                    if (ref[j] <= 0 or test[j] <= 0) {
                        nneg++;
                        continue;
                    }
                    nvalid++;
                    square_sum += pow(test[j] - ref[j], 2);
                }
            }
            // Compute average over entire image
            double rmse = std::sqrt(square_sum / nvalid);

            printf("    RMSE = %g\n", rmse);
            printf("    nnan = %d\n", nnan);
            printf("    nneg = %d\n", nneg);

            // Enforce bound on average pixel-error
            ASSERT_LT(rmse, max_rmse);

            // Enforce bound on number of ignored pixels
            ASSERT_LT(nnan, 1e-4 * refRaster.width() * refRaster.length());
            ASSERT_LT(nneg, 1e-4 * refRaster.width() * refRaster.length());
        }
    }
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
