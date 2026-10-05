#!/usr/bin/env bash
#
# Compares ./ft_ls against the system ls on a generated fixture tree.
# Checks stdout, stderr and the exit code of every case.
#
# usage: tests/run_tests.sh [-v] [-k]
#   -v  also run every case under valgrind and report leaks / errors
#   -k  keep the fixture directory and the diff files after the run
#
# Run it as a normal user: as root, chmod 000 directories stay readable
# and the permission cases test nothing.

set -u

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
FT_LS="$ROOT/ft_ls"
WORK="$ROOT/tests/.work"
FIX="$WORK/fixture"
OUT="$WORK/out"
VALGRIND=0
KEEP=0

while getopts "vk" opt; do
	case "$opt" in
		v) VALGRIND=1 ;;
		k) KEEP=1 ;;
		*) echo "usage: $0 [-v] [-k]"; exit 2 ;;
	esac
done

export LC_ALL=C
export TZ="${TZ:-Europe/Berlin}"

GREEN=$'\033[32m'; RED=$'\033[31m'; YELLOW=$'\033[33m'; RESET=$'\033[0m'
PASS=0; FAIL=0; LEAK=0

make -C "$ROOT" >/dev/null || { echo "${RED}build failed${RESET}"; exit 1; }

cleanup() {
	chmod -R u+rwx "$WORK" 2>/dev/null
	[ "$KEEP" -eq 1 ] || rm -rf "$WORK"
}
trap cleanup EXIT

build_fixture() {
	cleanup
	mkdir -p "$FIX" "$OUT"
	cd "$FIX" || exit 1

	echo hello > file1
	printf 'a longer file with more bytes\n' > file2
	: > empty_file
	echo hidden > .hidden_file
	mkdir -p dir/sub/deep .hidden_dir empty_dir
	echo x > dir/a; echo yy > dir/b; echo z > dir/.dot
	echo deep > dir/sub/deep/leaf
	echo h > .hidden_dir/inside
	ln -s file1 link_to_file
	ln -s dir link_to_dir
	ln -s does_not_exist broken_link
	mkfifo fifo
	echo exe > exe; chmod 755 exe
	echo s > suid; chmod 4755 suid
	echo s > suid_noexec; chmod 4644 suid_noexec
	echo g > sgid; chmod 2755 sgid
	mkdir sticky; chmod 1777 sticky
	mkdir sticky_noexec; chmod 1776 sticky_noexec
	echo big > big; truncate -s 123456789 big

	touch -d "2001-02-03 04:05:06" old_file
	touch -d "+3 days" future_file
	touch -d "-2 months" recent_file
	mkdir times
	touch -d "2020-01-01 00:00:00.300" times/c
	touch -d "2020-01-01 00:00:00.200" times/b
	touch -d "2020-01-01 00:00:00.100" times/a
	touch -d "2020-01-01 00:00:00.000" times/same_x times/same_y

	mkdir -p locked/inner; chmod 000 locked
	mkdir -p partial/blocked; echo k > partial/ok; chmod 000 partial/blocked
	mkdir -p noexec/sub; : > noexec/f; ln -s f noexec/l; chmod 444 noexec
}

# run_case <description> <args...>
run_case() {
	local name="$1"; shift
	local id=$((PASS + FAIL + 1))
	local exp="$OUT/$id.expected" got="$OUT/$id.got"

	(cd "$FIX" && ls "$@" >"$exp.out" 2>"$exp.err"; echo "exit $?" >>"$exp.out")
	(cd "$FIX" && "$FT_LS" "$@" >"$got.out" 2>"$got.err"; echo "exit $?" >>"$got.out")
	sed -i "s|$FT_LS|ls|g" "$got.err"

	if cmp -s "$exp.out" "$got.out" && cmp -s "$exp.err" "$got.err"; then
		PASS=$((PASS + 1))
		printf '%s[OK]%s   %s\n' "$GREEN" "$RESET" "$name"
	else
		FAIL=$((FAIL + 1))
		printf '%s[FAIL]%s %s   (ls %s)\n' "$RED" "$RESET" "$name" "$*"
		diff <(cat "$exp.out" "$exp.err") <(cat "$got.out" "$got.err") \
			| head -20 | sed 's/^/        /'
	fi
	if [ "$VALGRIND" -eq 1 ]; then
		(cd "$FIX" && valgrind --leak-check=full \
			--show-leak-kinds=definite,indirect \
			--errors-for-leak-kinds=definite,indirect --error-exitcode=42 \
			-q "$FT_LS" "$@" >/dev/null 2>"$got.vg")
		if [ $? -eq 42 ]; then
			LEAK=$((LEAK + 1))
			printf '%s[LEAK]%s %s\n' "$YELLOW" "$RESET" "$name"
			head -15 "$got.vg" | sed 's/^/        /'
		fi
	fi
}

