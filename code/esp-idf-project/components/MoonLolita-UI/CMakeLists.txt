set(
    UI_SRC_DIRS
    "home_page"
)

idf_component_register(SRC_DIRS ${UI_SRC_DIRS}
                    INCLUDE_DIRS "."
                    REQUIRES lvgl)
