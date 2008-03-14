
#ifndef _SQLPILOT_H_
#define _SQLPILOT_H_

#define UI_XML_FILE  "data/ui/sqlpilot.xml"
#define DB_FILE      "db/logbook.db"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "db/db.h"
#include "util.h"
#include "store.h"

#define EXIT_SUCESS 0
#define EXIT_BARF  1

#define EMPTY_IF_NULL(x) (x ? x : "")

#define BUF_DATE 11		/* "YYYY-MM-DD\0" */
#define BUF_TIME 6		/* "HH:MM\0" expecting no flight longer than 99 hours */
#define BUF_DATETIME 17		/* "YYYY-MM-DD HH:MM\0" */
#define BUF_TZ 256		/*  How ever big timezone names get??? */

typedef enum {
    SQLPILOT_NO_ERROR = 0,
    SQLPILOT_ERROR_INVALID_URI,
    SQLPILOT_ERROR_SAVE_FAILED,
    SQLPILOT_ERROR_OPEN_FAILED
} SqlpilotError;

enum {
  NOTEBOOK_PAGE_FLIGHTS = 0,
  NOTEBOOK_PAGE_ROLES,
  NOTEBOOK_PAGE_AIRCRAFT,
  NOTEBOOK_PAGE_TYPES,
  NOTEBOOK_PAGE_AIRPORTS
};

/* Column numbers for treemodel and select statements */
enum {
  FLIGHTS_COL_ID = 0,
  FLIGHTS_COL_DATE,
  FLIGHTS_COL_AIRCRAFT,
  FLIGHTS_COL_ROLE,
  FLIGHTS_COL_DEP,
  FLIGHTS_COL_ARR,
  FLIGHTS_COL_AOUT,
  FLIGHTS_COL_AIN,
  FLIGHTS_COL_DUR,
  FLIGHTS_COL_NIGHT,
  FLIGHTS_COL_INST,
  FLIGHTS_COL_SIMINST,
  FLIGHTS_COL_HOLD,
  FLIGHTS_COL_APRCH,
  FLIGHTS_COL_XC,
  FLIGHTS_COL_DLAND,
  FLIGHTS_COL_NLAND,
  FLIGHTS_COL_CREW,
  FLIGHTS_COL_NOTES,
  FLIGHTS_COL_FLTNO,
  FLIGHTS_COL_SOUT,
  FLIGHTS_COL_SIN,
  FLIGHTS_COL_SDUR,
  FLIGHTS_COL_TRIP,
  FLIGHTS_NUMCOL
};

#define FLIGHTS_SELECT					\
  "select flights.id as _id"				\
  ", flights.date as Date"				\
  ", a.ident as Aircraft"				\
  ", r.ident as Role"					\
  ", dep.ident as Dep"					\
  ", arr.ident as Arr"					\
  ", flights.aout as AOut"				\
  ", flights.ain as AIn"				\
  ", m_to_hhmm(flights.dur) as Dur"			\
  ", m_to_hhmm(flights.night) as Night"			\
  ", m_to_hhmm(flights.inst) as Inst"			\
  ", m_to_hhmm(flights.siminst) as SimInst"		\
  ", bool(flights.hold) as Hold"			\
  ", flights.aprch as Aprch"				\
  ", bool(flights.xc) as XC"				\
  ", flights.dland as DLand"				\
  ", flights.nland as NLand"				\
  ", flights.crew as _Crew"				\
  ", flights.notes as _Notes"				\
  ", flights.fltno as FltNo"				\
  ", flights.sout as SOut"				\
  ", flights.sin as SIn"				\
  ", m_to_hhmm(flights.sdur) as SDur"			\
  ", flights.trip as Trip"				\
  " from flights"					\
  " left join aircraft a on flights.aircraft_id = a.id"	\
  " left join roles r on flights.role_id = r.id"	\
  " left join airports dep on flights.dep_id = dep.id"	\
  " left join airports arr on flights.arr_id = arr.id"

#define FLIGHTS_WHERE_ID \
  " where flights.id = ?;"