build_fixture

echo "== basic"
run_case "no arguments"
run_case "dot"                          .
run_case "single directory"             dir
run_case "trailing slash"               dir/
run_case "absolute path"                "$FIX/dir"
run_case "single file"                  file1
run_case "empty directory"              empty_dir

echo "== -a"
run_case "-a"                           -a
run_case "-a on directory"              -a dir

echo "== -r / -t"
run_case "-r"                           -r
run_case "-t nanosecond order"          -t times
run_case "-t equal times use name"      -t times/same_y times/same_x
run_case "-tr"                          -tr times
run_case "-t operands"                  -t old_file recent_file future_file file1
run_case "-r operands"                  -r file1 file2 dir empty_dir

echo "== -l"
run_case "-l"                           -l
run_case "-la"                          -la
run_case "-l file operands"             -l file1 file2 big
run_case "-l files and directory"       -l file1 big dir
run_case "-l widths include dir operand" -l file1 dir
run_case "-l special permissions"       -l suid suid_noexec sgid sticky sticky_noexec exe
run_case "-l dates (old/future/recent)" -l old_file future_file recent_file
run_case "-l symlinks"                  -l link_to_file link_to_dir broken_link
run_case "-l fifo"                      -l fifo
run_case "-l /dev/null"                 -l /dev/null
# Not "-l /dev": some files there carry ACLs (the "+" after the mode),
# which is a bonus feature and shifts every column by one.
run_case "-l device files"              -l $(command ls -d /dev/null /dev/zero \
	/dev/tty /dev/random /dev/loop0 /dev/sda /dev/nvme0n1 2>/dev/null)
run_case "-l /etc"                      -l /etc
run_case "-lt"                          -lt
run_case "-lrt"                         -lrt
run_case "-l empty directory"           -l empty_dir

echo "== -R"
run_case "-R"                           -R
run_case "-Ra"                          -Ra
run_case "-lR"                          -lR
run_case "-lRa"                         -lRa
run_case "-Rr"                          -Rr
run_case "-Rt"                          -Rt
run_case "-R on file"                   -R file1
run_case "-R does not follow links"     -R link_to_dir/
run_case "-R two operands"              -R dir times

echo "== operands"
run_case "multiple directories"         dir times empty_dir
run_case "files then directories"       dir file2 times file1
run_case "symlink to dir (no -l)"       link_to_dir
run_case "symlink to dir (-l)"          -l link_to_dir
run_case "broken symlink operand"       broken_link
run_case "same operand twice"           file1 file1 dir dir

echo "== options parsing"
run_case "separate flags"               -l -a -r dir
run_case "flags after paths"            dir -l
run_case "-- ends options"              -- -l file1
run_case "lone dash is a path"          -
run_case "invalid option"               -z
run_case "invalid option in group"      -laz dir
run_case "unrecognized long option"     --nope

echo "== errors"
run_case "missing file"                 nope
run_case "missing files order"          zzz aaa mmm
run_case "missing + existing"           nope file1 dir
run_case "file inside not-a-directory"  file1/x
run_case "quoting in error messages"    "it's" "a b"
run_case "unreadable directory"         locked
run_case "file + unreadable directory"  file1 locked
run_case "unreadable + readable dir"    locked dir
run_case "unreadable subdirectory -R"   -R partial
run_case "unreadable subdirectory -lR"  -lR partial
run_case "not searchable dir"           noexec
run_case "not searchable dir -l"        -l noexec
run_case "not searchable dir -la"       -la noexec
run_case "not searchable dir -t"        -t noexec
run_case "not searchable dir -R"        -R noexec
run_case "not searchable dir -lR"       -lR noexec

echo
printf 'passed: %s%d%s  failed: %s%d%s' "$GREEN" "$PASS" "$RESET" "$RED" "$FAIL" "$RESET"
[ "$VALGRIND" -eq 1 ] && printf '  leaks/errors: %s%d%s' "$YELLOW" "$LEAK" "$RESET"
echo
[ "$FAIL" -eq 0 ] && [ "$LEAK" -eq 0 ]
