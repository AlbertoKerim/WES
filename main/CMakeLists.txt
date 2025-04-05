idf_component_register(SRCS "app_main.c" 
                    PRIV_REQUIRES spi_flash
                    INCLUDE_DIRS "../components/veml7700" "../components/sht31"
                    REQUIRES driver)