/* Insert and update bindings */
enum {
  FLIGHTS_WRITE_AIRCRAFT = 1,
  FLIGHTS_WRITE_ROLE,
  FLIGHTS_WRITE_DEP,
  FLIGHTS_WRITE_ARR,
  FLIGHTS_WRITE_DATE,
  FLIGHTS_WRITE_AOUT,
  FLIGHTS_WRITE_AIN,
  FLIGHTS_WRITE_DUR,
  FLIGHTS_WRITE_NIGHT,
  FLIGHTS_WRITE_INST,
  FLIGHTS_WRITE_SIMINST,
  FLIGHTS_WRITE_HOLD,
  FLIGHTS_WRITE_APRCH,
  FLIGHTS_WRITE_XC,
  FLIGHTS_WRITE_DLAND,
  FLIGHTS_WRITE_NLAND,
  FLIGHTS_WRITE_CREW,
  FLIGHTS_WRITE_NOTES,
  FLIGHTS_WRITE_FLTNO,
  FLIGHTS_WRITE_SOUT,
  FLIGHTS_WRITE_SIN,
  FLIGHTS_WRITE_SDUR,
  FLIGHTS_WRITE_TRIP,
  FLIGHTS_WRITE_ID
};


#define FLIGHTS_INSERT							\
  "insert into flights (aircraft_id, role_id, dep_id, arr_id, date, aout, ain, dur, night, " \
  "inst, siminst, hold, aprch, xc, dland, nland, crew, notes, fltno, sout, sin, sdur, trip) " \
  "values (?, ?, ?, ?, ?, ?, ?, hhmm_to_m(?), hhmm_to_m(?), hhmm_to_m(?), hhmm_to_m(?), ?, ?, ?, " \
  "?, ?, ?, ?, ?, ?, ?, hhmm_to_m(?), ?);"

#define FLIGHTS_UPDATE \
  "update flights set"				\
  " aircraft_id = ?"				\
  ", role_id = ?"				\
  ", dep_id = ?"				\
  ", arr_id = ?"				\
  ", date = ?"					\
  ", aout = ?"					\
  ", ain = ?"					\
  ", dur = hhmm_to_m(?)"			\
  ", night = hhmm_to_m(?)"			\
  ", inst = hhmm_to_m(?)"			\
  ", siminst = hhmm_to_m(?)"			\
  ", hold = ?"					\
  ", aprch = ?"					\
  ", xc = ?"					\
  ", dland = ?"					\
  ", nland = ?"					\
  ", crew = ?"					\
  ", notes = ?"					\
  ", fltno = ?"					\
  ", sout = ?"					\
  ", sin = ?"					\
  ", sdur = hhmm_to_m(?)"			\
  ", trip = ?"					\
  " where id = ?;"

#define FLIGHTS_DELETE				\
  "delete from flights where id = ?;"

enum {
  ROLES_COL_ID = 0,
  ROLES_COL_IDENT,
  ROLES_COL_NAME,
  ROLES_COL_PIC,
  ROLES_COL_SIC,
  ROLES_COL_FE,
  ROLES_COL_SOLO,
  ROLES_COL_DUAL,
  ROLES_COL_INSTRUCT,
  ROLES_COL_FLIGHTS,
  ROLES_COL_TIME
};

#define ROLES_SELECT					\
  "select roles.id as _id"				\
  ", roles.ident as Ident"				\
  ", roles.name as Name"				\
  ", roles.pic as PIC"					\
  ", roles.sic as SIC"					\
  ", roles.fe as FE"					\
  ", roles.solo as Solo"				\
  ", roles.dual as Dual"				\
  ", roles.instruct as Instruct"			\
  ", count(flights.id) as Flights"			\
  ", m_to_hhmm(sum(flights.dur)) as Time"		\
  " from roles"						\
  " left join flights on flights.role_id = roles.id"	\

#define ROLES_GROUP_BY \
  " group by roles.id"

#define ROLES_WHERE_ID \
  " where roles.id = ?"

