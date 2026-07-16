#include <iostream>
#include "utilities.h"
#include <vector>
#include <random>
#include <iomanip>

//======================================
//		GLOBALS (Structs, Enums).

struct Point{
	int x,y;
};
	
enum Direction{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

//======================================
//		GLOBALS (Designs).

	std::string obstacles_stored[] = {	"#########"  , "+~+  +  +" , "+~+| |+~+",
										"###[X]###"  , "I-I|O|I-I" , "[|] X [|]",
									  	"\\|/|O|/|\\", "(O)[#](O)" , "\\ / X / \\",
										".##.# .# "  , "+-+      " , "+  |  +  ",
										"      +-+"  , "  +  |  +" , "~~~      ",
										"|  |  |  "  , "___      " };	
			
	const std::string colors[] = {	"\033[34m",		// Blue
									"\033[31m",		// Red
									"\033[32m",		// Green
									"\033[33m",		// Yellow
									"\033[36m",		// Cyan
									"\033[35m"      // Magenta
									};
		
	std::string tank_shapes[] = { "/|\\!O!\\=/",	// UP
								  "/=\\!O!\\|/",	// DOWN
								  "/=\\~~O\\=/",	// LEFT
								  "/=\\O~~\\=/"};	// RIGHT
								  
	char bullet_shapes[] = { '^' , 'v' , '<' , '>' };
	
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//	GRID SIZE
		int grid_X = 56, grid_Y = 201;
	
	Point spawnpoints[] =  { {5,3} , {grid_X,grid_Y-2} , {5,grid_Y-2} , {grid_X,3} };
	
	Direction spawndirections[] = { DOWN , UP , RIGHT , LEFT };

	const std::string hidecursor = "\033[?25l";
	const std::string showcursor = "\033[?25h";

//================================================
//		CLASSES (Obstacle,Bullet,Tank,Player,Map)

class Obstacle{
	private:
		std::string shape;
		Point position;
	public:
		Obstacle(std::string shape, Point position) {
			this->shape = shape;
			this->position = position;
		}
		void setposition(Point position) { this->position = position; }
		Point getposition() { return position; }
		std::string getshape() { return shape; }
};

class Bullet{
	private:
		int damage;
		Point position;
		Direction direction;
	public:
		Bullet(Direction direction) {
			this->direction = direction;
			damage = 1;
		}
		int getdamage() { return damage; }
		void setposition(Point position) { this->position = position; }
		void setdirection(Direction direction) { this->direction = direction; }
		Point getposition() { return position; }
		Direction getdirection() { return direction; }
};

class Tank{
	private:
		int health;
		std::string color;
		Point position;
		Direction direction;
		bool alive;
	public:
		Tank(int health , std::string color) {
			this->health = health;
			this->color = color;
			this->alive = true;
		}
		void sethealth(int health) { this->health = health; }
		void setcolor(std::string color) { this->color = color; }
		void setposition(Point position) { this->position = position; }
		void setdirection(Direction direction) { this->direction = direction; }
		void setstatus(bool alive) { this->alive = alive; }
		int gethealth() { return health; }
		std::string getcolor() { return color; }
		Point getposition() { return position; }
		Direction getdirection() { return direction; }
		bool getstatus() { return alive; }
};

class Player{
	private:
		std::string name;
	public:
		Tank tank;
		Player(std::string name, int health, std::string color) : tank(health, color) {
			this->name = name;
		}
		void setname(std::string name)  { this->name = name; }
		std::string getname() { return name; }
};

class Map{
	private:
		long seed;
		char grid[56][201];
		std::vector<Obstacle> obstacles;
		void setobstacles() {
			std::random_device rd;		// Creating a... I think a Random Number Generator
    		std::mt19937 gen(rd());		// Getting the Random Number Generator to be ready. Mathematics Engine Actually!
    		// For Selecting how many obstacles will be selected to print
    		std::uniform_int_distribution<int> ob_number(250, 300);
			int number_of_obstacles = ob_number(gen);
			
			// For Selecting a Random Obstacle from the Obstacle Storage.
			std::uniform_int_distribution<int> ob_style(0 ,std::size(obstacles_stored)-1);
			
			for(int i = 0; i < number_of_obstacles; i++) {
				int printcontrol = 0, rowcontrol = 1;
				int rand_obstacle_no = ob_style(gen);
				std::string obstacle_toprint = obstacles_stored[rand_obstacle_no];
				
				// For Selecting a Random X Cordinate to Place that Obstacle.
				std::uniform_int_distribution<int> cordx(5,grid_X-7);
				int x = cordx(gen);
				// For Selecting a Random X Cordinate to Place that Obstacle.
				std::uniform_int_distribution<int> cordy(5,grid_Y-7);
				int y = cordy(gen);
				
				int row_control = 0,column_control = 0;
				for(int j = 0; j < 9 ; j++ ) {
					grid[x + (j / 3)][y + (j % 3)] = obstacle_toprint[j];
				}	
			}
		}
	public:
		char getpoint(int x, int y) {
			return grid[x][y];
		}
		void initializegrid() {
			for(int x = 0 ; x < grid_X ; x++ ) {
				for(int y = 0 ; y < grid_Y ; y++) {
					// Logic For the Corneres.
					if(x == 0 && y == 0) {
						grid[x][y] = '+';
					}else if(x == grid_X-1 && y == 0) {
						grid[x][y] = '+';
					}else if(x == 0 && y == grid_Y-1) {
						grid[x][y] = '+';
					}else if(x == grid_X-1 && y == grid_Y-1) {
						grid[x][y] = '+';
					}
					//Logic For Borders.
					if(x == 0 && (y > 0 && y < grid_Y-1))
						grid[x][y] = '=';
					if(x == grid_X-1 && (y > 0 && y < grid_Y-1))
						grid[x][y] = '=';
					if((x > 0 && x < grid_X-1) && y == 0)
						grid[x][y] = '|';
					if((x > 0 && x < grid_X-1) && y == grid_Y-1)
						grid[x][y] = '|';
						
					//Logic For Center
					if((x > 0 && x < grid_X-1) && (y > 0 && y < grid_Y-1))
						grid[x][y] = ' ';
				}
			}
			setobstacles();
		}
		void showgrid() {
			std::cout<<color::b_black;
			for(int x = 0 ; x < grid_X ; x++ ) {
				for(int y = 0 ; y < grid_Y ; y++) {
					std::cout<<grid[x][y];
				}
				std::cout<<"\n";
			}
			std::cout<<color::reset;
		}
};

//======================================
//		CLASS (Game_Manager)

class Game_Manager {
	private:
		Map map;
		std::vector<Player> players;
		std::vector<Bullet> bullets;
		unsigned long long gametick = 0;
		
