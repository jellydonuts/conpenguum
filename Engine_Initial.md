Game engine

Because the game engine is so procedural we decided to
show some (very) high level steps instead of a diagram.

The steps below are meant to show interactions between the
various segments in the engine and the order in which things
will be executed.init graphics
init physics
call menu
init input capture
connect client to server

Start game loop
	receive input
	send input data to server
	server replies with graphics/physics change
	update UI/graphics
	
	if player exits
		break game loop
	
disconnect client from server
call menu
exit or start game again}}}```