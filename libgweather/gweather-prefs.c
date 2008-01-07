/*
 *  Papadimitriou Spiros <spapadim+@cs.cmu.edu>
 *
 *  This code released under the GNU GPL.
 *  Read the file COPYING for more information.
 *
 *  Preference handling functions.
 *
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef HAVE__NL_MEASUREMENT_MEASUREMENT
#include <langinfo.h>
#endif

#include <glib/gi18n-lib.h>
#include <gconf/gconf-client.h>
#include <libgweather/gweather-prefs.h>

static GConfEnumStringPair temp_unit_enum_map [] = {
    { TEMP_UNIT_DEFAULT,    N_("Default") },
    { TEMP_UNIT_KELVIN,     N_("K")       }, /* translators: Kelvin */
    { TEMP_UNIT_CENTIGRADE, N_("C")       }, /* translators: Celsius */
    { TEMP_UNIT_FAHRENHEIT, N_("F")       }, /* translators: Fahrenheit */
    { 0, NULL }
};	

static GConfEnumStringPair speed_unit_enum_map [] = {
    { SPEED_UNIT_DEFAULT, N_("Default")        },
    { SPEED_UNIT_MS,      N_("m/s")            }, /* translators: meters per second */
    { SPEED_UNIT_KPH,     N_("km/h")           }, /* translators: kilometers per hour */
    { SPEED_UNIT_MPH,     N_("mph")            }, /* translators: miles per hour */
    { SPEED_UNIT_KNOTS,   N_("knots")          }, /* translators: knots (speed unit) */
    { SPEED_UNIT_BFT,     N_("Beaufort scale") }, /* translators: wind speed */
    { 0, NULL }
};

static GConfEnumStringPair pressure_unit_enum_map [] = {
    { PRESSURE_UNIT_DEFAULT, N_("Default") },
    { PRESSURE_UNIT_KPA,     N_("kPa")     }, /* translators: kilopascals */
    { PRESSURE_UNIT_HPA,     N_("hPa")     }, /* translators: hectopascals */
    { PRESSURE_UNIT_MB,      N_("mb")      }, /* translators: millibars */
    { PRESSURE_UNIT_MM_HG,   N_("mmHg")    }, /* translators: millimeters of mercury */
    { PRESSURE_UNIT_INCH_HG, N_("inHg")    }, /* translators: inches of mercury */
    { PRESSURE_UNIT_ATM,     N_("atm")     }, /* translators: atmosphere */
    { 0, NULL }
};

static GConfEnumStringPair distance_unit_enum_map [] = {
    { DISTANCE_UNIT_DEFAULT, N_("Default") },
    { DISTANCE_UNIT_METERS,  N_("m")       }, /* translators: meters */
    { DISTANCE_UNIT_KM,      N_("km")      }, /* translators: kilometers */
    { DISTANCE_UNIT_MILES,   N_("mi")      }, /* translators: miles */
    { 0, NULL }
};


static void parse_temp_string (const gchar *gconf_str, GWeatherPrefs *prefs)
{
    gint value = 0;
#ifdef HAVE__NL_MEASUREMENT_MEASUREMENT
    char *imperial = NULL;
#endif

    prefs->temperature_unit = TEMP_UNIT_INVALID;	
    prefs->use_temperature_default = TRUE;
	
    if ( gconf_str && gconf_string_to_enum (temp_unit_enum_map, gconf_str, &value) ) {
        prefs->temperature_unit = value;
		
		if ((prefs->temperature_unit == TEMP_UNIT_DEFAULT) &&
		    (gconf_string_to_enum (temp_unit_enum_map, _("DEFAULT_TEMP_UNIT"), &value)) ) {
              prefs->temperature_unit = value;
		} else {
            prefs->use_temperature_default = FALSE;
        }
    }	
    else {
        /* TRANSLATOR: This is the default unit to use for temperature measurements. */
        /* Valid values are: "K" (Kelvin), "C" (Celsius) and "F" (Fahrenheit) */
        if (gconf_string_to_enum (temp_unit_enum_map, _("DEFAULT_TEMP_UNIT"), &value) ) {
            prefs->temperature_unit = value;
        }
    }
    if (!prefs->temperature_unit || prefs->temperature_unit == TEMP_UNIT_DEFAULT ) {
#ifdef HAVE__NL_MEASUREMENT_MEASUREMENT
        imperial = nl_langinfo(_NL_MEASUREMENT_MEASUREMENT);
        if ( imperial && imperial[0] == 2 )  {
            /* imperial */
            prefs->temperature_unit = TEMP_UNIT_FAHRENHEIT;
        } else
#endif
            prefs->temperature_unit = TEMP_UNIT_CENTIGRADE;
    }
}

