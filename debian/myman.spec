Summary: the MyMan video game
Name: myman
Version: 0.7.1
Group: Amusements/Games
License: BSD
Source: %{name}-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-%{version}-buildroot

%description
MyMan is a video game for color and monochrome text
terminals in the genre of Namco's Pac-Man. It has been
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
sh ./configure --prefix=/usr CFLAGS="${CFLAGS:-${RPM_OPT_FLAGS}}" CXXFLAGS="${CFLAGS:-${RPM_OPT_FLAGS}}"

%install
make -C "${RPM_BUILD_ROOT}" install DESTDIR="${RPM_BUILD_ROOT}"

%clean
make -C "${RPM_BUILD_ROOT}" uninstall distclean DESTDIR="${RPM_BUILD_ROOT}"

%post
mandb /usr/share/man || makewhatis /usr/share/man

%postun
mandb /usr/share/man || makewhatis /usr/share/man

%files
%defattr(-,games,games)
%doc README TODO COPYING ChangeLog
/usr/bin/%{name}
/usr/bin/%{name}.command
/usr/bin/%{name}-%{version}
/usr/share/%{name}-%{version}
/usr/share/doc/%{name}-%{version}
/usr/share/man/man6/%{name}.6
/usr/share/man/man6/%{name}.command.6
/usr/share/man/man6/%{name}-%{version}.6

%changelog
* Tue Sep 30 2008 Benjamin C. Wiley Sittler <bsittler@gmail.com>
- initial RPM spec file created