enum {
  ROLES_WRITE_IDENT = 1,
  ROLES_WRITE_NAME,
  ROLES_WRITE_PIC,
  ROLES_WRITE_SIC,
  ROLES_WRITE_FE,
  ROLES_WRITE_SOLO,
  ROLES_WRITE_DUAL,
  ROLES_WRITE_INSTRUCT,
  ROLES_WRITE_ID
};

#define ROLES_INSERT \
  "insert into roles (ident, name, pic, sic, fe, solo, dual, instruct) values (?, ?, ?, ?, ?, ?, ?, ?);"

#define ROLES_UPDATE \
  "update roles set ident = ?, name = ?, pic = ?, sic = ?, fe = ?, solo = ?, dual = ?, instruct = ?" \
  " where id = ?;"

#define ROLES_DELETE \
  "delete from roles where id = ?;"

enum {
  AIRCRAFT_COL_ID = 0,
  AIRCRAFT_COL_IDENT,
  AIRCRAFT_COL_TYPE,
  AIRCRAFT_COL_FLEETNO,
  AIRCRAFT_COL_NOTES,
  AIRCRAFT_COL_FLIGHTS,
  AIRCRAFT_COL_TIME
};

#define AIRCRAFT_SELECT						\
  "select aircraft.id as _id"					\
  ", aircraft.ident as Ident"					\
  ", types.ident as Type"					\
  ", aircraft.fleetno as FleetNo"				\
  ", aircraft.notes as _Notes"					\
  ", count(flights.id) as Flights"				\
  ", m_to_hhmm(sum(flights.dur)) as Time"			\
  " from aircraft"						\
  " left join types on aircraft.type_id = types.id"		\
  " left join flights on flights.aircraft_id = aircraft.id"	\

#define AIRCRAFT_GROUP_BY \
  " group by aircraft.id"

#define AIRCRAFT_WHERE_ID \
  " where aircraft.id = ?"

enum {
  AIRCRAFT_WRITE_IDENT = 1,
  AIRCRAFT_WRITE_TYPE,
  AIRCRAFT_WRITE_FLEETNO,
  AIRCRAFT_WRITE_NOTES,
  AIRCRAFT_WRITE_ID
};

#define AIRCRAFT_INSERT \
  "insert into aircraft (ident, type_id, fleetno, notes) values (?, ?, ?, ?);"

#define AIRCRAFT_UPDATE \
  "update aircraft set ident = ?, type_id = ?, fleetno = ?, notes = ? where id = ?;"

#define AIRCRAFT_DELETE	\
  "delete from aircraft where id = ?;"

enum {
  TYPES_COL_ID = 0,
  TYPES_COL_IDENT,
  TYPES_COL_MAKE,
  TYPES_COL_MODEL,
  TYPES_COL_AIRPLANE,
  TYPES_COL_ROTORCRAFT,
  TYPES_COL_GLIDER,
  TYPES_COL_LTA,
  TYPES_COL_POWEREDLIFT,
  TYPES_COL_PPC,
  TYPES_COL_WEIGHTSHIFT,
  TYPES_COL_HELI,
  TYPES_COL_GYRO,
  TYPES_COL_AIRSHIP,
  TYPES_COL_BALLOON,
  TYPES_COL_SINGLE,
  TYPES_COL_MULTI,
  TYPES_COL_LAND,
  TYPES_COL_SEA,
  TYPES_COL_TURBINE,
  TYPES_COL_JET,
  TYPES_COL_HIGHPERF,
  TYPES_COL_RETRACT,
  TYPES_COL_COMPLEX,
  TYPES_COL_PRESSURIZED,
  TYPES_COL_LARGE,
  TYPES_COL_SPORT,
  TYPES_COL_ULTRULIGHT,
  TYPES_COL_FOOTLAUNCH,
  TYPES_COL_SIM,
  TYPES_COL_FTD,
  TYPES_COL_TOTAL,
};

