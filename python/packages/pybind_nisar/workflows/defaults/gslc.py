# goofy but portable

runconfig = """
runconfig:
    name: gslc_workflow_default

    groups:
        PGENameGroup:
            PGEName: GSLC_L_PGE

        InputFileGroup:
            # REQUIRED - One NISAR L1B RSLC formatted HDF5 file
            InputFilePath:
            - /home/lyu/data/NISARA_00914/NISARA_00914_19038_005_190620_L090_CX_129_04.h5

        DynamicAncillaryFileGroup:
            # REQUIRED - Use the provided DEM as input
            DEMFile: /home/lyu/data/NISARA_00914/dem.nc

        ProductPathGroup:
            # REQUIRED - Directory where PGE will place results. Irrelevant to SAS.
            ProductPath: /home/lyu/proj/pybind_rslc2gslc

            # REQUIRED - Directory where SAS can write temporary data
            ScratchPath: /home/lyu/proj/pybind_rslc2gslc/temp

            # REQUIRED - SAS writes output product to the following file. PGE may rename.
            # NOTE: For R2 will need to handle mixed-mode case with multiple outputs of RSLC workflow.
            SASOutputFile: /home/lyu/proj/pybind_rslc2gslc/GSLC_utm_sas_pge.h5

        PrimaryExecutable:
            ProductType: GSLC

        DebugLevelGroup:
            DebugSwitch: false

        Geometry:
            CycleNumber: 1
            RelativeOrbitNumber: 1
            FrameNumber: 1
            OrbitDirection: Descending

        # TODO OPTIONAL - To setup type of worker
        worker:
            # OPTIONAL - To prevent downloading DEM / other data automatically. Default True
            internet_access: False

            # OPTIONAL - To explicitly use GPU capability if available. Default False
            gpu_enabled: False

        # ADT section - isce3 + pyre workflow
        processing:
            input_subset:
                # Frequencies and polarisations to be processed
                list_of_frequencies:
                    # keys for frequency A and B are required.
                    # valid options for polarizations
                    # empty for all polarizations found in RSLC
                    # [polarizations] for list of specific frequency(s) e.g. [HH, HV] or [HH]
                    A:
                    B:

            # TODO OPTIONAL - Only checked when internet access is available
            dem_download:
                # OPTIONAL - s3 bucket / curl URL / local file
                source:
                top_left:
                    x:
                    y:
                bottom_right:
                    x:
                    y:

            # OPTIONAL - do we need this step. Should bandpass filter from 40MHz/20MHz be included
            pre_process:
                filter:
                    A:
                        type:
                        parameters:
                    B:
                        type:
                        parameters:

            # OPTIONAL - Mechanism to specify output posting and DEM
            geocode:
                # OPTIONAL - To control output grid in same units as output EPSG
                x_snap:

                # OPTIONAL - To control output grid in same units as output EPSG
                y_snap:

                # OPTIONAL - Same as input DEM if not provided.
                outputEPSG:

                # OPTIONAL - Spacing between pixels, in same units as output EPSG.
                # If no provided, values will match spacing in provided DEM
                output_posting:
                    A:
                        x_posting:
                        y_posting:
                    B:
                        x_posting:
                        y_posting:

                # OPTIONAL - Can control with absolute values or with snap values
                top_left:
                    # OPTIONAL - Set top-left y in same units as output EPSG
                    y_abs: 3993122.0
                    # OPTIONAL - Set top-left x in same units as output EPSG
                    x_abs: 752229.0

                # OPTIONAL - Can control with absolute values or with snap values
                bottom_right:
                    y_abs: 3949557.0
                    x_abs: 780669.0

            geo2rdr:
                threshold: 1.0e-8
                maxiter: 25

            blocksize:
                y: 1000

            dem_margin: 0.1

            flatten: True
"""
