#!/usr/bin/perl
#
# Import script for Geonames.org data to maitreya atlas database
#
# A new database must have tables (import db_dl.sql, see readme.txt)
#
#
#

use strict;

#
# Setup database name
#
# my $dbname = "geonames15000.db";
my $dbname = "geonames.db";
# my $dbname = "geonames_all.db";
my $dbname = $ENV{"HOME"} .  "/.maitreya-8.0/" . $dbname;

#
# The files below can be downloaded from the download area of geonames.org
#
my $tzfile="data/timeZones.txt";
my $countryfile="data/countryInfo.txt";
my $featurefile="data/featureCodes_en.txt";
my $admincodesfile="data/admin1CodesASCII.txt";

#
# comment/uncomment according to downloaded location file
#
# my $cityfile="data/cities15000.txt";
my $cityfile="data/cities1000.txt";
# my $cityfile="data/cities15000.txt";
# my $cityfile="data/allCountries.txt";

use DBI;
my $dbh = DBI->connect("dbi:SQLite:dbname=$dbname","","");

sub trim($)
{
	my $string = shift;
	$string =~ s/^\s+//;
	$string =~ s/\s+$//;
	return $string;
}

# doesn't work
sub new_db()
{
	unlink $dbname;
	my $cmd = "sqlite3 -init " . $ENV{"PWD"} . "/db_ddl.sql $dbname";
	print "CMD: " . $cmd . "\n";
	system $cmd;
}

sub insert_featurecodes
{
	my $count = 0;

	$dbh->begin_work;
	$dbh->do( "DELETE FROM featurecodes;" );
	$dbh->commit;

	$dbh->begin_work;
	my $statement = "INSERT INTO featurecodes values (?,?,?,?);";
	print "Statement: " . $statement . "\n";
	my $sth = $dbh->prepare( $statement );

	open FILE, "<$featurefile" or die "Kann Datei $featurefile nicht oeffnen\n";
	while ( <FILE> )
	{
		chomp;
		# print;
		next if /^#.*/;

		my ( $code1, $name, $name2 ) = split /\t/;
		my( $fcode, $fclass ) = split /\./, $code1;

		# print "code1: " . $code1 . " fcode " . $fcode .  " class " . $fclass . " Name: " . $name . " name2 " . $name2 . "\n";
		$sth->bind_param(1, $fcode );
		$sth->bind_param(2, $fclass );
		$sth->bind_param(3, $name );
		$sth->bind_param(4, $name2 );
		$sth->execute();
		$count++;
	}
	$dbh->commit;

	close FILE;
	print "Fertig: featurecodes, count " . $count . "\n";
}


sub insert_countries
{
	my $count = 0;

	$dbh->begin_work;
	$dbh->do( "DELETE FROM countries;" );
	$dbh->commit;

	$dbh->begin_work;
	my $statement = "INSERT INTO countries values (?,?,?,?,?,?);";
	print "Statement: " . $statement . "\n";
	my $sth = $dbh->prepare( $statement );

	open FILE, "<$countryfile" or die "Kann Datei $countryfile nicht oeffnen\n";
	while ( <FILE> )
	{
		chomp;
		# print;
		next if /^#.*/;

		my @a = split /\t/;
		my $iso = $a[0];
		my $name = $a[4];
		my $capital = $a[5];
		my $area = $a[6];
		my $population = $a[7];
		my $continent = $a[8];

		# print "ISO: " . $iso . " Name: " . $name . " capital " . $capital . " area " . $area . " continent " . $continent . "\n";
		$sth->bind_param(1, $iso );
		$sth->bind_param(2, $name );
		$sth->bind_param(3, $capital );
		$sth->bind_param(4, $area );
		$sth->bind_param(5, $population );
		$sth->bind_param(6, $continent );
		$sth->execute();
		$count++;
	}
	$dbh->commit;

	close FILE;
	print "Fertig: countries, count " . $count . "\n";
}


sub insert_admincodes
{
	my $count = 0;

	$dbh->begin_work;
	$dbh->do( "DELETE FROM admincodes;" );
	$dbh->commit;

	$dbh->begin_work;
	my $statement = "INSERT INTO admincodes values (?,?,?,?);";
	print "Statement: " . $statement . "\n";
	my $sth = $dbh->prepare( $statement );

	open FILE, "<$admincodesfile" or die "Kann Datei $admincodesfile nicht oeffnen\n";
	while ( <FILE> )
	{
		chomp;
		my ( $code, $name, $nameascii ) = split /\t/;

		$code =~ /([\w\d]+)\.([\d\w]+)/ or die "Wrong admin code " . $code . "\n";;
		my $country = $1;
		my $admincode = $2;

		# print "Code: " . $code . " country " . $country . " admincode " . $admincode . " Name: " . $name . " nameascii " . $nameascii . "\n";
		$sth->bind_param(1, $country );
		$sth->bind_param(2, $admincode );
		$sth->bind_param(3, $name );
		$sth->bind_param(4, $nameascii );
		$sth->execute();
		$count++;
	}
	$dbh->commit;

	close FILE;
	print "Fertig: admincodes, count " . $count . "\n";
}