#define TYPES_SELECT						\
  "select types.id as _id"					\
  ", types.ident as Ident"					\
  ", types.make as Make"					\
  ", types.model as Model"					\
  ", types.airplane as _Airplane"				\
  ", types.rotorcraft as _Rotorcraft"				\
  ", types.glider as _Glider"					\
  ", types.lta as _LTA"						\
  ", types.poweredlift as _Poweredlift"				\
  ", types.ppc as _PPC"						\
  ", types.weightshift as _Weightshift"				\
  ", types.heli as _Heli"					\
  ", types.gyro as _Gyro"					\
  ", types.airship as _Airship"					\
  ", types.balloon as _Balloon"					\
  ", types.single as _Single"					\
  ", types.multi as _Multi"					\
  ", types.land as _Land"					\
  ", types.sea as _Sea"						\
  ", types.turbine as _Turbine"					\
  ", types.jet as _Jet"						\
  ", types.highperf as _HighPerf"				\
  ", types.retract as _Retract"					\
  ", types.complex as _Complex"					\
  ", types.pressurized as _Pressurized"				\
  ", types.large as _Large"					\
  ", types.sport as _Sport"					\
  ", types.ultralight as _Ultralight"				\
  ", types.footlaunch as _Footlaunch"				\
  ", types.sim as _Sim"						\
  ", types.ftd as _FTD"						\
  ", types.total as _Total"					\
  ", count(distinct(aircraft.id)) as Aircraft"			\
  ", count(flights.id) as Flights"				\
  ", m_to_hhmm(sum(flights.dur)) as Time"			\
  " from types"							\
  " left join aircraft on aircraft.type_id = types.id"		\
  " left join flights on flights.aircraft_id = aircraft.id"

#define TYPES_GROUP_BY \
  " group by types.id"

#define TYPES_WHERE_ID \
  " where types.id = ?"

enum {
  TYPES_WRITE_IDENT = 1,
  TYPES_WRITE_MAKE,
  TYPES_WRITE_MODEL,
  TYPES_WRITE_AIRPLANE,
  TYPES_WRITE_ROTORCRAFT,
  TYPES_WRITE_GLIDER,
  TYPES_WRITE_LTA,
  TYPES_WRITE_POWEREDLIFT,
  TYPES_WRITE_PPC,
  TYPES_WRITE_WEIGHTSHIFT,
  TYPES_WRITE_HELI,
  TYPES_WRITE_GYRO,
  TYPES_WRITE_AIRSHIP,
  TYPES_WRITE_BALLOON,
  TYPES_WRITE_SINGLE,
  TYPES_WRITE_MULTI,
  TYPES_WRITE_LAND,
  TYPES_WRITE_SEA,
  TYPES_WRITE_TURBINE,
  TYPES_WRITE_JET,
  TYPES_WRITE_HIGHPERF,
  TYPES_WRITE_RETRACT,
  TYPES_WRITE_COMPLEX,
  TYPES_WRITE_PRESSURIZED,
  TYPES_WRITE_LARGE,
  TYPES_WRITE_SPORT,
  TYPES_WRITE_ULTRULIGHT,
  TYPES_WRITE_FOOTLAUNCH,
  TYPES_WRITE_SIM,
  TYPES_WRITE_FTD,
  TYPES_WRITE_TOTAL,
  TYPES_WRITE_ID
};

#define TYPES_INSERT				\
  "insert into types "				\
  "( ident"					\
  ", make"					\
  ", model"					\
  ", airplane"					\
  ", rotorcraft"				\
  ", glider"					\
  ", lta"					\
  ", poweredlift"				\
  ", ppc"					\
  ", weightshift"				\
  ", heli"					\
  ", gyro"					\
  ", airship"					\
  ", balloon"					\
  ", single"					\
  ", multi"					\
  ", land"					\
  ", sea"					\
  ", turbine"					\
  ", jet"					\
  ", highperf"					\
  ", retract"					\
  ", complex"					\
  ", pressurized"				\
  ", large"					\
  ", sport"					\
  ", ultralight"				\
  ", footlaunch"				\
  ", sim"					\
  ", ftd"					\
  ", total)"					\
  " values ("					\
  "  ?, ?, ?, ?, ?, ?, ?, ?, ?, ?"		\
  ", ?, ?, ?, ?, ?, ?, ?, ?, ?, ?"		\
  ", ?, ?, ?, ?, ?, ?, ?, ?, ?, ?"		\
  ", ?"					\
  " );"

