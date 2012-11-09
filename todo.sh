#!/usr/bin/env bash
# This file is using for managing todolist, it uses TodoList.txt to record
# todo items. If the file not exists, it'll create one.

# get last argument, or using todofile=${!#} for bash 3.0|greater
# more portable way: for todofile; do true; done
# todofile=${@: -1}
# using Todolist.txt as default, create if not exsit
todofile="TodoList.txt"
[ ! -f ${todofile} ] && touch ${todofile}
tmpfile=".tmpfile"
cat ${todofile} > ${tmpfile}
program=$0

usage()
{
    echo $(basename $0) [options] option_arg...
    cat<<EOF
Options:
-a --add-todo [todo]      add one todo to todolist
-m --mark-done [index]    mark one todo as done
-u --unmark-todo [index]  unmark one todo as undone
-n --add-note [index]     add note to one todo
-t --show-todo            show list of todo items
-d --show_done            show list of done items
-i --show_detail [index]  show detail of one todo item
-h --help                 show this help
EOF
    exit 0
}

tmp_flush()
{
	cat ${tmpfile} > ${todofile}
	rm -f ${tmpfile}
}

add_todo()
{
	lastline=`tail -n 1 ${tmpfile}`
	regexp="^[1-9][0-9]*"
	lastnum=`expr "$lastline" : "\($regexp\)\. \[.*"`
	newtasknum=`expr $lastnum + 1`
	echo -e "\n${newtasknum}. [] $1" >> ${tmpfile}
	tail -n 1 ${tmpfile}
	tmp_flush
}

mark_as_done()
{
	grep "^$1\. \[\]" $tmpfile > /dev/null
	grepret=$?
	if [ $grepret -eq 0 ]
	then
		sed "s/^$1\. \[\]/$1\. \[X\]/1" ${todofile} > ${tmpfile}
		grep "^$1\. \[X\]" $tmpfile
	else
		grep "^$1\. \[X\]" $tmpfile
		grepret=$?
		if [ $grepret -eq 0 ]
		then
			echo "Task $1 has already been done."
		else
			echo "Task $1 does not exist."
		fi
	fi
	tmp_flush
}

unmark_todo()
{
	grep "^$1\. \[X\]" $tmpfile > /dev/null
	grepret=$?
	if [ $grepret -eq 0 ]
	then
		sed "s/^$1\. \[X\]/$1\. \[\]/1" ${todofile} > ${tmpfile}
		grep "^$1\. \[\]" $tmpfile
	else
		grep "^$1\. \[\]" $tmpfile
		grepret=$?
		if [ $grepret -eq 0 ]
		then
			echo "Task $1 has not been done, no need of change"
		else
			echo "Task $1 does not exist."
		fi
	fi
	tmp_flush
}

todo_main()
{
    set -- `getopt -n$program -u -a --longoptions="add-todo: mark-done: unmark-todo: add-note: show-todo show-done show-detail: help" a:m:u:n:tdhi: $*` || usage
    while [ $# -gt 0 ]
    do
	case $1 in
	    -a | --add-todo )
		add_todo $2;shift
		;;
	    -m | --mark-done )
		mark_as_done $2;shift
		;;
	    -u | --unmark-todo ) 
		unmark_todo $2;shift
		;;
            -n | --add-note )
		shift
		;;
	    -t | --show-todo )
		cat $todofile |grep "\[\]"
		shift;;
	    -d | --show-done )
		cat $todofile |grep "\[X\]"
		shift;;
	    -i | --show-detail )
		shift
		;;
	    -h | --help ) 
		usage
		shift;;
	    -- )shift;break;;
	    -* )usage;;
            ?  )if [ `expr index "adntdi" $2` -gt 0 ];then
		echo "Need one parameter: " -$2
		else
		echo "Wrong argument: " -$2
		fi
		shift
		;;
        esac
	shift
    done
# more processing of remaining parameters?
}

if [ $# -eq 0 ];then
    usage
else
    todo_main $*
fi
# clean remaining tmpfile
if [ -f $tmpfile ];then
	rm -f $tmpfile
fi
exit 0