		void checkcollisionsbullets() {
			Direction cur_dir;
			Point cur_pos, next_pos;
			std::vector<Bullet>::iterator it = bullets.begin();
			while(it != bullets.end()) {
				cur_dir = it->getdirection();
				cur_pos = it->getposition();
				next_pos = cur_pos;
				if (cur_dir == UP)    next_pos.x -= 1;
		        if (cur_dir == DOWN)  next_pos.x += 1;
		        if (cur_dir == LEFT)  next_pos.y -= 1;
		        if (cur_dir == RIGHT) next_pos.y += 1;
		        
		        if(map.getpoint(next_pos.x, next_pos.y) != ' ' ) {
		        	sys::gotoxy(cur_pos.x, cur_pos.y);
           			std::cout << " ";
		        	it = bullets.erase(it);
				} else {
					it++;
				}
			}
		}
		
		void checkmovementsbullets() {
			Point position;
			for(auto &b : bullets) {
				position = b.getposition();
				if(b.getdirection() == UP)
				position.x -= 1; 
				else if(b.getdirection() == DOWN) 	{ position.x += 1; } 
				else if(b.getdirection() == LEFT) 	{ position.y -= 1; }
				else if(b.getdirection() == RIGHT) 	{ position.y += 1; } 
				b.setposition(position);
			}
		}
		void checkcollisionstanks() {
			
		}
		bool cantankmove(Point newcenter) {
			for (int j = 0 ; j < 9 ; j++) {
				int cx = newcenter.x + (j / 3) - 1;
				int cy = newcenter.y + (j % 3) - 1;
				char pointongrid = map.getpoint(cx,cy);
				if(pointongrid != ' ') return false;
			}
			return true;
		}
		void checkmovementstanks(char in) {
			Point position;
			// PLAYER 1 <~~~
			if(in == 'w' || in == 'W') {
				position = players[0].tank.getposition();
				position.x -= 1;
				if(cantankmove(position)) players[0].tank.setposition(position);
			} else if(in == 's' || in == 'S') {
				position = players[0].tank.getposition();
				position.x += 1;
				if(cantankmove(position)) players[0].tank.setposition(position);
			} else if(in == 'a' || in == 'A') {
				position = players[0].tank.getposition();
				position.y -= 1;
				if(cantankmove(position)) players[0].tank.setposition(position);
			} else if(in == 'd' || in == 'D') {
				position = players[0].tank.getposition();
				position.y += 1;
				if(cantankmove(position)) players[0].tank.setposition(position);
			} else if(in == 'q' || in == 'Q') {
			    Direction cur_dir = players[0].tank.getdirection();
			    if(cur_dir == UP) {
			        players[0].tank.setdirection(RIGHT);
			    } else if(cur_dir == RIGHT) {
			        players[0].tank.setdirection(DOWN);
			    } else if(cur_dir == DOWN) {
			        players[0].tank.setdirection(LEFT);
			    } else if(cur_dir == LEFT) {
			        players[0].tank.setdirection(UP);
			    }
			} else if(in == 'e' || in == 'E') {
			    Direction cur_dir = players[0].tank.getdirection();
			    if(cur_dir == UP) {
			        players[0].tank.setdirection(LEFT);
			    } else if(cur_dir == LEFT) {
			        players[0].tank.setdirection(DOWN);
			    } else if(cur_dir == DOWN) {
			        players[0].tank.setdirection(RIGHT);
			    } else if(cur_dir == RIGHT) {
			        players[0].tank.setdirection(UP);
			    }
			} else if(in == 'f' || in == 'F') {
				position = players[0].tank.getposition();
				Direction cur_dir = players[0].tank.getdirection();
				Bullet new_bullet(cur_dir);
				if(cur_dir == UP){
					position.x -= 2;
					new_bullet.setposition(position);
				}else if(cur_dir == DOWN){
					position.x += 2;
					new_bullet.setposition(position);
				}else if(cur_dir == LEFT){
					position.y -= 2;
					new_bullet.setposition(position);
				}else if(cur_dir == RIGHT){
					position.y += 2;
					new_bullet.setposition(position);
				}
				
				if(map.getpoint(position.x, position.y) == ' ')
					bullets.push_back(new_bullet);
			}
			// PLAYER 2 <~~~
			if(in == 'i' || in == 'I') {
				position = players[1].tank.getposition();
				position.x -= 1;
				if(cantankmove(position)) players[1].tank.setposition(position);
			} else if(in == 'k' || in == 'K') {
				position = players[1].tank.getposition();
				position.x += 1;
				if(cantankmove(position)) players[1].tank.setposition(position);
			} else if(in == 'j' || in == 'J') {
				position = players[1].tank.getposition();
				position.y -= 1;
				if(cantankmove(position)) players[1].tank.setposition(position);
			} else if(in == 'l' || in == 'L') {
				position = players[1].tank.getposition();
				position.y += 1;
				if(cantankmove(position)) players[1].tank.setposition(position);
			} else if(in == 'o' || in == 'O') {
			    Direction cur_dir = players[1].tank.getdirection();
			    if(cur_dir == UP) {
			        players[1].tank.setdirection(RIGHT);
			    } else if(cur_dir == RIGHT) {
			        players[1].tank.setdirection(DOWN);
			    } else if(cur_dir == DOWN) {
			        players[1].tank.setdirection(LEFT);
			    } else if(cur_dir == LEFT) {
			        players[1].tank.setdirection(UP);
			    }
			} else if(in == 'u' || in == 'U') {
			    Direction cur_dir = players[1].tank.getdirection();
			    if(cur_dir == UP) {
			        players[1].tank.setdirection(LEFT);
			    } else if(cur_dir == LEFT) {
			        players[1].tank.setdirection(DOWN);
			    } else if(cur_dir == DOWN) {
			        players[1].tank.setdirection(RIGHT);
			    } else if(cur_dir == RIGHT) {
			        players[1].tank.setdirection(UP);
			    }
			} else if(in == 'p' || in == 'P') {
				position = players[1].tank.getposition();
				Direction cur_dir = players[1].tank.getdirection();
				Bullet new_bullet(cur_dir);
				if(cur_dir == UP){
					position.x -= 2;
					new_bullet.setposition(position);
				}else if(cur_dir == DOWN){
					position.x += 2;
					new_bullet.setposition(position);
				}else if(cur_dir == LEFT){
					position.y -= 2;
					new_bullet.setposition(position);
				}else if(cur_dir == RIGHT){
					position.y += 2;
					new_bullet.setposition(position);
				}
				if (map.getpoint(position.x, position.y) == ' ')
					bullets.push_back(new_bullet);
			}
		}
		
