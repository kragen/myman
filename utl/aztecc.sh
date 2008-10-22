#!/bin/bash --
# aztecc.sh - compile MyMan for DOS using Aztec C
#
# This calls aztec.bat inside DOSBox to do the dirty work.

# override DOSBOX to point to your DOSBox binary and possibly some arguments
DOSBOX="${DOSBOX:-dosbox}"

# override $AZTECC to point to the root of your Aztec C installation,
# i.e. the directory containing a "bin" subdirectory
AZTECC="${AZTECC:-~/az8652a}"

prefix="AZC$$"
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
    tail -f "${TMPDIR}/${errfile}" | sed 's/$//;s/^\([^:]*\)\\/\1\//;s/^\([^ ]*\) \([^:]*\) \([0-9]\+\): /\2:\3: \1: /;s/\(.*\) (warning)$/warning: \1/;s/ (error)$//'
) & tail=${!}
sleep 1
(echo '[dosbox]'; echo 'memsize=64'; echo '[cpu]'; echo 'cycles=max') | $DOSBOX -conf /dev/fd/0 -noautoexec -c cls -c 'mount c '"${AZTECC}" -c 'mount t '"${TMPDIR:-/tmp}" -c 'mount d .' -c 'd:' -c 'set AZTECC=c:\' -c '%comspec% /c call utl\aztecc > t:'"${errfile}"'' -c 'if errorlevel 1 echo 1 > t:'"${retfile}"'' -c 'exit' >/dev/null || echo $? > "${TMPDIR}/${retfile}"
sleep 1
disown "$tail"
kill "$tail"
rm "${TMPDIR}/${errfile}"
ret="$( ( echo 1; cat "${TMPDIR}/${retfile}" ) | tr -d '' | grep . | tail -1)"
rm -f "${TMPDIR}/${retfile}" "${TMPDIR}/${errfile}"
exit "$ret"