static void parse_speed_string (const gchar *gconf_str, GWeatherPrefs *prefs)
{
    gint value = 0;
#ifdef HAVE__NL_MEASUREMENT_MEASUREMENT
    char *imperial = NULL;
#endif
	
    prefs->speed_unit = SPEED_UNIT_INVALID;
    prefs->use_speed_default = TRUE;
	
    if ( gconf_str && gconf_string_to_enum (speed_unit_enum_map, gconf_str, &value) ) {
        prefs->speed_unit = value;
		if ((prefs->speed_unit == SPEED_UNIT_DEFAULT) &&
			(gconf_string_to_enum (speed_unit_enum_map, _("DEFAULT_SPEED_UNIT"), &value)) ) {
              prefs->speed_unit = value;
		} else {
            prefs->use_speed_default = FALSE;
        }
    }	
    else {
        /* TRANSLATOR: This is the default unit to use for wind speed. */
        /* Valid values are: "m/s" (meters per second), "km/h" (kilometers per hour), */
        /* "mph" (miles per hour) and "knots"  */
        if (gconf_string_to_enum (speed_unit_enum_map, _("DEFAULT_SPEED_UNIT"), &value) ) {
            prefs->speed_unit = value;
        }
    }
    if ( (!prefs->speed_unit) || prefs->speed_unit == SPEED_UNIT_DEFAULT ) {
#ifdef HAVE__NL_MEASUREMENT_MEASUREMENT
        imperial = nl_langinfo(_NL_MEASUREMENT_MEASUREMENT);
        if ( imperial && imperial[0] == 2 )  {
            /* imperial */
            prefs->speed_unit = SPEED_UNIT_KNOTS;
        } else
#endif
            prefs->speed_unit = SPEED_UNIT_MS;
    }
}


static void parse_pressure_string (const gchar *gconf_str, GWeatherPrefs *prefs)
{
    gint value = 0;
#ifdef _NL_MEASUREMENT_MEASUREMENT
    char *imperial = NULL;
#endif
	
    prefs->pressure_unit = PRESSURE_UNIT_INVALID;
    prefs->use_pressure_default = TRUE;
	
    if ( gconf_str && gconf_string_to_enum (pressure_unit_enum_map, gconf_str, &value) ) {
        prefs->pressure_unit = value;

        if ((prefs->pressure_unit == PRESSURE_UNIT_DEFAULT) &&
			(gconf_string_to_enum (pressure_unit_enum_map, _("DEFAULT_PRESSURE_UNIT"), &value)) ) {
              prefs->pressure_unit = value;
		} else {
            prefs->use_pressure_default = FALSE;
        }
    }	
    else {
        /* TRANSLATOR: This is the default unit to use for atmospheric pressure. */
        /* Valid values are: "kPa" (kiloPascals), "hPa" (hectoPascals),
           "mb" (millibars), "mmHg" (millimeters of mercury),
           "inHg" (inches of mercury) and "atm" (atmosphere) */
        if (gconf_string_to_enum (pressure_unit_enum_map, _("DEFAULT_PRESSURE_UNIT"), &value) ) {
            prefs->pressure_unit = value;
        }
    }
    if ( (!prefs->pressure_unit) || prefs->pressure_unit == PRESSURE_UNIT_DEFAULT ) {
#ifdef _NL_MEASUREMENT_MEASUREMENT
        imperial = nl_langinfo(_NL_MEASUREMENT_MEASUREMENT);
        if ( imperial && imperial[0] == 2 )  {
            /* imperial */
            prefs->pressure_unit = PRESSURE_UNIT_INCH_HG;
        } else
#endif
            prefs->pressure_unit = PRESSURE_UNIT_HPA;
    }	
}

static void parse_distance_string (const gchar *gconf_str, GWeatherPrefs *prefs)
{
    gint value = 0;
#ifdef _NL_MEASUREMENT_MEASUREMENT
    char *imperial = NULL;
#endif
	
    prefs->distance_unit = DISTANCE_UNIT_INVALID;
    prefs->use_distance_default = TRUE;
    if ( gconf_str && gconf_string_to_enum (distance_unit_enum_map, gconf_str, &value) ) {
        prefs->distance_unit = value;

		if ((prefs->distance_unit == DISTANCE_UNIT_DEFAULT) &&
			(gconf_string_to_enum (distance_unit_enum_map, _("DEFAULT_DISTANCE_UNIT"), &value)) ) {
              prefs->distance_unit = value;
		} else {
            prefs->use_distance_default = FALSE;
        }
    }	
    else {
        /* TRANSLATOR: This is the default unit to use for visibility distance. */
        /* Valid values are: "m" (meters), "km" (kilometers) and "mi" (miles)   */
        if (gconf_string_to_enum (distance_unit_enum_map, _("DEFAULT_DISTANCE_UNIT"), &value) ) {
            prefs->distance_unit = value;
        }
    }

	if ((!prefs->distance_unit) || prefs->distance_unit == DISTANCE_UNIT_DEFAULT ) {
#ifdef _NL_MEASUREMENT_MEASUREMENT
        imperial = nl_langinfo(_NL_MEASUREMENT_MEASUREMENT);
        if ( imperial && imperial[0] == 2 )  {
            /* imperial */
            prefs->distance_unit = DISTANCE_UNIT_MILES;
        } else
#endif
            prefs->distance_unit = DISTANCE_UNIT_METERS;
    }

    return;
}	

