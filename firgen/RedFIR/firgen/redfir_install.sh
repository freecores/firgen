#!/bin/bash
copy_and_delete()
{
	rm -f ./test_area/redFir_engine/firgen
	cp -f ./firgen ./test_area/redFir_engine
}


main()
{
	if ! copy_and_delete
		then
			echo "Fehler bei copy_and_delete() ...";
			exit;
	fi
	echo "done";
}
main
