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

usage()
{
    echo $(basename $0) [options] option_arg...
    cat<<EOF
Options:
-a --add-todo [todo]    add one todo to todolist
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
	//todo
	tmp_flush
}

todo_main()
{
    while getopts :a:m:u:n:tdhi: opt
    do
	case $opt in
	    a ) add_todo $OPTARG
		;;
	    m ) mark_as_done $OPTARG
		;;
		u )
		;;
        n )
		;;
	    t ) cat $todofile |grep "\[\]"
		;;
	    d ) cat $todofile |grep "\[X\]"
		;;
	    i )
		;;
	    h ) usage
		;;
        ? ) if [ `expr index "adntdi" $OPTARG` -gt 0 ];then
				echo "Need one parameter: " -$OPTARG
			else
				echo "Wrong argument: " -$OPTARG
			fi
		;;
        esac
    done
    shift $(($OPTIND - 1))
# normal processing of arguments...
}

if [ $# -eq 0 ];then
    usage
fi
todo_main $*
# clean remaining tmpfile
if [-f $tmpfile ];then
	rm -f $tmpfile
fi
exit 0