#define TYPES_UPDATE	  \
  "update types set"	  \
  " ident = ?"		  \
  ", make = ?"		  \
  ", model = ?"		  \
  ", airplane = ?"	  \
  ", rotorcraft = ?"	  \
  ", glider = ?"	  \
  ", lta = ?"		  \
  ", poweredlift = ?"	  \
  ", ppc = ?"		  \
  ", weightshift = ?"	  \
  ", heli = ?"		  \
  ", gyro = ?"		  \
  ", airship = ?"	  \
  ", balloon = ?"	  \
  ", single = ?"	  \
  ", multi = ?"		  \
  ", land = ?"		  \
  ", sea = ?"		  \
  ", turbine = ?"	  \
  ", jet = ?"		  \
  ", highperf = ?"	  \
  ", retract = ?"	  \
  ", complex = ?"	  \
  ", pressurized = ?"	  \
  ", large = ?"		  \
  ", sport = ?"		  \
  ", ultralight = ?"	  \
  ", footlaunch = ?"	  \
  ", sim = ?"		  \
  ", ftd = ?"		  \
  ", total = ?"		  \
  "where id = ?;"


#define TYPES_DELETE \
  "delete from types where id = ?;"

enum {
  AIRPORTS_COL_ID = 0,
  AIRPORTS_COL_IDENT,
  AIRPORTS_COL_ICAO,
  AIRPORTS_COL_NAME,
  AIRPORTS_COL_TZONE,
  AIRPORTS_COL_NOTES,
  AIRPORTS_COL_DEP,
  AIRPORTS_COL_ARR
};

#define AIRPORTS_SELECT						\
  "select airports.id as _id"					\
  ", airports.ident as Ident"					\
  ", airports.icao as ICAO"					\
  ", airports.name as Name"					\
  ", airports.tzone as TZone"					\
  ", airports.notes as _Notes"					\
  ", count(distinct(fdep.id)) as Dep"				\
  ", count(distinct(farr.id)) as Arr"				\
  " from airports"						\
  " left join flights fdep on fdep.dep_id = airports.id"	\
  " left join flights farr on farr.arr_id = airports.id"

#define AIRPORTS_GROUP_BY \
  " group by airports.id"

#define AIRPORTS_WHERE_ID \
  " where airports.id = ?"

enum {
  AIRPORTS_WRITE_IDENT = 1,
  AIRPORTS_WRITE_ICAO,
  AIRPORTS_WRITE_NAME,
  AIRPORTS_WRITE_TZONE,
  AIRPORTS_WRITE_NOTES,
  AIRPORTS_WRITE_ID
};


#define AIRPORTS_INSERT \
  "insert into airports (ident, icao, name, tzone, notes) values (?, ?, ?, ?, ?);"

#define AIRPORTS_UPDATE \
  "update airports set ident = ?, icao = ?, name = ?, tzone = ?, notes = ? where id = ?;"

#define AIRPORTS_DELETE \
  "delete from airports where id = ?;"
  

typedef struct Sqlpilot Sqlpilot;
struct Sqlpilot {
  DB *db;
  GtkWidget *window;

  int flights_stale;		/* Model contains stale data as a result of updates to db elsewhere ... the model for the view is stale, not the db */
  int flights_modified;		/* Entries have been modified but not committed to db */
  DBStatement *flights_select_all; /* Select for tree model */
  DBStatement *flights_select_by_id; /* Select for update a single treemodel row */
  DBStatement *flights_insert;
  DBStatement *flights_update;
  DBStatement *flights_delete;
  GtkWidget *flights_sw;	/* Scrollable Window - contains the treeview */
  GtkTreeModel *flights_treemodel;
  GtkTreeSelection *flights_selection;
  GtkWidget *flights_treeview;
  GtkWidget *flights_aircraft;
  GtkWidget *flights_date;
  GtkWidget *flights_role;
  GtkWidget *flights_dep;
  GtkWidget *flights_arr;
  GtkWidget *flights_aout;
  GtkWidget *flights_ain;
  GtkWidget *flights_dur;
  GtkWidget *flights_night;
  GtkWidget *flights_inst;
  GtkWidget *flights_siminst;
  GtkWidget *flights_hold;
  GtkWidget *flights_aprch;
  GtkWidget *flights_xc;
  GtkWidget *flights_dland;
  GtkWidget *flights_nland;
  GtkWidget *flights_crew;
  GtkWidget *flights_notes;
  GtkWidget *flights_fltno;
  GtkWidget *flights_sout;
  GtkWidget *flights_sin;
  GtkWidget *flights_sdur;
  GtkWidget *flights_trip;

