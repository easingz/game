#!/usr/bin/env bash
# get last argument, or using todofile=${!#} for bash 3.0|greater
# more portable way: for todofile; do true; done
todofile=${@: -1}
usage()
{
    echo $(basename $0) [options] option_arg...
    cat<<EOF
Options:
-a --add-todo [string]    add one todo to todolist
-d --mark-done [index]    mark one todo as done
-n --add-note [index]     add note to one todo
-t --show-todo            show list of todo items
-D --show_done            show list of done items
-i --show_detail [index]  show detail of one todo item
-h --help                 show this help
EOF
    exit 0
}

todo_main()
{
    while getopts :a:d:n:tDhi: opt
    do
	case $opt in
	    a ) echo it\'s a
		;;
	    d )
		;;
            n )
		;;
	    t ) cat $todofile |grep "\[\]"
		;;
	    D ) cat $todofile |grep "\[X\]"
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
exit 0