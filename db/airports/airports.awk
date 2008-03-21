BEGIN {
    FS=","
}

{
    print "INSERT INTO Airports"					\
	" (ICAO, Ident, Name, City, Province, Country, Lat, Lon, Elev, OffUTC, OffDST, TZone)" \
	" VALUES (\"" $1 "\", \"" $2 "\", \""  $3 "\", \""  $4 "\", \""  $5 "\", \""  $6 "\", \""  $7 "\", \""  $8 "\", \"" \
	$9  "\", \""  $10  "\", \""  $11  "\", \""  $12 "\");"
}