sub insert_timezones
{
	my $count = 0;

	$dbh->begin_work;
	$dbh->do( "DELETE FROM timezones;" );
	$dbh->commit;
	$dbh->begin_work;
	$dbh->do( "DELETE FROM stimezones;" );
	$dbh->commit;

	$dbh->begin_work;
	my $statement = "INSERT INTO timezones values (?,?,?);";
	# print "Statement: " . $statement . "\n";
	my $sth = $dbh->prepare( $statement );

	my $Sstatement = "INSERT INTO stimezones values (?,?,?,?);";
	my $Ssth = $dbh->prepare( $Sstatement );

	open FILE, "<$tzfile" or die "Kann Datei $tzfile nicht oeffnen\n";
	while ( <FILE> )
	{
		chomp;
		# print;
		my ( $country, $name, $offset ) = split /\t/;
		next if $count++ == 0;

		# print "Country " . $country  . " Name: " . $name . " offset " . $offset . "\n";
		$sth->bind_param(1, $name );
		$sth->bind_param(2, $country );
		$sth->bind_param(3, $offset );
		$sth->execute();

		my ( $continent, $city ) = split /\//, $name;
		# print "Continent " . $continent . " city " . $city . "\n";
		$Ssth->bind_param(1, $continent );
		$Ssth->bind_param(2, $city );
		$Ssth->bind_param(3, $country );
		$Ssth->bind_param(4, $offset );
		$Ssth->execute();
	}
	$dbh->commit;

	close FILE;
	print "Fertig: timezones, count " . $count . "\n";
}

sub dofile
{
	my $filename = shift;
	my $count = 0;
	my $linecount = 0;
	my $transactionsize = 25000;
	my %featurecount;
	my 
	$starttime = time;

	my $prepcache = "PRAGMA cache_size=100000;";
	my $dgeonames = "DELETE FROM geonames;";
	my $daliases = "DELETE FROM geonames_aliases;";

	$dbh->begin_work;
	print $dgeonames . " \n";
	$dbh->do( $prepcache );
	$dbh->do( $dgeonames );
	$dbh->commit;

	$dbh->begin_work;
	print $daliases . " \n";
	$dbh->do( $daliases );
	$dbh->commit;

	my $statement = "INSERT INTO geonames values (?,?,?,?,?,?,?,?,?,?,?,?,?);";
	my $aliasstatement = "INSERT INTO geonames_aliases values (?,?);";
	print "Statement: " . $statement . "\n";
	print "Alias Statement: " . $aliasstatement . "\n";
	my $sth = $dbh->prepare( $statement );
	my $sta = $dbh->prepare( $aliasstatement );

	print "Start file $filename\n";
	open FILE, "<$filename" or die "Kann Datei $filename nicht oeffnen\n";
	$dbh->begin_work;
	while ( <FILE> )
	{
		chomp;
		# print;
		my @a = split /\t/;

		my $id = $a[0];
		my $name = $a[1];
		my $asciiname = $a[2];
		my $aliasname = $a[3];
		my $lat = $a[4];
		my $long = $a[5];
		my $fclass = $a[6];
		my $fcode = $a[7];
		my $ccode = $a[8];
		my $admin1code = $a[10];
		my $population = $a[14];
		my $elevation = $a[15];
		my $timezone = $a[17];

		$linecount++;
		next if $fclass ne "P";

		unless ( $id =~ /\d+/ )
		{
			warn "Wrong id " . $id . " in line " . $linecount . ", skipping\n";
			next;
		}

		print "\nName: " . $name . " ASCII name " . $asciiname . " lat: " . $lat . " long " . $long . " fclass " . $fclass
			. " fcode " . $fcode . " ccode " . $ccode . " pop " . $population . " elevation " . $elevation
			. " tz " . $timezone . "\n"
			if  1 > 2
			;

		if ( $count > 0 and $count % $transactionsize == 0 )
		{
			print "COMMIT transaction (" . $count . ")\n";
			$dbh->commit;
			print "BEGIN transaction\n";
			$dbh->begin_work;
		}

		$featurecount{ $fcode }++;

		$sth->bind_param(1, $id );
		$sth->bind_param(2, $name );
		$sth->bind_param(3, $asciiname );
		$sth->bind_param(4, $lat );
		$sth->bind_param(5, $long );
		$sth->bind_param(6, $fclass );
		$sth->bind_param(7, $fcode );
		$sth->bind_param(8, $ccode );
		$sth->bind_param(9, $admin1code );
		$sth->bind_param(10, $population );
		$sth->bind_param(11, $elevation );
		$sth->bind_param(12, $timezone );
		$sth->bind_param(13, 0 );
		$sth->execute();

		# print "Aliase sind: " . $aliasname . "\n";
		if ( $aliasname eq "" )
		{
			$aliasname = $name;
			$aliasname .= $asciiname if $asciiname ne $name;
		}
		my @aliases = split /,/, $aliasname;
		while ( my $a = shift @aliases )
		{
			# print "Alias " . $id . "\t" . $a . "\n";
			$sta->bind_param(1, $id );
			$sta->bind_param(2, trim $a );
			$sta->execute();
			warn "Wrong alias: " . $a . "\n" if $a eq "";
		}
		$count++;
		# last if $count > 100000;
	}
	print "COMMIT transaction (" . $count . ")\n";
	$dbh->commit;

	close FILE;
	my $totaltime = time - $starttime;
	print "Fertig file " . $filename . " lines " . $linecount . " datasets " . $count . " in " . $totaltime .  " sec\n";
	
	# foreach my $key ( sort keys %featurecount )
	#{
#		my $perc = 100 * $featurecount{ $key } / $count;
#	print $key . ": " . $featurecount{ $key } . " " . $perc . "\n";
	#}

}

print "Start\n";
# new_db;
# insert_featurecodes;
insert_admincodes;
insert_countries;
insert_timezones;

dofile $cityfile;

$dbh->disconnect;

print "Finished\n";

