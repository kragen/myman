#!/bin/bash --
# lsic.sh - compile MyMan for DOS using LSIC-86
#
# This calls lsic.bat inside DOSBox to do the dirty work.
#
# Configure your LSIC-86 according to these instructions:
#     http://atmori.hp.infoseek.co.jp/lsic/detail.htm

# override DOSBOX to point to your DOSBox binary and possibly some arguments
DOSBOX="${DOSBOX:-dosbox}"

# override $LSIC to point to the root of your LSIC-86 installation,
# i.e. the directory containing a "lsic86" subdirectory with "bin"
# sub-subdirectory
LSIC="${LSIC:-~}"

prefix="LCC$$"
srcdir="$(dirname "$0")"/..
TMPDIR="${TMPDIR:-/tmp}"
errfile="${prefix}.ERR"
retfile="${prefix}.RET"

# allow settings to be overridden on the command line
while [ $# -gt 0 ]
do
    if [ :"${1/=/}" != :"$1" ]
    then
        export -- "$1"
    else
        echo "$0: unrecognized option: '$1'" >&2
        echo "usage: $0 [ VAR1=VALUE1 [ VAR2=VALUE2 [ ... ] ]" >&2
        exit 2
    fi
    shift
done

cd "$srcdir" &&
rm -f "${TMPDIR}/${errfile}" &&
touch "${TMPDIR}/${errfile}" &&
rm -f "${TMPDIR}/${retfile}" &&
echo 0 > "${TMPDIR}/${retfile}" &&
(
    tail -f "${TMPDIR}/${errfile}" | sed 's/$//;s/^\([^:]*\)\\/\1\//;s/^\([^ ]*\) \([0-9]\+\): /\1:\2: /'
) & tail=${!}
sleep 1
(echo '[dosbox]'; echo 'memsize=128'; echo '[cpu]'; echo 'cycles=max') | $DOSBOX -conf /dev/fd/0 -noautoexec -c cls -c 'mount c '"${LSIC}" -c 'mount t '"${TMPDIR:-/tmp}" -c 'mount d .' -c 'd:' -c 'path c:\lsic86\bin;%path%' -c 'command /c call utl\lsic > t:'"${errfile}"'' -c 'if errorlevel 1 echo 1 >> t:'"${retfile}"'' -c 'exit' >/dev/null || echo $? > "${TMPDIR}/${retfile}"
sleep 1
disown "$tail"
kill "$tail"
ret="$(cat "${TMPDIR}/${retfile}" | tr -d '' | tail -1)"
rm -f "${TMPDIR}/${retfile}" "${TMPDIR}/${errfile}"
exit "$ret"
