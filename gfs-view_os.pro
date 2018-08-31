TARGET = gfs-view_os
TEMPLATE = app

CONFIG += c++11

QT += core gui widgets
QT  += qml quick quickcontrols2 location positioning network

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += HAVE_ISATTY
DEFINES += USE_JPEG2000 USE_PNG PNG_CONFIGURE_LIBPNG

QMAKE_CXXFLAGS += -Wno-reorder
QMAKE_CXXFLAGS += -Wno-sign-compare
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-write-strings
QMAKE_CXXFLAGS += -Wno-comment
QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CXXFLAGS += -Wno-missing-field-initializers
QMAKE_CXXFLAGS += -Wno-parentheses

INCLUDEPATH += \
src/g2clib-1.6.0 \
src/met-5.2_bugfix \
src/met-5.2_bugfix/src/basic/vx_cal \
src/met-5.2_bugfix/src/basic/vx_config \
src/met-5.2_bugfix/src/basic/vx_log \
src/met-5.2_bugfix/src/basic/vx_math \
src/met-5.2_bugfix/src/basic/vx_util \
src/met-5.2_bugfix/src/libcode/vx_data2d \
src/met-5.2_bugfix/src/libcode/vx_data2d_factory \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib2 \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib \
src/met-5.2_bugfix/src/libcode/vx_grid \
../opencv-master/modules/core/include \
../opencv-master/modules/imgproc/include \
../opencv-master/modules/highgui/include \
../opencv-master/3rdparty/libjpeg \
../opencv-master/3rdparty/libpng \
../opencv-master/3rdparty/libjasper