	public:
		
		void startround() {
			char input;
			bool health_changed = true;
			sys::clearscreen();
			map.initializegrid();
			std::cout<<"+"; design::line(grid_Y-2,"="); std::cout<<"+\n\n";
			map.showgrid();
			// MAIN GAME LOOP   <~~~~~   Main loop of the Round. That Runs the Game!
			
			for(auto &p : players) {
				p.tank.sethealth(10);
				p.tank.setstatus(true);
			}
			
			while(true) {
				// Health Header Only Redrawn When Health of Any Player Changes.
				if(health_changed) {
					sys::gotoxy(2,0);
					std::cout<<"| "<<std::left<<players[0].tank.getcolor()<<std::setw(14)<<players[0].getname()<<":"<<std::setw(2)<<players[0].tank.gethealth();
					design::line(163," ");
					std::cout<<std::left<<players[1].tank.getcolor()<<std::setw(14)<<players[1].getname()<<":"<<std::setw(2)<<players[1].tank.gethealth()<<color::reset<<" |";
					health_changed = false;
				}
				
				// Removing Old Bullets
				for( auto b : bullets ) {
					Point pos = b.getposition();
					sys::gotoxy(pos.x,pos.y);
					std::cout<<" ";
				}
				
				if(gametick % 2 == 0) { 
					checkcollisionsbullets(); 
					checkmovementsbullets();
				}
				if(gametick % 5 == 0) {
					if(_kbhit()) {
						while(_kbhit()) {
				            input = _getch(); 
				        }
						// Remove the Old Tanks First
						for(auto &p : players) {
						    if (p.tank.getstatus() == false) continue;
						    Point pos = p.tank.getposition(); 
						    std::string shape = tank_shapes[p.tank.getdirection()];
						    for(int j = 0; j < 9; j++) {
						        int screen_x = pos.x + (j / 3) - 1;
						        int screen_y = pos.y + (j % 3) - 1;
						        sys::gotoxy(screen_x, screen_y);
						        std::cout << " ";
						    }
						}
					}
					checkmovementstanks(input);
					input = ' ';
				}
				
				for(auto p : players) {
				    if (p.tank.getstatus() == false) break;
				    Point pos = p.tank.getposition(); 
				    std::string shape = tank_shapes[p.tank.getdirection()];
				    
				    std::cout << p.tank.getcolor(); 
				    for(int j = 0; j < 9; j++) {
				        int screen_x = pos.x + (j / 3) - 1;
				        int screen_y = pos.y + (j % 3) - 1;
				        sys::gotoxy(screen_x, screen_y);
				        std::cout << shape[j];
				    }
				}
				std::cout << color::reset;
				
				for( auto b : bullets ) {
					Point pos = b.getposition();
					sys::gotoxy(pos.x,pos.y);
					std::cout<<bullet_shapes[b.getdirection()];
				}
				
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				gametick++;
			}
		}
		void mainmenu() {
			char choice;
			do{
				sys::clearscreen();
				std::cout<<color::b_white;
				design::draw_header("    MULTI TANK    ","=");
				std::cout<<"\n\t\t\t||                                      ||";
				std::cout<<"\n\t\t\t|| "<<color::yellow<<" [1]. "<<color::b_cyan<<" Start a new Game. "<<color::b_white<<"            ||";
				std::cout<<"\n\t\t\t||                                      ||";
				std::cout<<"\n\t\t\t|| "<<color::yellow<<" [2]. "<<color::b_cyan<<" Set Players. "<<color::b_white<<"                 ||";
				std::cout<<"\n\t\t\t||                                      ||";
				std::cout<<"\n\t\t\t|| "<<color::yellow<<" [3]. "<<color::b_cyan<<" Remove Players. "<<color::b_white<<"              ||";
				std::cout<<"\n\t\t\t||                                      ||";
				std::cout<<"\n\t\t\t|| "<<color::yellow<<" [0]. "<<color::b_cyan<<" Exit the Game. "<<color::b_white<<"               ||";
				std::cout<<"\n\t\t\t||                                      ||";
				std::cout<<"\n\t\t\t++"; design::line(38,"~"); std::cout<<"++";
				std::cout<<std::left<<"\n\t\t\t|| "<<color::b_yellow<<" Current Players : "<<color::cyan<<std::setw(2)<<players.size()<<color::b_white<<"                ||";
				if(players.size() == 0) {
					std::cout<<"\n\t\t\t|| "<<color::b_red<<" NO PLAYERS READY! "<<color::b_white<<"                  ||";
				} else if(players.size() > 0) {
					int sr = 1;
					for(auto p : players) {
						std::cout<<"\n\t\t\t||  "<<color::b_yellow<<" ["<<sr<<"]   "<<p.tank.getcolor()<<std::setw(20)<<p.getname()<<color::b_white<<"         ||";
						sr++;
					}
				}
				std::cout<<"\n\t\t\t++"; design::line(38,"="); std::cout<<"++";
				choice =_getch();
				switch(choice) {
					case '1':
						{
							sys::clearscreen();
							std::cout<<"\n\n\n\t\t\t++"; design::line(38,"="); std::cout<<"++";
							if(players.size() < 2) {
									std::cout<<"\n\t\t\t   "<<color::b_yellow<<" [>] "<<color::b_red<<" Not Enough Players in Game! "<<color::b_white;
									sys::pause();
									break;
							}
							sys::type_write("\n\n\t\t\t [>] Please Zoom Out by 6 Times (ctrl + \'-\')");
							sys::printdots(3);
							startround();
							break;
						}
					case '2':
						{
							sys::clearscreen();
							std::string new_player;
							bool already_exist = false;
							std::cout<<"\n\n\n\t\t\t++"; design::line(38,"="); std::cout<<"++";
							if(players.size() >= 2) {
								std::cout<<"\n\t\t\t   "<<color::b_yellow<<" [>] "<<color::b_red<<" Players Already Full! "<<color::b_white;
								sys::pause();
								break;
							}
							std::cout<<"\n\t\t\t   "<<color::b_yellow<<" [>] "<<color::b_blue<<" Enter New Player Name : "<<color::b_white<<showcursor;
							std::getline(std::cin,new_player);
							for(auto p : players) {
								if (p.getname() == new_player) {
									already_exist = true;
								}
							}
							if(already_exist){
								std::cout<<"\n\t\t\t   "<<color::b_yellow<<" [>] "<<color::b_red<<" Player Already Exists! "<<color::b_white<<hidecursor;
								sys::pause();
							} else {
								Player new_p(new_player,10,colors[players.size()]);
								new_p.tank.setdirection(spawndirections[players.size()]);
								new_p.tank.setposition(spawnpoints[players.size()]);   
								players.push_back(new_p);
								std::cout<<"\n\t\t\t   "<<color::b_yellow<<" [>] "<<color::b_green<<" Player Added Successfully! "<<color::b_white<<hidecursor;
								sys::pause();
							}
							break;
						}
					case '3':
						{
							sys::clearscreen();
							std::string del_player;
							bool doesnt_exist = true;
							std::cout<<"\n\n\n\t\t\t++"; design::line(38,"="); std::cout<<"++";
							if(players.size() <= 0) {
								std::cout<<"\n\t\t\t   "<<color::b_yellow<<" [>] "<<color::b_red<<" No Players in Game! "<<color::b_white;
								sys::pause();
								break;
							}
							std::cout<<"\n\t\t\t   "<<color::b_yellow<<" [>] "<<color::b_blue<<" Enter Player Name to Remove: "<<color::b_white<<showcursor;
							std::getline(std::cin,del_player);
							int index = 0;
							for(auto p : players) {
								if (p.getname() == del_player) {
									players.erase(players.begin() + index);
									doesnt_exist = false;
								}
								index++;
							}
							if(doesnt_exist){
								std::cout<<"\n\t\t\t   "<<color::b_yellow<<" [>] "<<color::b_red<<" Player doesn't Exist! "<<color::b_white<<hidecursor;
								sys::pause();
							} else {
								std::cout<<"\n\t\t\t   "<<color::b_yellow<<" [>] "<<color::b_green<<" Player Removed Successfully! "<<color::b_white<<hidecursor;
								sys::pause();
							}
							break;
						}
					case '0':
						sys::clearscreen();
						std::cout<<"\n\n\n\t\t\t++"; design::line(38,"="); std::cout<<"++";
						std::cout<<"\n\t\t\t   "<<color::b_yellow<<" [>] "<<color::b_blue<<" Exiting "<<color::b_white;
						sys::printdots(3);
						break;
				}
			}while(choice != '0');
		}
};

int main()
{
	std::cout << hidecursor;
	Game_Manager GM;
	
	GM.mainmenu();
 	std::cout<<"Hello Basit"<<std::endl;
 	return 0;
}
