Summary: the MyMan video game
Name: myman
Version: 0.7.1
Release: 1
Group: Amusements/Games
License: BSD
Source: %{name}-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-%{version}-buildroot

%description
MyMan is a video game for color and monochrome text
terminals in the genre of Namco\'s Pac-Man. It has been
ported to a variety of operating systems using the
following for user interaction: ncurses, PDcurses,
Curses, sysV-curses, S/Lang slcurses, Win32 console, raw
stdio and termios, GGI, Allegro, aalib, libcaca, TWin,
and Carbon. It includes many maze variations and several
tile and sprite sets, ranging from large ASCII art
through large pseudo-bitmap Unicode or CP437 graphics to
single characters.

%prep
%setup

%build
%configure
make CFLAGS="${CFLAGS:-%optflags}" CXXFLAGS="${CXXFLAGS:-%optflags}" FFLAGS="${FFLAGS:-%optflags}"

%install
mkdir -p "%{buildroot}"
%makeinstall MAKEWHATIS=: MANDB=:
gzip -9 "%{buildroot}%{_mandir}/man6/%{name}.6"
gzip -9 "%{buildroot}%{_mandir}/man6/%{name}.command.6"
gzip -9 "%{buildroot}%{_mandir}/man6/%{name}-%{version}.6"

%clean
rm -f "%{buildroot}%{_mandir}/man6/%{name}.6.gz"
rm -f "%{buildroot}%{_mandir}/man6/%{name}.command.6.gz"
rm -f "%{buildroot}%{_mandir}/man6/%{name}-%{version}.6.gz"
make uninstall distclean DESTDIR="%{buildroot}"

%files
%defattr(-,root,root)
%doc %{_datadir}/doc/%{name}-%{version}/*
%{_bindir}/%{name}
%{_bindir}/%{name}.command
%{_bindir}/%{name}-%{version}
%{_datadir}/%{name}-%{version}
%{_mandir}/man6/%{name}.6.gz
%{_mandir}/man6/%{name}.command.6.gz
%{_mandir}/man6/%{name}-%{version}.6.gz

%changelog
* Tue Sep 30 2008 Benjamin C. Wiley Sittler <bsittler@gmail.com> 0.7.1-1
- initial RPM spec file created