SOURCES += \
src/met-5.2_bugfix/src/basic/vx_config/config.tab.cc \
src/met-5.2_bugfix/src/basic/vx_config/config_file.cc \
src/met-5.2_bugfix/src/basic/vx_config/config_scanner.cc \
src/met-5.2_bugfix/src/basic/vx_config/threshold.cc \
src/met-5.2_bugfix/src/basic/vx_math/angles.cc \
src/met-5.2_bugfix/src/basic/vx_util/check_endian.cc \
src/met-5.2_bugfix/src/basic/vx_util/comma_string.cc \
src/met-5.2_bugfix/src/basic/vx_log/concat_string.cc \
src/met-5.2_bugfix/src/basic/vx_util/temp_file.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d_factory/data2d_factory.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d_factory/data2d_factory_utils.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib2/data2d_grib2.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib/data2d_grib_utils.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d/data2d_utils.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d/data_class.cc \
src/met-5.2_bugfix/src/basic/vx_util/data_plane.cc \
src/met-5.2_bugfix/src/basic/vx_util/data_plane_util.cc \
src/met-5.2_bugfix/src/basic/vx_cal/date_to_mjd.cc \
src/met-5.2_bugfix/src/basic/vx_config/dictionary.cc \
src/met-5.2_bugfix/src/basic/vx_cal/doyhms_to_unix.cc \
src/met-5.2_bugfix/src/basic/vx_util/file_exists.cc \
src/met-5.2_bugfix/src/basic/vx_util/file_linecount.cc \
src/met-5.2_bugfix/src/basic/vx_util/file_size.cc \
src/met-5.2_bugfix/src/basic/vx_util/filename_suffix.cc \
src/met-5.2_bugfix/src/libcode/vx_grid/find_grid_by_name.cc \
src/met-5.2_bugfix/src/basic/vx_util/fix_float.cc \
src/met-5.2_bugfix/src/basic/vx_config/grdfiletype_to_string.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib/grib_classes.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib/grib_utils.cc \
src/met-5.2_bugfix/src/libcode/vx_grid/grid_base.cc \
src/met-5.2_bugfix/src/basic/vx_log/indent.cc \
src/met-5.2_bugfix/src/basic/vx_util/int_array.cc \
src/met-5.2_bugfix/src/basic/vx_util/interp_mthd.cc \
src/met-5.2_bugfix/src/basic/vx_util/interp_util.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d_factory/is_grib_file.cc \
src/met-5.2_bugfix/src/basic/vx_util/is_number.cc \
src/met-5.2_bugfix/src/libcode/vx_grid/latlon_grid.cc \
src/met-5.2_bugfix/src/libcode/vx_grid/lc_grid.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d/level_info.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d/leveltype_to_string.cc \
src/met-5.2_bugfix/src/basic/vx_cal/mdyhms_to_unix.cc \
src/met-5.2_bugfix/src/libcode/vx_grid/merc_grid.cc \
src/met-5.2_bugfix/src/basic/vx_cal/mjd_to_date.cc \
src/met-5.2_bugfix/src/basic/vx_math/nint.cc \
src/met-5.2_bugfix/src/basic/vx_util/num_array.cc \
src/met-5.2_bugfix/src/basic/vx_math/ptile.cc \
src/met-5.2_bugfix/src/basic/vx_math/pwl.cc \
src/met-5.2_bugfix/src/libcode/vx_grid/st_grid.cc \
src/met-5.2_bugfix/src/basic/vx_log/string_array.cc \
src/met-5.2_bugfix/src/basic/vx_util/string_fxns.cc \
src/met-5.2_bugfix/src/basic/vx_util/substring.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d/table_lookup.cc \
src/met-5.2_bugfix/src/basic/vx_util/thresh_array.cc \
src/met-5.2_bugfix/src/basic/vx_cal/time_array.cc \
src/met-5.2_bugfix/src/basic/vx_cal/time_strings.cc \
src/met-5.2_bugfix/src/basic/vx_util/two_to_one.cc \
src/met-5.2_bugfix/src/basic/vx_cal/unix_to_mdyhms.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d/var_info.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib/var_info_grib.cc \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib2/var_info_grib2.cc \
src/g2clib-1.6.0\comunpack.c \
src/g2clib-1.6.0\drstemplates.c \
src/g2clib-1.6.0\g2_free.c \
src/g2clib-1.6.0\g2_getfld.c \
src/g2clib-1.6.0\g2_info.c \
src/g2clib-1.6.0\g2_miss.c \
src/g2clib-1.6.0\g2_unpack1.c \
src/g2clib-1.6.0\g2_unpack2.c \
src/g2clib-1.6.0\g2_unpack3.c \
src/g2clib-1.6.0\g2_unpack4.c \
src/g2clib-1.6.0\g2_unpack5.c \
src/g2clib-1.6.0\g2_unpack6.c \
src/g2clib-1.6.0\g2_unpack7.c \
src/g2clib-1.6.0\gbits.c \
src/g2clib-1.6.0\gridtemplates.c \
src/g2clib-1.6.0\rdieee.c \
src/g2clib-1.6.0\seekgb.c \
src/g2clib-1.6.0\simunpack.c \
src/g2clib-1.6.0\specunpack.c \
src/g2clib-1.6.0\int_power.c \
src/g2clib-1.6.0\pdstemplates.c \
src/g2clib-1.6.0\dec_jpeg2000.c \
src/g2clib-1.6.0\dec_png.c \
src/g2clib-1.6.0\jpcunpack.c \
src/g2clib-1.6.0\pngunpack.c \
src/color_tables\BlueDarkOrange18.cpp \
src/color_tables\BlueDarkRed18.cpp \
src/color_tables\BlueGreen14.cpp \
src/color_tables\BrownBlue12.cpp \
src/color_tables\Cat12.cpp \
src/color_tables\GreenMagenta16.cpp \
src/color_tables\StepSeq25.cpp \
src/color_tables\amwg.cpp \
src/color_tables\amwg_blueyellowred.cpp \
src/color_tables\cosam.cpp \
src/color_tables\cosam12.cpp \
src/color_tables\hotcold_18lev.cpp \
src/color_tables\hotcolr_19lev.cpp \
src/color_tables\nrl_sirkes.cpp \
src/color_tables\nrl_sirkes_nowhite.cpp \
src/color_tables\perc2_9lev.cpp \
src/color_tables\percent_11lev.cpp \
src/color_tables\posneg_1.cpp \
src/color_tables\posneg_2.cpp \
src/color_tables\prcp_1.cpp \
src/color_tables\prcp_2.cpp \
src/color_tables\prcp_3.cpp \
src/color_tables\precip2_15lev.cpp \
src/color_tables\precip2_17lev.cpp \
src/color_tables\precip3_16lev.cpp \
src/color_tables\precip4_11lev.cpp \
src/color_tables\precip4_diff_19lev.cpp \
src/color_tables\precip_11lev.cpp \
src/color_tables\precip_diff_12lev.cpp \
src/color_tables\precip_diff_1lev.cpp \
src/color_tables\radar.cpp \
src/color_tables\radar_1.cpp \
src/color_tables\rh_19lev.cpp \
src/color_tables\spread_15lev.cpp \
src/color_tables\sunshine_9lev.cpp \
src/color_tables\sunshine_diff_12lev.cpp \
src/color_tables\temp_19lev.cpp \
src/color_tables\temp_diff_18lev.cpp \
src/color_tables\temp_diff_1lev.cpp \
src/color_tables\topo_15lev.cpp \
src/color_tables\wgne15.cpp \
src/color_tables\wind_17lev.cpp \
src/color_tables\mch_default.cpp \
src/codetable_4_230.cpp \
src/level_table_192.cpp \
src/cntrs.cpp \
src/winc.cpp \
src/datefcst.cpp \
src/dlgdatetime.cpp \
src/dlgiface.cpp \
src/dlgstartup.cpp \
src/graphicsvw.cpp \
src/gribini.cpp \
src/img.cpp \
src/infodata.cpp \
src/mgrb2.cpp \
src/sets.cpp \
src/shaperead.cpp \
src/staticstr.cpp \
src/tvdelegate.cpp \
src/main.cpp \
src/ncl_pals.cpp \
src/places.cpp \
src/dbldateui.cpp \
src/fcstui.cpp \
src/regui.cpp \
src/tagtreeui.cpp \
src/infocntrs.cpp \
src/ramppal.cpp

