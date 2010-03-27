#include <iostream>
#include <QPoint>
#include "Physics.h"
#include "Frame.h"
#include "Ship.h"
#include "../../Core/comm/data/clientaction.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;
int main() {
	Frame frame("gameplay/map.xml");

	frame.addShip(0);
	frame.spawnShip(0);
	frame.printShips();    
	ClientAction actionMask(0);
	actionMask.setAccelerating();
	frame.listShip[0]->applyActionMask(actionMask);
	frame.tick();
	frame.printShips();
	frame.printShots();
	//system("clear");
	frame.tick();
	frame.printShips();
        actionMask.clear();
	frame.listShip[0]->applyActionMask(actionMask);
	frame.tick();
	frame.printShips();
	frame.tick();
	frame.printShips();
	return(0);
	actionMask.clear();
	frame.listShip[0]->applyActionMask(actionMask);
	while(1){
        frame.tick();
	frame.printShips();
	system("clear");
        }
/*
cout << endl;
QPoint tPoint = QPoint(29,25);
QPoint t2Point = QPoint(0,0);
if(frame.map.isWall(tPoint)){
  cout << "wall at 25x25" << endl;
}
if(frame.map.isWall(t2Point)){
  cout << "wall at 0x0" << endl;
}
cout << "THE TEST!" << endl << frame.map.canMove(tPoint, false, 50, -6) << endl;
*/


	/*ClientAction actionMask(0);
    actionMask.setAccelerating();
    actionMask.setTurningLeft();
    Frame frame("map.xml");
    Ship ship01, ship02;
    frame.addShip(ship01);
    frame.addShip(ship02);
    frame.printShips();
    frame.spawnShip(0);
    frame.printShips();
    frame.getShip(0)->applyActionMask(actionMask);
    frame.tick();
    frame.printShips();
    frame.tick();
    frame.printShips();
    actionMask.clear();
    frame.getShip(0)->applyActionMask(actionMask);
    frame.tick();
    frame.printShips();
    
    QPoint tPoint = QPoint(4,4);
    
    
    cout << "THE TEST!" << endl << frame.map.canMove(tPoint, true, 50, 0) << endl;
    */
    return 0;
}