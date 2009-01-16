[CCode (cheader_filename = "stdio.h")]
public static int sscanf (...);

[CCode (cprefix = "G", lower_case_cprefix = "g_", cheader_filename = "glib.h")]
namespace Stardate {
	[CCode (has_type_id = false)]
	public struct TimeVal {
		public long tv_sec;
		public long tv_usec;

		[CCode (cname = "g_get_current_time")]
		public TimeVal ();
		[CCode (cname = "g_get_current_time")]
		public void get_current_time ();
		public void add (long microseconds);
		[CCode (instance_pos = -1)]
		public bool from_iso8601 (string iso_date);
		[InstanceByReference]
		public string to_iso8601 ();
	}

	public struct DateDay : uchar {
		[CCode (cname = "G_DATE_BAD_DAY")]
		public static DateDay BAD_DAY;

		[CCode (cname = "g_date_valid_day")]
		public bool valid ();
	}

	[CCode (cprefix = "G_DATE_", has_type_id = false)]
	public enum DateMonth {
		BAD_MONTH,
		JANUARY,
		FEBRUARY,
		MARCH,
		APRIL,
		MAY,
		JUNE,
		JULY,
		AUGUST,
		SEPTEMBER,
		OCTOBER,
		NOVEMBER,
		DECEMBER;

		[CCode (cname = "g_date_get_days_in_month")]
		public uchar get_days_in_month (DateYear year);
		[CCode (cname = "g_date_valid_month")]
		public bool valid (); 
	}

	public struct DateYear : ushort {
		[CCode (cname = "G_DATE_BAD_YEAR")]
		public static DateDay BAD_YEAR;

		[CCode (cname = "g_date_is_leap_year")]
		public bool is_leap_year ();
		[CCode (cname = "g_date_get_monday_weeks_in_year")]
		public uchar get_monday_weeks_in_year ();
		[CCode (cname = "g_date_get_sunday_weeks_in_year")]
		public uchar get_sunday_weeks_in_year ();
		[CCode (cname = "g_date_valid_year")]
		public bool valid ();
	}

	[CCode (cprefix = "G_DATE_", has_type_id = false)]
	public enum DateWeekday {
		BAD_WEEKDAY,
		MONDAY,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY,
		SUNDAY;

		[CCode (cname = "g_date_valid_weekday")]
		public bool valid (); 
	}

	public struct Date {
		public void clear (uint n_dates = 1);
		public void set_day (DateDay day);
		public void set_month (DateMonth month);
		public void set_year (DateYear year);
		public void set_dmy (DateDay day, int month, DateYear y);
		public void set_julian (uint julian_day);
		public void set_time_val (TimeVal timeval);
		public void set_parse (string str);
		public void add_days (uint n_days);
		public void subtract_days (uint n_days);
		public void add_months (uint n_months);
		public void subtract_months (uint n_months);
		public void add_years (uint n_years);
		public void subtract_years (uint n_years);
		public int days_between (Date date2);
		public int compare (Date rhs);
		public void clamp (Date min_date, Date max_date);
		public void order (Date date2);
		public DateDay get_day ();
		public DateMonth get_month ();
		public DateYear get_year ();
		public uint get_julian ();
		public DateWeekday get_weekday ();
		public uint get_day_of_year ();
		public bool is_first_of_month ();
		public bool is_last_of_month ();
		public uint get_monday_week_of_year ();
		public uint get_sunday_week_of_year ();
		public uint get_iso8601_week_of_year ();
		[CCode (instance_pos = -1)]
		public size_t strftime (char[] s, string format);
		[CCode (cname = "g_date_to_struct_tm")]
		public void to_time (out Time tm);
		public bool valid ();
		public static uchar get_days_in_month (DateMonth month, DateYear year);
		public static bool valid_day (DateDay day);
		public static bool valid_dmy (DateDay day, DateMonth month, DateYear year);
		public static bool valid_julian (uint julian_date);
		public static bool valid_weekday (DateWeekday weekday);

		public string to_iso8601 () {
			DateDay y;
			int m;
			DateYear d;			
			return "la la la";
		}

		public Date from_iso8601 (string date) {
			DateDay y;
			int m;
			DateYear d;
			sscanf (date, "%h-%d-%hh", out y, out m, out d);
 			this.set_dmy(d,m,y);
			return this;
		}
	}

	[CCode (cname = "struct tm", cheader_filename = "time.h", has_type_id = false)]
	public struct Time {
		[CCode (cname = "tm_sec")]
		public int second;
		[CCode (cname = "tm_min")]
		public int minute;
		[CCode (cname = "tm_hour")]
		public int hour;
		[CCode (cname = "tm_mday")]
		public int day;
		[CCode (cname = "tm_mon")]
		public int month;
		[CCode (cname = "tm_year")]
		public int year;
		[CCode (cname = "tm_wday")]
		public int weekday;
		[CCode (cname = "tm_yday")]
		public int day_of_year;
		[CCode (cname = "tm_isdst")]
		public int isdst;

		[CCode (cname = "gmtime_r", instance_pos = -1)]
		public Time.gm (time_t time);
		[CCode (cname = "localtime_r", instance_pos = -1)]
		public Time.local (time_t time);

		public string to_string () {
			return "%04d-%02d-%02d %02d:%02d:%02d".printf (year + 1900, month + 1, day, hour, minute, second);
		}

		public string format (string format) {
			var buffer = new char[64];
			this.strftime (buffer, format);
			return (string) buffer;
		}

		[CCode (cname = "mktime")]
		public time_t mktime ();

		[CCode (cname = "strftime", instance_pos = -1)]
		public size_t strftime (char[] s, string format);
		[CCode (cname = "strptime", instance_pos = -1)]
		public weak string? strptime (string buf, string format);
	}
}