HEADERS += \
src/cntrs.h \
src/winc.h \
src/datefcst.h \
src/defs.h \
src/dlgdatetime.h \
src/dlgiface.h \
src/dlgstartup.h \
src/graphicsvw.h \
src/gribini.h \
src/img.h \
src/infodata.h \
src/mgrb2.h \
src/sets.h \
src/shaperead.h \
src/staticstr.h \
src/tvdelegate.h \
src/cntrs.h \
src/color_tables.h \
src/datefcst.h \
src/defs.h \
src/dlgdatetime.h \
src/dlgiface.h \
src/dlgstartup.h \
src/graphicsvw.h \
src/grib1_ncep_2_7.h \
src/grib2_all.h \
src/gribini.h \
src/gribitem.h \
src/griblatlon.h \
src/hpa2gpt.h \
src/img.h \
src/infodata.h \
src/local_str.h \
src/map_qml.h \
src/mgrb2.h \
src/ne10m_places_sim.h \
src/places.h \
src/sets.h \
src/shaperead.h \
src/staticstr.h \
src/tvdelegate.h \
src/winc.h \
src/dbldateui.h \
src/fcstui.h \
src/regui.h \
src/tagtreeui.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib/data2d_grib.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib/data2d_grib_utils.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib/grib_classes.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib/grib_strings.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib/grib_utils.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib/var_info_grib.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib/vx_data2d_grib.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib2/data2d_grib2.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_grib2/var_info_grib2.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_factory/data2d_factory.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_factory/data2d_factory_utils.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_factory/is_bufr_file.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_factory/is_grib_file.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_factory/is_netcdf_file.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_factory/var_info_factory.h \
src/met-5.2_bugfix/src/libcode/vx_data2d_factory/vx_data2d_factory.h \
src/met-5.2_bugfix/src/libcode/vx_data2d/data2d_utils.h \
src/met-5.2_bugfix/src/libcode/vx_data2d/data_class.h \
src/met-5.2_bugfix/src/libcode/vx_data2d/level_info.h \
src/met-5.2_bugfix/src/libcode/vx_data2d/leveltype_to_string.h \
src/met-5.2_bugfix/src/libcode/vx_data2d/table_lookup.h \
src/met-5.2_bugfix/src/libcode/vx_data2d/var_info.h \
src/met-5.2_bugfix/src/libcode/vx_data2d/vx_data2d.h \
src/met-5.2_bugfix/src/basic/enum_to_string/code.h \
src/met-5.2_bugfix/src/basic/enum_to_string/enum.tab.h \
src/met-5.2_bugfix/src/basic/enum_to_string/info.h \
src/met-5.2_bugfix/src/basic/enum_to_string/scope.h \
src/met-5.2_bugfix/src/basic/vx_cal/time_array.h \
src/met-5.2_bugfix/src/basic/vx_cal/vx_cal.h \
src/met-5.2_bugfix/src/basic/vx_config/config.tab.h \
src/met-5.2_bugfix/src/basic/vx_config/config_constants.h \
src/met-5.2_bugfix/src/basic/vx_config/config_file.h \
src/met-5.2_bugfix/src/basic/vx_config/configobjecttype_to_string.h \
src/met-5.2_bugfix/src/basic/vx_config/config_util.h \
src/met-5.2_bugfix/src/basic/vx_config/data_file_type.h \
src/met-5.2_bugfix/src/basic/vx_config/dictionary.h \
src/met-5.2_bugfix/src/basic/vx_config/grdfiletype_to_string.h \
src/met-5.2_bugfix/src/basic/vx_config/object_types.h \
src/met-5.2_bugfix/src/basic/vx_config/scanner_stuff.h \
src/met-5.2_bugfix/src/basic/vx_config/threshold.h \
src/met-5.2_bugfix/src/basic/vx_config/vx_config.h \
src/met-5.2_bugfix/src/basic/vx_math/affine.h \
src/met-5.2_bugfix/src/basic/vx_math/angles.h \
src/met-5.2_bugfix/src/basic/vx_math/hist.h \
src/met-5.2_bugfix/src/basic/vx_math/is_bad_data.h \
src/met-5.2_bugfix/src/basic/vx_math/math_constants.h \
src/met-5.2_bugfix/src/basic/vx_math/nint.h \
src/met-5.2_bugfix/src/basic/vx_math/nti.h \
src/met-5.2_bugfix/src/basic/vx_math/ptile.h \
src/met-5.2_bugfix/src/basic/vx_math/pwl.h \
src/met-5.2_bugfix/src/basic/vx_math/trig.h \
src/met-5.2_bugfix/src/basic/vx_math/viewgravity_to_string.h \
src/met-5.2_bugfix/src/basic/vx_math/vx_math.h \
src/met-5.2_bugfix/src/basic/vx_util/ascii_table.h \
src/met-5.2_bugfix/src/basic/vx_util/asciitablejust_to_string.h \
src/met-5.2_bugfix/src/basic/vx_util/bool_to_string.h \
src/met-5.2_bugfix/src/basic/vx_util/check_endian.h \
src/met-5.2_bugfix/src/basic/vx_util/command_line.h \
src/met-5.2_bugfix/src/basic/vx_util/comma_string.h \
src/met-5.2_bugfix/src/basic/vx_util/conversions.h \
src/met-5.2_bugfix/src/basic/vx_util/data_line.h \
src/met-5.2_bugfix/src/basic/vx_util/data_plane.h \
src/met-5.2_bugfix/src/basic/vx_util/data_plane_util.h \
src/met-5.2_bugfix/src/basic/vx_util/empty_string.h \
src/met-5.2_bugfix/src/basic/vx_util/file_exists.h \
src/met-5.2_bugfix/src/basic/vx_util/file_linecount.h \
src/met-5.2_bugfix/src/basic/vx_util/filename_suffix.h \
src/met-5.2_bugfix/src/basic/vx_util/file_size.h \
src/met-5.2_bugfix/src/basic/vx_util/fix_float.h \
src/met-5.2_bugfix/src/basic/vx_util/get_filenames.h \
src/met-5.2_bugfix/src/basic/vx_util/grib_constants.h \
src/met-5.2_bugfix/src/basic/vx_util/int_array.h \
src/met-5.2_bugfix/src/basic/vx_util/interp_mthd.h \
src/met-5.2_bugfix/src/basic/vx_util/interp_util.h \
src/met-5.2_bugfix/src/basic/vx_util/is_number.h \
src/met-5.2_bugfix/src/basic/vx_util/long_array.h \
src/met-5.2_bugfix/src/basic/vx_util/make_path.h \
src/met-5.2_bugfix/src/basic/vx_util/memory.h \
src/met-5.2_bugfix/src/basic/vx_util/met_buffer.h \
src/met-5.2_bugfix/src/basic/vx_util/num_array.h \
src/met-5.2_bugfix/src/basic/vx_util/ordinal.h \
src/met-5.2_bugfix/src/basic/vx_util/polyline.h \
src/met-5.2_bugfix/src/basic/vx_util/read_fortran_binary.h \
src/met-5.2_bugfix/src/basic/vx_util/roman_numeral.h \
src/met-5.2_bugfix/src/basic/vx_util/string_fxns.h \
src/met-5.2_bugfix/src/basic/vx_util/substring.h \
src/met-5.2_bugfix/src/basic/vx_util/temp_file.h \
src/met-5.2_bugfix/src/basic/vx_util/thresh_array.h \
src/met-5.2_bugfix/src/basic/vx_util/two_to_one.h \
src/met-5.2_bugfix/src/basic/vx_util/util_constants.h \
src/met-5.2_bugfix/src/basic/vx_util/vx_util.h 

LIBS += \
-L/usr/local/lib \
-L${PWD}/opencv-master/bin/3rdparty/lib \
-llibjasper \
-lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
-ljpeg \
-lpng \
-lshp \
-rdynamic

RESOURCES += \
    src/Resource.qrc
