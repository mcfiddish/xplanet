#ifndef KEYWORDS_H
#define KEYWORDS_H

enum keyWords
{
    UNKNOWN = '?',            // for getopt
    ABOVE, ABSOLUTE, ALIGN, ANCIENT, ARC_COLOR, ARC_FILE, ARC_SPACING, AUTO, AZIMUTHAL, 
    BACKGROUND, BASEMAG, BELOW, BODY, BONNE, BUMP_MAP, BUMP_SCALE, BUMP_SHADE,  
    CENTER, CIRCLE, CLOUD_GAMMA, CLOUD_MAP, CLOUD_SSEC, CLOUD_THRESHOLD, COLOR, CONFIG_FILE, 
    DATE, DATE_FORMAT, DAY_MAP, DELIMITER, DRAW_ORBIT, DYNAMIC_ORIGIN,
    ENDOFLINE, EPHEMERIS, EQUAL_AREA, 
    FONT, FONTSIZE, FORK, FOV, 
    GALACTIC, GEOMETRY, GLARE, GNOMONIC, GRID, GRID1, GRID2, GRID_COLOR, GROUND, GRS_LON,
    HEMISPHERE, HIBERNATE,
    ICOSAGNOMONIC, IDLEWAIT, IMAGE, INTERPOLATE_ORIGIN_FILE,
    JDATE, JPL_FILE, 
    LABEL, LABELPOS, LABEL_ALTITUDE, LABEL_BODY, LABEL_STRING, LAMBERT, LANGUAGE, LATITUDE, LATLON, LBR, LEFT, LIGHT_TIME, LOCALTIME, LOGMAGSTEP, LONGITUDE, 
    MAGNIFY, MAJOR, MAKECLOUDMAPS, MAP_BOUNDS, MARKER_BOUNDS, MARKER_COLOR, MARKER_FILE, MARKER_FONT, MARKER_FONTSIZE, MAX_RAD_FOR_LABEL, MIN_RAD_FOR_LABEL, MAX_RAD_FOR_MARKERS, MIN_RAD_FOR_MARKERS, MERCATOR, MOLLWEIDE, MULTIPLE,
    NAME, NIGHT_MAP, NORTH, NUM_TIMES, 
    OPACITY, ORBIT, ORBIT_COLOR, ORIGIN, ORIGINFILE, ORTHOGRAPHIC, OUTLINED, OUTPUT, OUTPUT_MAP_RECT, OUTPUT_START_INDEX, 
    PANGO, PATH, PATH_RELATIVE_TO, PETERS, POLYCONIC, POSITION, POST_COMMAND, PREV_COMMAND, PROJECTION, PROJECTIONPARAMETER, 
    QUALITY, 
    RADIUS, RANDOM, RANDOM_ORIGIN, RANDOM_TARGET, RANGE, RAYLEIGH_EMISSION_WEIGHT, RAYLEIGH_FILE, RAYLEIGH_LIMB_SCALE, RAYLEIGH_SCALE, RECTANGULAR, RIGHT, ROOT, ROTATE, 
    SATELLITE_FILE, SAVE_DESKTOP_FILE, SEARCHDIR, SEPARATION, SHADE, SPACING, SPECULAR_MAP, SPICE_EPHEMERIS, SPICE_FILE, STARFREQ, STARMAP, SYMBOLSIZE, SYSTEM, 
    TARGET, TERRESTRIAL, TEXT_COLOR, THICKNESS, TIMEWARP, TIMEZONE, TMPDIR, TRAIL, TRANSPARENT, TRANSPNG, TSC, TWILIGHT,
    UTCLABEL, 
    VERBOSITY, VERSIONNUMBER, VROOT, 
    WAIT, WINDOW, WINDOWTITLE, 
    XWINID, XYZ, XYZFILE, 
    LAST_WORD
};

