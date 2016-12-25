#!/usr/bin/python
#
# Import of geonames.org files to sqlite3 database
# See documentation for details
#
#

import os
import sqlite3
import re

# sqlite3 database file
dbname = os.getenv("HOME") + "/.maitreya-8.0/geonames.db"

fcities = "data/cities1000.txt"
tcities = "geonames"
tcities_a = "geonames_aliases"

tadmincodes = "admincodes"
fadmincodes = "data/admin1CodesASCII.txt"

tcountries = "countries"
fcountries = "data/countryInfo.txt"

tfeaturecodes = "featurecodes"
ffeaturecodes = "data/featureCodes_en.txt"

ttimezones = "timezones"
ftimezones = "data/timeZones.txt"

class GeoImporter:
	
	def __init__(self): 
		self.conn = sqlite3.connect( dbname )
		self.conn.text_factory = str

	def do_import( self ):
		self.do_timezones()
		self.do_featurecodes()
		self.do_countries()
		self.do_admincodes()
		self.do_cities()

	def cleanup( self, tname, verbose ):
		sql = "delete from " + tname
		if ( verbose ): print "SQL: " + sql

		cur = self.conn.cursor()
		cur.execute( sql )
		self.conn.commit()

	# 
	# CITIES
	# 
	# Sample
	# 292953	Adh Dhayd	Adh Dhayd	Adh Dhaid,Adh Dhayd,Al Daid,Al-Dhayd,Dayd,Dhaid,Dhayd,Duhayd,Ihaid,aldhyd	25.28812	55.88157	P	PPL	AE		06				24716		114	Asia/Dubai	2012-01-18
	#
	def do_cities( self ):
		print
		print "Start Cities"
		verbose = False
		count = 0
		errors = 0
		self.cleanup( tcities, verbose )
		self.cleanup( tcities_a, verbose )

		cur = self.conn.cursor()
		fobj = open( fcities, "r") 
		for line in fobj: 

			# ignore comments
			if ( re.match( "^#", line )): continue
			if ( verbose ): print "#", line, "#"

			# if ( count > 10 ): break
			if ( count > 0 and count % 1000 == 0 ): print count,"datasets read"

			t = line.split( "\t" )
			if ( t ):
				iid = t[0]
				name = t[1]
				asciiname = t[2]
				aliasname = t[3]
				if ( not aliasname ):
					aliasname = name
					if ( asciiname != name ): aliasname += "," + asciiname
				if ( verbose ):
					print "ID         ",iid
					print "name       ",name
					print "asciiname  ",asciiname
					print "aliasname  ",aliasname
					print "latitude   ",t[4]
					print "longitude  ",t[5]
					print "fclass     ",t[6]
					print "fcode      ",t[7]
					print "ccode      ",t[8]
					print "admin1code ",t[10]
					print "population ",t[14]
					print "elevation  ",t[15]
					print "timezone   ",t[17]

				cur.execute( "insert into " + tcities + " values(?,?,?,?,?,?,?,?,?,?,?,?,?)",
					(iid,name,asciiname,t[4],t[5],t[6],t[7],t[8],t[10],t[14],t[15],t[17],0))

				# alias names are comma serarated and must be parsed separately
				a = aliasname.split ( "," )
				for token in a:
					if ( verbose ): print "TOKEN ", token
					cur.execute( "insert into " + tcities_a + " values(?,?)", (iid,token))
			else:
				print "ERROR#"+line+"#"
				errors += 1
			count += 1;

		self.conn.commit()
		fobj.close()
		print "Finished cities,", count, "datasets inserted,", errors, "errors"
		print

	# 
	# 
	# TIMEZONES
	# 
	# Sample
	# CountryCode	TimeZoneId	GMT offset 1. Jan 2016	DST offset 1. Jul 2016	rawOffset (independant of DST)
	# CI	Africa/Abidjan	0.0	0.0	0.0
	#
	def do_timezones( self ):
		print
		print "Start Time Zones"
		verbose = False
		count = 0
		errors = 0
		self.cleanup( ttimezones, verbose )

		cur = self.conn.cursor()
		fobj = open( ftimezones, "r") 
		for line in fobj: 

			# ignore comments
			if ( re.match( "^#", line )): continue
			if ( verbose ): print "#", line, "#"

			t = line.split( "\t" )
			if ( t ):
				if ( verbose ):
					print "name       ",t[0]
					print "country    ",t[1]
					print "offset     ",t[2]

				# colums for countrycode an tzname are different
				cur.execute( "insert into " + ttimezones + " values(?,?,?)", (t[1],t[0],t[2]))
			else:
				print "ERROR#"+line+"#"
				print "SQL insert into " + ttimezones + " values(?,?,?)", t[0]," ",t[1], " ",t[2]
				errors += 1
			count += 1;

		self.conn.commit()
		fobj.close()
		print "Finished Time Zones,", count, "datasets inserted,", errors, "errors"
		print

	# 
	# FEATURECODES
	# 
	# Sample
	# A.ADM1	first-order administrative division	a primary administrative division of a country, such as a state in the United States
	#
	def do_featurecodes( self ):
		print
		print "Start Feature Codes"
		verbose = False
		count = 0
		errors = 0
		self.cleanup( tfeaturecodes, verbose )

		cur = self.conn.cursor()
		fobj = open( ffeaturecodes, "r") 
		for line in fobj: 

			# ignore comments
			if ( re.match( "^#", line )): continue
			if ( verbose ): print "#", line, "#"

			# fcode DOT fclass TAB name TAB name2
			t = re.search( ur'^(\w+)\.(\w+)[\t\s*]([^\t]*)[\t\s*]([^\t]*)', line, re.UNICODE )

			if ( t ):
				if ( verbose ):
					print "fcode  ",t.group(1)
					print "fclass ",t.group(2)
					print "name   ",t.group(3)
					print "name2  ",t.group(4)
				cur.execute( "insert into " + tfeaturecodes + " values(?,?,?,?)", (t.group(1),t.group(2),t.group(3),t.group(4)))
			else:
				print "ERROR#"+line+"#"
				errors += 1
			count += 1;

		self.conn.commit()
		fobj.close()
		print "Finished Feature Codes,", count, "datasets inserted,", errors, "errors"
		print

	# 
	# COUNTRIES
	# 
	# ISO	ISO3	ISO-Numeric	fips	Country	Capital	Area(in sq km)	Population	Continent	tld	CurrencyCode etc
	#
	# Sample
	# AF	AFG	004	AF	Afghanistan	Kabul	647500	29121286	AS	.af	AFN	Afghani	93			fa-AF,ps,uz-AF,tk	1149361	TM,CN,IR,TJ,PK,UZ	
	#
	def do_countries( self ):
		print
		print "Start Countries"
		verbose = False
		count = 0
		errors = 0
		self.cleanup( tcountries, verbose )

		cur = self.conn.cursor()
		fobj = open( fcountries, "r") 
		for line in fobj: 

			# ignore comments
			if ( re.match( "^#", line )): continue
			if ( verbose ): print "#", line, "#"
			t = line.split( "\t" )
			if ( t ):
				if ( verbose ):
					print "iso        ",t[0]
					print "name       ",t[4]
					print "capital    ",t[5]
					print "area       ",t[6]
					print "population ",t[7]
					print "continent  ",t[8]

				cur.execute( "insert into " + tcountries + " values(?,?,?,?,?,?)", (t[0],t[4],t[5],t[6],t[7],t[8]))
			else:
				print "ERROR#"+line+"#"
				errors += 1
			count += 1;

		self.conn.commit()
		fobj.close()
		print "Finished Countries,", count, "datasets inserted,", errors, "errors"
		print

	#
	# ADMINCODES
	#
	# Sample
	# AD.07	Andorra la Vella	Andorra la Vella	3041566
	#
	def do_admincodes( self ):
		print
		print "Start Admincodes"
		verbose = False
		count = 0
		errors = 0
		self.cleanup( tadmincodes, verbose )

		cur = self.conn.cursor()
		fobj = open( fadmincodes, "r") 
		for line in fobj: 
			if ( verbose ): print "#", line, "#"

			# countrycode DOT admincode TAB name TAB nameascii TAB id
			t = re.search( ur'^(\w+)\.(\w+)[\t\s*]([^\t]*)[\t\s*]([^\t]*)[\t\s*](\d+)\n$', line, re.UNICODE )

			if ( t ):
				if ( verbose ):
					print "country   ",t.group(1)
					print "admincode ",t.group(2)
					print "name      ",t.group(3)
					print "nameascii ",t.group(4)
					print "id        ",t.group(5)
				cur.execute( "insert into " + tadmincodes + " values(?,?,?,?)", (t.group(1),t.group(2),t.group(3),t.group(4)))
			else:
				print "ERROR#"+line+"#"
				errors += 1
			count += 1;

		self.conn.commit()
		fobj.close()
		print "Finished Admincodes,", count, "datasets inserted,", errors, "errors"
		print

#
# main
#
print "Start"

g = GeoImporter()
g.do_import()

print "Finished"
