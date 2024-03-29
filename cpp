#!/bin/sh
#
# Transitional front end to CCCP to make it behave like (Reiser) CCP:
#	specifies -traditional
#	doesn't search gcc-include
#
# Please beware that this program exists only to provide legacy BSD
#    software access to cccp. Direct access to the C pre-processor
#    is deprecated; it is only supported for use by the cc(1) C
#    compiler. Use of cccp for anything other than C code is a bad
#    idea. Don't do it. If you want a macro processor, use m4(1).
#
# Copyright (c) 1990 The Regents of the University of California.
# All rights reserved.
#
# This code is derived from software contributed to Berkeley by
# the Systems Programming Group of the University of Utah Computer
# Science Department.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this software
#    must display the following acknowledgement:
#	This product includes software developed by the University of
#	California, Berkeley and its contributors.
# 4. Neither the name of the University nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
#	@(#)usr.bin.cpp.sh	6.5 (Berkeley) 4/1/91
#
unset xcrun_log
if xcrun -find cc > /dev/null 2>&1; then
  CPP="xcrun cc -E"
else
  CPP="cc -E"
fi
OUTPUT="-"
NSI=no
OPTS=""
INCS=""
FOUNDFILES=no

while [ $# -gt 0 ]
do
	A="$1"
	shift

	case "$A" in
	-nostdinc)
		NSI=yes
		;;
	-traditional)
		;;
	-I*)
		INCS=("${INCS[@]}" "$A")
		;;
	-imacros|-include|-idirafter|-iprefix|-iwithprefix)
		INCS=("${INCS[@]}" "$A" "$1")
		shift
		;;
	-*)
		OPTS=("${OPTS[@]}" "$A")
		;;
	*)
		FOUNDFILES=yes
		# If we've found an input name and there's still an arg left, 
		# that next arg will be the output file.  
		if [ $# -eq 1 ]; then
		    OUTPUT="$1"
		    # and get rid of last arg
		    shift
		fi
		$CPP -traditional "${INCS[@]}" -x c "${OPTS[@]}" "$A" \
		     -o "$OUTPUT" || exit $?
		;;
	esac
done

if [ $FOUNDFILES = "no" ]; then
	# read standard input
	if [ $NSI = "no" ]; then
		INCS=("${INCS[@]}" "-I/usr/include")
	fi
	$CPP -traditional "${INCS[@]}" -x c "${OPTS[@]}" "-" || exit $?
fi

exit 0
