#include"player.h"
#include"entity.h"
#include"stack.h"
#include"dllist.h"
#include"room.h"
#include"maze.h"
#include<iostream>

//  Player( ... )
//		Constructs and initializes the Player/Entity and its member variables
//		Remembers and discovers the starting room.
Player::Player(Maze* maze, Room p, std::string name, char sprite, const bool tbacktrack)
	:Entity(maze, p, name, sprite),
	m_lookingPaper(),
	m_discoveredRooms(),
	m_btQueue(),
		m_btStack(),
		BACKTRACKENABLED(tbacktrack) {

		// Discover the starting room
		m_discoveredRooms.insert_front(p);
		m_lookingPaper.enqueue(p);
	}

	//  stuck() 
	//		See if the player is stuck in the maze (no solution)
	bool Player::stuck() const {
		return Entity::state() == State::NOEXIT;
	}

	//  foundExit()
	//		See if the player has found the exit
	bool Player::foundExit() const {
		return Entity::state() == State::EXIT;
	}

	//  getTargetRoom()
	//		Get the room the player wants to look around next.
	//      If m_lookingPaper is empty then return an invalid room.
	Room Player::getTargetRoom() const {
		if (m_lookingPaper.empty()) return Room(-1, -1);
		return m_lookingPaper.peek();
	}

	// discovered
	//		returns true if the item is in the list
	bool Player::discovered(const Room& p) const {
		return (m_discoveredRooms.find(p) != -1);
	}



	// say()
	//		What does the player say?
	//		Already Implemented, nothing to do here
	void Player::say() {

		// Freedom supercedes being eaten
		if (state() == State::EXIT) {
			std::cout << name() << ": WEEEEEEEEE!";
			return;
		}

		// Being eaten supercedes being lost
		switch (interact()) {
		case Interact::ATTACK:
			std::cout << name() << ": OUCH!";
			break;
		case Interact::GREET:
			break;
		case Interact::ALONE:
		default:
			switch (state()) {
			case State::LOOK:
				std::cout << name() << ": Where is the exit?";
				break;
			case State::NOEXIT:
				std::cout << name() << ": Oh no! I am Trapped!";
				break;
			case State::BACKTRACK:
				std::cout << name() << ": Got to backtrack...";
				break;
			default:
				break;
			}

			break;
		}
	}


	//  update() 
	//		This function implements an algorithm to look through the maze
	//      for places to move (STATE::LOOK). update also handles moving the 
	//		player as well, if there is an open undiscovered cell to move to the  
	//		player just moves there.  However, there will be cases in the next 
	//		desired room to look around is not adjacent (more than one cell away), 
	//		peek this room the player must backtrack (State::BACKTRACK) to a room 
	//		that is adjacent to the next room the player was planning to visit
	//
	//		The player can only do one thing each call, they can either LOOK or 
	//		BACKTRACK not both.  Nor should the player move more than one cell per
	//		call to update.  If you examine the function calls for the entire game
	//		you should observe that this is already be called within a loop. Your
	//		Implementation should NOT have any loops that pertain to actual movement 
	//		of the player.  
	//
	//		Backtracking is challenging, save it for the very very very last thing.
	//		Make sure the STATE::LOOK aspect compiles and works first.
	void Player::update() {

		say();
		if (!BACKTRACKENABLED) {

			if (state() == State::LOOK) {

				Room currentRoom = room();

				if (maze()->foundExit(currentRoom)) {
					state(State::EXIT);
					return;
				}

				if (m_lookingPaper.empty()) {
					state(State::NOEXIT);
					return;
				}

				move(getTargetRoom());
				m_discoveredRooms.insert_front(room());

				Room up(room().x(), room().y() + 1);
				Room down(room().x(), room().y() - 1);
				Room left(room().x() - 1, room().y());
				Room right(room().x() + 1, room().y());

				if ((m_discoveredRooms.find(up) == -1) && ((maze()->open(up)))) {
					m_lookingPaper.enqueue(up);
				}

				if ((m_discoveredRooms.find(down) == -1) && ((maze()->open(down)))) {
					m_lookingPaper.enqueue(down);
				}

				if ((m_discoveredRooms.find(left) == -1) && ((maze()->open(left)))) {
					m_lookingPaper.enqueue(left);
				}

				if ((m_discoveredRooms.find(right) == -1) && ((maze()->open(right)))) {
					m_lookingPaper.enqueue(right);
				}

				if (getTargetRoom() == room()) {
					m_lookingPaper.dequeue();
				}

			}

			if (state() == State::BACKTRACK) {

			}

			if (state() == State::NOEXIT) {

			}
			if (state() == State::EXIT) {

			}

			// Set by the settings file, if BACKTRACKENABLED is false, then
			// your program should behave exactly as seen in the slides or
			// example executables (with teleporting).
			// You may have multiple branching statements like this.
			// if(BACKTRACKENABLED) { ... code relating to backtracking 


		}

		if (BACKTRACKENABLED) {

			if (state() == State::LOOK) {

				Room currentRoom = room();
				m_btList.insert_front(room());

				if (maze()->foundExit(currentRoom)) {
					state(State::EXIT);
					return;
				}

				if (m_lookingPaper.empty()) {
					state(State::NOEXIT);
					return;
				}



				// ~~ ** ROOM LOGGING ** ~~

				if (m_discoveredRooms.find(room()) == -1) {
					m_discoveredRooms.insert_front(room());
				}

				Room up(room().x(), room().y() + 1);
				Room down(room().x(), room().y() - 1);
				Room left(room().x() - 1, room().y());
				Room right(room().x() + 1, room().y());

				if ((m_discoveredRooms.find(down) == -1) && ((maze()->open(down)))) {
					m_lookingPaper.enqueue(down);
				}

				if ((m_discoveredRooms.find(up) == -1) && ((maze()->open(up)))) {
					m_lookingPaper.enqueue(up);
				}

				if ((m_discoveredRooms.find(right) == -1) && ((maze()->open(right)))) {
					m_lookingPaper.enqueue(right);
				}

				if ((m_discoveredRooms.find(left) == -1) && ((maze()->open(left)))) {
					m_lookingPaper.enqueue(left);
				}

				
				
				// ~~ ** MOVEMENT ** ~~

				if (getTargetRoom() == room()) {
					m_lookingPaper.dequeue();
				}

				if (getTargetRoom() == down) {
					move(down);
					return;
				}
				if (getTargetRoom() == up) {
					move(up);
					return;
				}
				if (getTargetRoom() == right) {
					move(right);
					return;
				}
				if (getTargetRoom() == left) {
					move(left);
					return;
				}


				
				// ~~ ** DEAD END CHECKER ** ~~
				if ((!(m_btList.find(down) == -1) || (!(maze()->open(down)))) && (!(m_btList.find(up) == -1) || (!(maze()->open(up)))) && (!(m_btList.find(left) == -1) || (!(maze()->open(left)))) && (!(m_btList.find(right) == -1) || (!(maze()->open(right))))) {
					state(State::BACKTRACK);
				}


				// Pick a random direction
				int dir = maze()->randInt(0, 3);

				// Check to see if desired direction is inbounds.
				// Accounting for the Walls and one space to allow
				// movement in that direction. Update the x,y
				// coordinates or do nothing
				switch (dir) {
				case 0:
					if ((!maze()->open(down)) || (!(m_btList.find(down) == -1)))	break;   
					else move(down);	
					break;
				case 1:  
					if ((!maze()->open(up)) || (!(m_btList.find(up) == -1)))	break;
					else move(up);		
					break;
				case 2:  
					if ((!maze()->open(left)) || (!(m_btList.find(left) == -1)))	break;
					else move(left);	
					break;
				case 3:  
					if ((!maze()->open(right)) || (!(m_btList.find(right) == -1)))	break;
					else move(right);	
					break;
				default: 
					break;
				}



			}

			if (state() == State::BACKTRACK) {

				if (m_btList.empty()) {
					(state(State::LOOK));
				}


				move(m_btList.peek_front());
				if (m_btList.count() == 1) {
					(state(State::LOOK));
					return;
				}
				m_btList.remove_front();

			}

			if (state() == State::NOEXIT) {

			}

			if (state() == State::EXIT) {

			}
		}
	}