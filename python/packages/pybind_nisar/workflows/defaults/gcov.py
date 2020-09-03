# goofy but portable

runconfig = """
runconfig:
    name: gcov_workflow_default

    groups:
        PGENameGroup:
            PGEName: GCOV_L_PGE

        InputFileGroup:
            # REQUIRED - One NISAR L1B RSLC formatted HDF5 file
            InputFilePath:
            - /home/lyu/proj/pybind_rslc2gcov/SanAnd_05024_18038_006_180730_L090_CX_138_03.h5

        DynamicAncillaryFileGroup:
            # REQUIRED - Use the provided DEM as input
            DEMFile: /home/lyu/proj/pybind_rslc2gcov/SanAnd_05024.dem

        ProductPathGroup:
            # Directory where PGE will place results. Irrelevant to SAS.
            ProductPath: /home/lyu/proj/pybind_rslc2gcov

            # Directory where SAS can write temporary data
            ScratchPath: /home/lyu/proj/pybind_rslc2gcov/temp

            # SAS writes output product to the following file. PGE may rename.
            # NOTE: For R2 will need to handle mixed-mode case with multiple outputs of RSLC workflow.
            SASOutputFile: /home/lyu/proj/pybind_rslc2gcov/GCOV_utm_sas_pge.h5

        PrimaryExecutable:
            ProductType: GCOV

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

        #adt section - isce3 + pyre workflow
        processing:
            # Frequencies and polarisations to be processed
            input_subset:
                # List of frequencies to process
                list_of_frequencies:
                    # valid values for polarizations
                    # empty for all polarizations found in RSLC
                    # [polarizations] for list of specific frequency(s) e.g. [HH, HV] or [HH]
                    A:
                    B:

                # OPTIONAL - If we want full covariance instead of diagonals only. Default False
                fullcovariance:   False

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

            # OPTIONAL - if amplitude data needs to be mulitlooked before GCOV generation
            pre_process:   
               azimuth_looks: 1
               range_looks:   1

            # OPTIONAL - to control behavior of RTC module
            rtc:
                # OPTIONAL - Choices:
                # "david_small" (default)
                # "area_projection"
                algorithm_type: area_projection

                # OPTIONAL - Choices:
                # "beta0" (default)
                # "sigma0"
                input_terrain_radiometry: sigma0

                # OPTIONAL - Minimum RTC area factor in dB
                rtc_min_value_db: -30

            # OPTIONAL - Mechanism to specify output posting and DEM
            geocode:
                # OPTIONAL -
                algorithm_type: area_projection
 
                # OPTIONAL - Choices: "single_block", "geogrid", "geogrid_radargrid", and "auto" (default)
                memory_mode:

                # OPTIONAL - Processing upsampling factor applied to input geogrid
                geogrid_upsampling: 1

                # OPTIONAL - absolute radiometric correction
                abs_rad_cal: 1

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

                # OPTIONAL - To control output grid in same units as output EPSG
                y_snap:

                # OPTIONAL - To control output grid in same units as output EPSG
                x_snap:

                # OPTIONAL - Can control with absolute values or with snap values
                top_left:        
                    # OPTIONAL - Set top-left y in same units as output EPSG
                    y_abs:
                    # OPTIONAL - Set top-left x in same units as output EPSG
                    x_abs:

                # OPTIONAL - Can control with absolute values or with snap values
                bottom_right:
                    y_abs:
                    x_abs:

            geo2rdr:
                threshold: 1.0e-8
                maxiter: 25

            dem_margin: 0.1

            # OPTIONAL - if noise correction desired (for ISRO)
            noise_correction:
                # OPTIONAL -
                apply_correction: False

                # OPTIONAL -
                correction_type:
"""