const char *gweather_prefs_temp_enum_to_string (TempUnit temp)
{
	return gconf_enum_to_string(temp_unit_enum_map, temp);
}

const char *gweather_prefs_speed_enum_to_string (SpeedUnit speed)
{
	return gconf_enum_to_string(speed_unit_enum_map, speed);
}

const char *gweather_prefs_pressure_enum_to_string (PressureUnit pressure)
{
	return gconf_enum_to_string(pressure_unit_enum_map, pressure);
}

const char *gweather_prefs_distance_enum_to_string (DistanceUnit distance)
{
	return gconf_enum_to_string(distance_unit_enum_map, distance);
}


void gweather_prefs_load (GWeatherPrefs *prefs, GWeatherGConf *ctx)
{
    GError *error = NULL;
    gchar  *gconf_str = NULL;

    if (prefs->location) {
       weather_location_free(prefs->location);
    }
    prefs->location = gweather_gconf_get_location(ctx);

    /* Assume we use unit defaults */
    prefs->use_temperature_default = TRUE;
    prefs->use_speed_default = TRUE;
    prefs->use_pressure_default = TRUE;
    prefs->use_distance_default = TRUE;
	
    prefs->update_interval = 
    	gweather_gconf_get_int(ctx, "auto_update_interval", &error);
    if (error) {
	g_print ("%s \n", error->message);
	g_error_free (error);
	error = NULL;
    }
    prefs->update_interval = MAX (prefs->update_interval, 60);
    prefs->update_enabled =
    	gweather_gconf_get_bool(ctx, "auto_update", NULL);
    prefs->detailed =
    	gweather_gconf_get_bool(ctx, "enable_detailed_forecast", NULL);
    prefs->radar_enabled =
    	gweather_gconf_get_bool(ctx, "enable_radar_map", NULL);
    prefs->use_custom_radar_url = 
    	gweather_gconf_get_bool(ctx, "use_custom_radar_url", NULL);

    if (prefs->radar) {
        g_free(prefs->radar);
        prefs->radar = NULL;
    }
    prefs->radar = gweather_gconf_get_string (ctx, "radar", NULL);
    
    gconf_str = gweather_gconf_get_string (ctx, GCONF_TEMP_UNIT, NULL);
    parse_temp_string(gconf_str, prefs);
    g_free (gconf_str);
    
    gconf_str = gweather_gconf_get_string (ctx, GCONF_SPEED_UNIT, NULL);
    parse_speed_string(gconf_str, prefs);
    g_free (gconf_str);
			
    gconf_str = gweather_gconf_get_string (ctx, GCONF_PRESSURE_UNIT, NULL);
    parse_pressure_string(gconf_str, prefs);
    g_free (gconf_str);

    gconf_str = gweather_gconf_get_string (ctx, GCONF_DISTANCE_UNIT, NULL);
    parse_distance_string(gconf_str, prefs);
    g_free (gconf_str);
	
    return;
}

TempUnit
gweather_prefs_parse_temperature (const char *str, gboolean *is_default)
{
	GWeatherPrefs prefs;

	parse_temp_string (str, &prefs);
	*is_default = prefs.use_temperature_default;
	return prefs.temperature_unit;
}

SpeedUnit
gweather_prefs_parse_speed (const char *str, gboolean *is_default)
{
	GWeatherPrefs prefs;

	parse_speed_string (str, &prefs);
	*is_default = prefs.use_speed_default;
	return prefs.speed_unit;
}

static const char *
get_translated_unit (int unit, GConfEnumStringPair *pairs, int min_value, int max_value)
{
    g_return_val_if_fail (unit < min_value || unit > max_value, NULL);

    return _(pairs[unit - 1].str); /* minus 1 because enum value 0 is for "invalid" (look at weather.h) */
}

const char *
gweather_prefs_get_temp_display_name (TempUnit temp)
{
    return get_translated_unit (temp, temp_unit_enum_map, TEMP_UNIT_DEFAULT, TEMP_UNIT_FAHRENHEIT);
}

const char *
gweather_prefs_get_speed_display_name (SpeedUnit speed)
{
    return get_translated_unit (speed, speed_unit_enum_map, SPEED_UNIT_DEFAULT, SPEED_UNIT_BFT);
}

const char *
gweather_prefs_get_pressure_display_name (PressureUnit pressure)
{
    return get_translated_unit (pressure, pressure_unit_enum_map, PRESSURE_UNIT_DEFAULT, PRESSURE_UNIT_ATM);
}

const char *
gweather_prefs_get_distance_display_name (DistanceUnit distance)
{
    return get_translated_unit (distance, distance_unit_enum_map, DISTANCE_UNIT_DEFAULT, DISTANCE_UNIT_MILES);
}
