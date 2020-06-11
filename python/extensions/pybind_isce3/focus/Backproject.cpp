#include "Backproject.h"

#include <isce/container/RadarGeometry.h>
#include <isce/core/Kernels.h>
#include <isce/except/Error.h>
#include <isce/focus/Backproject.h>
#include <isce/focus/DryTroposphereModel.h>
#include <isce/geometry/DEMInterpolator.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

using namespace isce::focus;

using isce::container::RadarGeometry;
using isce::core::Kernel;
using isce::except::InvalidArgument;
using isce::geometry::DEMInterpolator;

void addbinding_backproject(py::module& m)
{
    m.def("backproject", [](
                py::array_t<std::complex<float>, py::array::c_style> out,
                const RadarGeometry& out_geometry,
                py::array_t<std::complex<float>, py::array::c_style> in,
                const RadarGeometry& in_geometry,
                const DEMInterpolator& dem,
                double fc,
                double ds,
                const Kernel<float>& kernel,
                const std::string& dry_tropo_model,
                py::dict rdr2geo_params,
                py::dict geo2rdr_params) {

            if (out.ndim() != 2) {
                throw InvalidArgument(ISCE_SRCINFO(), "output array must be 2-D");
            }

            if (out.shape()[0] != out_geometry.gridLength() or
                out.shape()[1] != out_geometry.gridWidth()) {

                std::string errmsg = "output array shape must match output "
                    "radar grid shape";
                throw InvalidArgument(ISCE_SRCINFO(), errmsg);
            }

            if (in.ndim() != 2) {
                throw InvalidArgument(ISCE_SRCINFO(), "input signal data must be 2-D");
            }

            if (in.shape()[0] != in_geometry.gridLength() or
                in.shape()[1] != in_geometry.gridWidth()) {

                std::string errmsg = "input signal data shape must match "
                    "input radar grid shape";
                throw InvalidArgument(ISCE_SRCINFO(), errmsg);
            }

            std::complex<float>* out_data = out.mutable_data();
            const std::complex<float>* in_data = in.data();

            DryTroposphereModel atm = parseDryTropoModel(dry_tropo_model);

            Rdr2GeoParams r2gparams;
            if (rdr2geo_params.contains("threshold")) {
                r2gparams.threshold = py::float_(rdr2geo_params["threshold"]);
            }
            if (rdr2geo_params.contains("maxiter")) {
                r2gparams.maxiter = py::int_(rdr2geo_params["maxiter"]);
            }
            if (rdr2geo_params.contains("extraiter")) {
                r2gparams.extraiter = py::int_(rdr2geo_params["extraiter"]);
            }

            Geo2RdrParams g2rparams;
            if (geo2rdr_params.contains("threshold")) {
                g2rparams.threshold = py::float_(geo2rdr_params["threshold"]);
            }
            if (geo2rdr_params.contains("maxiter")) {
                g2rparams.maxiter = py::int_(geo2rdr_params["maxiter"]);
            }
            if (geo2rdr_params.contains("dr")) {
                g2rparams.delta_range = py::float_(geo2rdr_params["dr"]);
            }

            backproject(out_data, out_geometry, in_data, in_geometry, dem, fc,
                    ds, kernel, atm, r2gparams, g2rparams);
            },
            R"(
                Focus in azimuth via time-domain backprojection.
            )",
            py::arg("out"),
            py::arg("out_geometry"),
            py::arg("in"),
            py::arg("in_geometry"),
            py::arg("dem"),
            py::arg("fc"),
            py::arg("ds"),
            py::arg("kernel"),
            py::arg("dry_tropo_model") = "tsx",
            py::arg("rdr2geo_params") = py::dict(),
            py::arg("geo2rdr_params") = py::dict());
}
