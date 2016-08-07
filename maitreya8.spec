%define dir_name maitreya8
%define font_dir %{_datadir}/fonts/truetype

%if 0%{?suse_version} >= 1140
%define _use_internal_dependency_generator 0
%define __find_requires %wx_requires
%endif


Name:           maitreya8
Version:        8.0.pre533
Release:        1%{?dist}
Summary:        Software for Vedic and western astrology
Group:          Productivity/Other
License:        GPLv2+
URL:            http://www.saravali.de
Source0: http://www.saravali.de/downloads/%{version}/%{name}-%{version}.tar.bz2

BuildRoot:       %{_tmppath}/%{name}-%{version}-build

BuildRequires:  gcc-c++

#opensuse 11.4 and better wants wxWidgets-wxcontainer-devel all others wxGTK-devel
#changed 20150306 martin: wxWidgets-wxcontainer-devel replaced by wxWidgets-devel
%if 0%{?suse_version} >= 1140
BuildRequires:  wxWidgets-devel
%else
BuildRequires:  wxGTK-devel
%endif

#let build system find dependancies.
#Requires:       wxGTK,wxBase

#mandrivia needs libfontconfig
%if 0%{?mdkversion} 
BuildRequires:  libfontconfig-devel
BuildRequires:  libfontconfig
%endif

#to verifiy desktop file.
BuildRequires: desktop-file-utils

#suse checks for dups and the desktop file
%if 0%{?suse_version} || 0%{?sles_version}
BuildRequires:  fdupes
BuildRequires: update-desktop-files
#suse is a lang package which must be recommended.
Recommends: %{name}-lang = %{version}
%lang_package
%endif

%description
Free software for western and Vedic astrology (Jyotish). Supports classical
Vedic astrology, KP system and parts of Jaimini system.  Western part
includes Uranian astrology.  Platforms: Win32, Linux and UNIX.


#normal setup
%prep
%setup -q

%build
#autoreconf needed because of reorder patch.
autoreconf -fi
%configure
make %{?_smp_mflags}


%install
make install DESTDIR=$RPM_BUILD_ROOT
#find lang is needed because of reasons in
#http://fedoraproject.org/wiki/Packaging/Guidelines#Why_do_we_need_to_use_.25find_lang.3F
%find_lang %{dir_name}


%if 0%{?suse_version} || 0%{?sles_version}
# check desktop fits suse specs
%suse_update_desktop_file %{dir_name}

%endif

#check the desktop file.
desktop-file-validate %{buildroot}/%{_datadir}/applications/%{dir_name}.desktop

%if 0%{?suse_version} || 0%{?sles_version}
#if suse do fdupes to detect duplicates.
%fdupes -s %{buildroot}
%endif

%clean
rm -rf $RPM_BUILD_ROOT

%if 0%{?suse_version} || 0%{?sles_version}
#if suse reconfigure for fonts.
%post
%run_permissions
%run_suseconfig_fonts
  
%postun
%run_suseconfig_fonts
  
 
%files
%else
%files -f %{dir_name}.lang
%endif
%defattr(-,root,root,-)
%doc COPYING README NEWS INSTALL AUTHORS ChangeLog
%{_bindir}/%{name}8t
%{_bindir}/%{dir_name}
%{_bindir}/%{dir_name}.bin
%{_datadir}/applications/%{dir_name}.desktop
%if 0%{?suse_version} || 0%{?sles_version}
%dir %{font_dir}
%endif
%dir %{font_dir}/%{name}
%{font_dir}/%{name}/Saravali.ttf
%{_datadir}/%{dir_name}
%{_datadir}/pixmaps/maitreya8.png

%if 0%{?suse_version} || 0%{?sles_version}
%files lang -f %{dir_name}.lang
%defattr(-,root,root,-)
%endif

%changelog