  int roles_stale;
  int roles_modified;
  DBStatement *roles_select_all;
  DBStatement *roles_select_by_id;
  DBStatement *roles_insert;
  DBStatement *roles_update;
  DBStatement *roles_delete;
  GtkWidget *roles_sw;
  GtkTreeModel *roles_treemodel;
  GtkTreeSelection *roles_selection;
  GtkWidget *roles_treeview;
  GtkWidget *roles_ident;
  GtkWidget *roles_name;
  GtkWidget *roles_pic;
  GtkWidget *roles_sic;
  GtkWidget *roles_fe;
  GtkWidget *roles_solo;
  GtkWidget *roles_dual;
  GtkWidget *roles_instruct;  

  int aircraft_stale;
  int aircraft_modified;
  DBStatement *aircraft_select_all;
  DBStatement *aircraft_select_by_id;
  DBStatement *aircraft_insert;
  DBStatement *aircraft_update;
  DBStatement *aircraft_delete;
  GtkWidget *aircraft_sw;
  GtkTreeModel *aircraft_treemodel;
  GtkTreeSelection *aircraft_selection;
  GtkWidget *aircraft_treeview;
  GtkWidget *aircraft_ident;
  GtkWidget *aircraft_type;
  GtkWidget *aircraft_fleetno;
  GtkWidget *aircraft_notes;

  int types_stale;
  int types_modified;
  DBStatement *types_select_all;
  DBStatement *types_select_by_id;
  DBStatement *types_insert;
  DBStatement *types_update;
  DBStatement *types_delete;
  GtkWidget *types_sw;
  GtkTreeModel *types_treemodel;
  GtkTreeSelection *types_selection;
  GtkWidget *types_treeview;
  GtkWidget *types_ident;
  GtkWidget *types_make;
  GtkWidget *types_model;
  GtkWidget *types_airplane;
  GtkWidget *types_rotorcraft;
  GtkWidget *types_glider;
  GtkWidget *types_lta;
  GtkWidget *types_poweredlift;
  GtkWidget *types_ppc;
  GtkWidget *types_weightshift;
  GtkWidget *types_heli;
  GtkWidget *types_gyro;
  GtkWidget *types_airship;
  GtkWidget *types_balloon;
  GtkWidget *types_single;
  GtkWidget *types_multi;
  GtkWidget *types_land;
  GtkWidget *types_sea;
  GtkWidget *types_turbine;
  GtkWidget *types_jet;
  GtkWidget *types_highperf;
  GtkWidget *types_retract;
  GtkWidget *types_complex;
  GtkWidget *types_pressurized;
  GtkWidget *types_large;
  GtkWidget *types_sport;
  GtkWidget *types_ultralight;
  GtkWidget *types_footlaunch;
  GtkWidget *types_sim;
  GtkWidget *types_ftd;
  GtkWidget *types_total;

  int airports_stale;
  int airports_modified;
  DBStatement *airports_select_all;
  DBStatement *airports_select_by_id;
  DBStatement *airports_insert;
  DBStatement *airports_update;
  DBStatement *airports_delete;
  GtkWidget *airports_sw;
  GtkTreeModel *airports_treemodel;
  GtkTreeSelection *airports_selection;
  GtkWidget *airports_treeview;
  GtkWidget *airports_ident;
  GtkWidget *airports_icao;
  GtkWidget *airports_name;
  GtkWidget *airports_tzone;
  GtkWidget *airports_notes;
};

Sqlpilot *sqlpilot_new(void);
void sqlpilot_finalize(Sqlpilot *);

void barf(const char *message);

#endif
