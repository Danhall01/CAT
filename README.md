If you're using a custom callback function for logging and want to use custom severity levels for the strings then disable the colors in WECL_COLOUR_XXX so that no automatic coloring happens.


## Overall
TODO:
* Rename everything to follow cat instead of wecl
* Change every function and macro to follow new naming convention (vulkan inspired)

## Logging
TODO:
* Staging buffer for pending log messages
    Each staged message will be printed if an error is occured, or if a flush is made. Otherwise it will be overritted in the next stage/region call
