#!/bin/bash



version=`grep AC_INIT configure.ac | sed "s/.*, //g;s/)//g"`

dmgfile="maitreya8-${version}.dmg"

appname=Maitreya8.app

basedir="maitreya8-mac-${version}"
appdir="${basedir}/${appname}"

contentsdir="${appdir}/Contents"
macosdir="${contentsdir}/MacOS"
resourcesdir="${contentsdir}/Resources"

copy_resources()
{
	rname=$1
	echo "copy resources of type $rname ..."
	if [[ ! -d src/resources/${rname} ]]
	then
		echo "FATAL: directory src/resources/${rname} does not exist"
		exit 1
	fi
	mkdir -p $resourcesdir/$rname
	cp -r src/resources/${rname}/*.json $resourcesdir/${rname}
}

create_dmg()
{
	echo "Creating disk image ${dmgfile}"
	rm -f ${dmgfile}
	hdiutil create -srcfolder ${basedir} ${dmgfile}
	echo "Done"
}

cleanup()
{
	echo "Cleaning directory $basedir"
	rm -fr $basedir
	echo "Done"
}

create_targetdir()
{
	echo "Preparing Mac release in directory $appdir ..."

	mkdir -p ${macosdir}
	mkdir -p ${resourcesdir}

	cp src/gui/maitreya8.bin $macosdir/Maitreya8
	cp MacOS/Info.plist $contentsdir
	cp MacOS/PkgInfo $contentsdir
	cp MacOS/readme_macos.txt $basedir

	echo -n "Languages: "
	for l in `cat po/LINGUAS`
	do
		echo -n " $l"
		mkdir -p $resourcesdir/$l
		cp po/$l.gmo $resourcesdir/$l/maitreya8.mo
	done
	echo " done."

	echo "Pictures ..."
	mkdir -p $resourcesdir/pics
	cp src/pics/*.jpg $resourcesdir/pics
	cp src/pics/*.png $resourcesdir/pics
	cp src/pics/*.icns $resourcesdir

	mkdir -p $resourcesdir/fonts
	cp -r src/fonts/Saravali.ttf $resourcesdir/fonts
	cd $basedir
	ln -fs Maitreya8.app/Contents/Resources/fonts/Saravali.ttf 
	cd -

	cp COPYING $resourcesdir
	cd $basedir
	if test ! -L COPYING
	then
		ln -s ${appname}/Contents/Resources/COPYING
	fi
	cd -

	echo "Locations ..."
	mkdir -p $resourcesdir/atlas
	cp src/atlas/geonames1000.sql $resourcesdir/atlas

	echo "resource files ..."
	cp -r src/resources/*.json $resourcesdir

	copy_resources "dasas"
	copy_resources "yogas"
	copy_resources "wchart"
	copy_resources "vchart"
	copy_resources "print"
	copy_resources "mview"

	if test -L ${appname} 
	then
		rm ${appname}
	fi
	ln -s ${appdir}

	echo "Done"
}

echo "Start $1"
echo "Version is \"$version\" dmg is ${dmgfile}"
echo "Appdir is $appdir"


if [[ $1 == "clean" ]]
then
	cleanup
elif [[ $1 == "dmg" ]]
then
	create_targetdir
	create_dmg
else
	create_targetdir
fi


