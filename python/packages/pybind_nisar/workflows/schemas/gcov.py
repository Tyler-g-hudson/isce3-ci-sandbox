# goofy but portable

schema = """
runconfig:
    name: str()

    groups:
        PGENameGroup:
            PGEName: enum('GCOV_L_PGE')
       
        InputFileGroup:
            # REQUIRED - One NISAR L1B RSLC formatted HDF5 file
            InputFilePath: list(str(), min=1, max=1)
       
        DynamicAncillaryFileGroup:
            # Digital elevation model
            DEMFile: str()
       
        ProductPathGroup:
            # Directory where PGE will place results
            ProductPath: str()
       
            # Directory where SAS can write temporary data
            ScratchPath: str()
       
            # Intermediate file name.  SAS writes output product to the following file.
            # After the SAS completes, the PGE wrapper renames the product file
            # according to proper file naming conventions.
            SASOutputFile: str()
       
        PrimaryExecutable:
            ProductType: enum('GCOV')
       
        DebugLevelGroup:
            DebugSwitch: bool()
       
        Geometry:
            CycleNumber: int(min=1, max=999)
            RelativeOrbitNumber: int(min=1, max=173)
            FrameNumber: int(min=1, max=176)
            OrbitDirection: enum('Descending', 'Ascending')
       
        #adt section - isce3 + pyre workflow
        processing:
            # Mechanism to select frequencies and polarizations
            input_subset:
                # List of frequencies to process
                list_of_frequencies:
                    # valid values for polarizations
                    # empty for all polarizations found in RSLC
                    # [polarizations] for list of specific frequency(s) e.g. [HH, HV] or [HH]
                    A: any(list(str(min=2, max=2), min=1, max=4), str(min=2, max=2), null(), required=False)
                    B: any(list(str(min=2, max=2), min=1, max=4), str(min=2, max=2), null(), required=False)
               
                # Compute cross-elements (True) or diagonals only (False). Default: False
                fullcovariance: bool(required=False)
        
            # Only checked when internet access is available
            dem_download:
                # s3 bucket / curl URL / local file
                source: str(required=False)
                top_left:
                    x: num(required=False)
                    y: num(required=False)
               
                bottom_right:
                    x: num(required=False)
                    y: num(required=False)
        
            # Pre-processing options (before geocoding)
            pre_process:
                # Number of looks in azimuth
                azimuth_looks: int(min=1, required=False)
               
                # Number of looks in range
                range_looks: int(min=1, required=False)
        
            rtc: # RTC options
                output_type: enum(Null, 'gamma0', required=False)
               
                algorithm_type: enum('area_projection', 'david_small', required=False)
               
                input_terrain_radiometry: enum('beta0', 'sigma0', required=False)
               
                # Minimum RTC area factor in dB
                rtc_min_value_db: num(required=False)
        
            # Mechanism to specify output posting and DEM
            geocode:
                algorithm_type: enum('area_projection', 'area_projection_gamma_naught', 'interp', required=False)
               
                memory_mode: enum('auto', 'single_block', 'blocks_geogrid', 'blocks_geogrid_and_radargrid', required=False)
               
                # Processing upsampling factor on top of the input geogrid
                geogrid_upsampling: int(required=False)
               
                # Save the number of looks used to compute GCOV
                save_nlooks: bool(required=False)
               
                # Save the RTC area factor used to compute GCOV
                save_rtc: bool(required=False)
               
                # Absolute radiometric correction factor
                abs_rad_cal: num(required=False)
               
                # Same as input DEM if not provided.
                outputEPSG: int(required=False)
               
                # Output posting in same units as output EPSG. Single value or list indicating the output posting for each frequency
                output_posting:
                    A:
                        x_posting: num(min=0, required=False)
                        y_posting: num(min=0, required=False)
                    B:
                        x_posting: num(min=0, required=False)
                        y_posting: num(min=0, required=False)
               
                # To control output grid in same units as output EPSG
                x_snap: num(min=0, required=False)
               
                # To control output grid in same units as output EPSG
                y_snap: num(min=0, required=False)
               
                top_left:    
                    # Set top-left y in same units as output EPSG
                    y_abs: num(required=False)
                   
                    # Set top-left x in same units as output EPSG
                    x_abs: num(required=False)
               
                bottom_right:  
                    # Set bottom-right y in same units as output EPSG
                    y_abs: num(required=False)
                   
                    # Set bottom-right x in same units as output EPSG
                    x_abs: num(required=False)

            geo2rdr:
                threshold: num(min=1.0e-9, max=1.0e-3, required=False)
                maxiter: int(min=10, max=50, required=False)

            dem_margin: num()
        
            # If noise correction desired (for ISRO)
            noise_correction:
                apply_correction: bool(required=False)
               
                correction_type:  str(required=False)
        
        # To setup type of worker
        worker:
            # To prevent downloading DEM / other data automatically. Default True
            internet_access: bool(required=False)
           
            # To explicitly use GPU capability if available. Default False
            gpu_enabled: bool(required=False)

"""