const char * const keyWordString[LAST_WORD] =
{
    "UNKNOWN",
    "ABOVE", "ABSOLUTE", "ALIGN", "ANCIENT", "ARC_COLOR", "ARC_FILE", "ARC_SPACING", "AUTO", "AZIMUTHAL", 
    "BACKGROUND", "BASEMAG", "BELOW", "BODY", "BONNE", "BUMP_MAP", "BUMP_SCALE", "BUMP_SHADE",  
    "CENTER", "CIRCLE", "CLOUD_GAMMA", "CLOUD_MAP", "CLOUD_SSEC", "CLOUD_THRESHOLD", "COLOR", "CONFIG_FILE", 
    "DATE", "DATE_FORMAT", "DAY_MAP", "DELIMITER", "DRAW_ORBIT", "DYNAMIC_ORIGIN",
    "ENDOFLINE", "EPHEMERIS", "EQUAL_AREA", 
    "FONT", "FONTSIZE", "FORK", "FOV", 
    "GALACTIC", "GEOMETRY", "GLARE", "GNOMONIC", "GRID", "GRID1", "GRID2", "GRID_COLOR", "GROUND", "GRS_LON", 
    "HEMISPHERE", "HIBERNATE",
    "ICOSAGNOMONIC", "IDLEWAIT", "IMAGE", "INTERPOLATE_ORIGIN_FILE",
    "JDATE", "JPL_FILE", 
    "LABEL", "LABELPOS", "LABEL_ALTITUDE", "LABEL_BODY", "LABEL_STRING", "LAMBERT", "LANGUAGE", "LATITUDE", "LATLON", "LBR", "LEFT", "LIGHT_TIME", "LOCALTIME", "LOGMAGSTEP", "LONGITUDE", 
    "MAGNIFY", "MAJOR", "MAKECLOUDMAPS", "MAP_BOUNDS", "MARKER_BOUNDS", "MARKER_COLOR", "MARKER_FILE", "MARKER_FONT", "MARKER_FONTSIZE", "MAX_RAD_FOR_LABEL", "MIN_RAD_FOR_LABEL", "MAX_RAD_FOR_MARKERS", "MIN_RAD_FOR_MARKERS", "MERCATOR", "MOLLWEIDE", "MULTIPLE",
    "NAME", "NIGHT_MAP", "NORTH", "NUM_TIMES", 
    "OPACITY", "ORBIT", "ORBIT_COLOR", "ORIGIN", "ORIGINFILE", "ORTHOGRAPHIC", "OUTLINED", "OUTPUT", "OUTPUT_MAP_RECT", "OUTPUT_START_INDEX", 
    "PANGO", "PATH", "PATH_RELATIVE_TO", "PETERS", "POLYCONIC", "POSITION", "POST_COMMAND", "PREV_COMMAND", "PROJECTION", "PROJECTIONPARAMETER", 
    "QUALITY", 
    "RADIUS", "RANDOM", "RANDOM_ORIGIN", "RANDOM_TARGET", "RANGE", "RAYLEIGH_EMISSION_WEIGHT", "RAYLEIGH_FILE", "RAYLEIGH_LIMB_SCALE", "RAYLEIGH_SCALE", "RECTANGULAR", "RIGHT", "ROOT", "ROTATE", 
    "SATELLITE_FILE", "SAVE_DESKTOP_FILE", "SEARCHDIR", "SEPARATION", "SHADE", "SPACING", "SPECULAR_MAP", "SPICE_EPHEMERIS", "SPICE_FILE", "STARFREQ", "STARMAP", "SYMBOLSIZE", "SYSTEM", 
    "TARGET", "TERRESTRIAL", "TEXT_COLOR", "THICKNESS", "TIMEWARP", "TIMEZONE", "TMPDIR", "TRAIL", "TRANSPARENT", "TRANSPNG", "TSC", "TWILIGHT",
    "UTCLABEL", 
    "VERBOSITY", "VERSIONNUMBER", "VROOT", 
    "WAIT", "WINDOW", "WINDOWTITLE", 
    "XWINID", "XYZ", "XYZFILE",
    "LAST_WORD"
};

#endif
