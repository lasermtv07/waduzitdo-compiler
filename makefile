
waduzitdo-comp: src/main.c src/cli.c src/parser.c src/structs.c
	gcc src/main.c -o waduzitdo-comp
	chmod u+x waduzitdo-comp
clean: waduzitdo-comp
	rm waduzitdo-comp
