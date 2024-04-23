# Checkpoint 1

Map: 98x3

This checkpoint is implemented using heuristic approach:

- Going from top (exit) to bottom (my_car) i look for any cars that are blocking
- Whenever i find one i try to move it and then recursively move any other car that is blocking that one
- Vertically oriented cars i try to move to the furthest place on the upper side of map (if above my_car)
- If that's not possible then in first possible place (not blocked by horizontally oriented cars)
- In the case of vertical cars, I additionally transmit the place I want to move to (I transmit to other vertical cars) so that they know from which position they can start searching for a possible place to move to.
- Ocassionally i change the values of horizontall/vertically cars to 'x' (border) to make sure that they cannot be moved anymore (just in case for other instances of recursion) 
