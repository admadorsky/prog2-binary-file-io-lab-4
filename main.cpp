#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std ;

struct Weapon {
    int len_name ;
    char* w_name ;
    int w_power_rating ;
    float w_power_consumption ;
};

struct Spaceship {
    int len_name ;
    char* sp_name ;
    int len_class ;
    char* sp_class ;
    short sp_length ;
    int sp_shield_capacity ;
    float sp_warp_speed ;
    int sp_weapons_count ;
    vector<Weapon> weapons ;
};

struct SP_File {
    int spaceship_count ;
};

string convert_to_string(char* array, int count) {
    string output_string = "" ;
    for (int i = 0; i < count - 1; i++) {
        output_string += array[i];
    }
    return output_string ;
}

vector<Spaceship> readFile(string file_name) {
    vector<Spaceship> ships_list ;
    ifstream inSP(file_name, ios::binary);
    if(!inSP) {
        cout << "Cannot open file!" << endl;
    } else {
        SP_File ships_file ;
        inSP.read((char*)&ships_file, sizeof(ships_file)) ;
            // cout << ships_file.spaceship_count << endl ;                         // DEBUG: prints spaceship_count

        for (int i = 0; i < ships_file.spaceship_count; i++) {
            Spaceship current_ship ;

            // reads 4 bytes into len_name ... f**k reinterpret_cast (respectfully)
            inSP.read((char*)& (current_ship.len_name), 4) ;
                // cout << current_ship.len_name << endl ;                         // DEBUG: prints len_name
            // allocates memory for the array of characters that make up the name based on len_name
            current_ship.sp_name = new char[current_ship.len_name] ;
            // reads the name into sp_name as an array of char's
            inSP.read(current_ship.sp_name, current_ship.len_name) ;
                // cout << current_ship.sp_name << endl ;                          // DEBUG: prints sp_name

            // reads 4 bytes into len_class
            inSP.read((char*)& (current_ship.len_class), 4) ;
            current_ship.sp_class = new char[current_ship.len_class] ;
            // reads len_class bytes char's into sp_class
            inSP.read(current_ship.sp_class, current_ship.len_class) ;
                // cout << current_ship.sp_class << endl ;                         // DEBUG: prints sp_class

            // reads 2 bytes into length
            inSP.read((char*)& (current_ship.sp_length), 2) ;
            // cout << current_ship.sp_length << endl ;

            // reads 4 bytes into shield_capacity
            inSP.read((char*)& (current_ship.sp_shield_capacity), 4) ;
            // cout << current_ship.sp_shield_capacity << endl ;                   // DEBUG: prints shield_capacity

            // reads 4 bytes into warp_speed
            inSP.read((char*)& (current_ship.sp_warp_speed), 4) ;
            // cout << current_ship.sp_warp_speed << endl ;                        // DEBUG: prints warp_speed

            // reads 4 bytes into weapons_count
            inSP.read((char*)& (current_ship.sp_weapons_count), 4) ;
            // cout << current_ship.sp_weapons_count << endl ;                     // DEBUG: prints weapons_count

            for (int k = 0; k < current_ship.sp_weapons_count; k++) {
                Weapon current_weapon ;

                //reads 4 bytes into len_name
                inSP.read((char*)& (current_weapon.len_name), 4) ;
                // cout << current_weapon.len_name << endl ;                        // DEBUG: prints len_name

                current_weapon.w_name = new char[current_weapon.len_name] ;
                // reads len_name bytes char's into current_weapon's name
                inSP.read(current_weapon.w_name, current_weapon.len_name) ;
                // cout << current_weapon.w_name << endl ;                          // DEBUG: prints w_name

                // reads 4 bytes into power_rating
                inSP.read((char*)& current_weapon.w_power_rating, 4) ;
                // cout << current_weapon.w_power_rating << endl ;                  // DEBUG: prints power_rating

                // reads 4 bytes into power_consumption
                inSP.read((char*)& current_weapon.w_power_consumption, 4) ;
                // cout << current_weapon.w_power_consumption << endl ;             // DEBUG: prints power_consumption

                // add the weapon and it's data to the list of current_ship's armaments
                current_ship.weapons.push_back(current_weapon) ;
            }
            // add the current_ship to the list of all enemy spaceships
            ships_list.push_back(current_ship) ;
        }
    }
    inSP.close() ;
    return ships_list ;
}

void printShip(Spaceship ship) {
    // print general ship stats
    cout << "Name: " << convert_to_string(ship.sp_name, ship.len_name) << endl ;
    cout << "Class: " << convert_to_string(ship.sp_class, ship.len_class) << endl ;
    cout << "Length: " << ship.sp_length << endl ;
    cout << "Shield capacity: " << ship.sp_shield_capacity << endl ;
    cout << "Maximum Warp: " << ship.sp_warp_speed << endl ;
    // header for the weapons section
    cout << "Armaments: " << endl ;
    if (ship.weapons.size() == 0) {
        cout << "Unarmed" << endl ;
    } else {
        int total_firepower = 0;
        // prints the stats for each weapon, and adds up the total firepower stat
        for (unsigned int i = 0; i < ship.weapons.size(); i++) {
            Weapon curr_w = ship.weapons.at(i);
            cout << convert_to_string(curr_w.w_name, curr_w.len_name) << ", " << curr_w.w_power_rating << ", "
                 << curr_w.w_power_consumption << endl;
            total_firepower += curr_w.w_power_rating;
        }
        // print total firepower
        cout << "Total firepower: " << total_firepower << endl;
    }
    // newline to separate different ships
    cout << endl ;
}

int maxPower(Spaceship ship) {
    // an unarmed ship has a max power of 0
    if (ship.weapons.size() == 0) {
        return 0 ;
    }
    // initialize the max value to the ship's first weapon
    int max_power = ship.weapons.at(0).w_power_rating ;
    for (unsigned int i = 0; i < ship.weapons.size(); i++) {
        // check if any of the other weapons have higher power ratings
        if (ship.weapons.at(i).w_power_rating > max_power) {
            // update the ship's max power rating
            max_power = ship.weapons.at(i).w_power_rating ;
        }
    }
    return max_power ;
}

// returns a given ship's total power rating
int totalPower(Spaceship ship) {
    // if no weapons, the ship has a max power rating of 0
    if (ship.weapons.size() == 0) {
        return 0 ;
    }
    // initialize the total value to 0
    int total_rating = 0 ;
    for (unsigned int i = 0; i < ship.weapons.size(); i++) {
        total_rating += ship.weapons.at(i).w_power_rating ;
    }
    return total_rating ;
}

void printAllShips(vector<Spaceship> ships) {
    // print all ships
    for (unsigned int f = 0; f < ships.size(); f++) {
        printShip(ships.at(f));
    }
}

void printStrongestWeapon(vector<Spaceship> ships) {
    Spaceship max_power_weapon = ships.at(0) ;
    for (unsigned int i = 0; i < ships.size(); i++) {
        // check if the current ship has a higher max power weapon than the current max
        if (maxPower(ships.at(i)) > maxPower(max_power_weapon)) {
            max_power_weapon = ships.at(i) ;
        }
    }
    // print the ship with the most powerful weapon
    printShip(max_power_weapon) ;
}

void printStrongestShip(vector<Spaceship> ships) {
    // intialize the ship with the highest total power to the first ship
    Spaceship max_total_power = ships.at(0) ;
    // iterate through enemies and find ship with the most powerful weapon
    for (unsigned int i = 0; i < ships.size(); i++) {
        // check if the current ship has a higher max power weapon than the current max
        if (totalPower(ships.at(i)) > totalPower(max_total_power)) {
            max_total_power = ships.at(i) ;
        }
    }
    // print the ship with the highest total power level
    printShip(max_total_power) ;
}

void printWeakestShip(vector<Spaceship> ships) {
    // intialize the ship with the lowest total power to the first ship
    Spaceship min_total_power = ships.at(0) ;
    // iterate through ships and find ship with the least powerful weapon
    for (unsigned int i = 0; i < ships.size(); i++) {
        // check if the current ship has a lower max power weapon than the current max
        if (
        (totalPower(ships.at(i)) < totalPower(min_total_power)) &&
        (ships.at(i).weapons.size() > 0)
        ) {
            min_total_power = ships.at(i) ;
        }
    }
    // print the ship with the lowest total power level
    printShip(min_total_power) ;
}

void printUnarmedShips(vector<Spaceship> ships) {
    // print unarmed ships
    for (unsigned int i = 0; i < ships.size(); i++) {
        if (ships.at(i).weapons.size() == 0) {
            printShip(ships.at(i)) ;
        }
    }
}

int main() {
//    cout << "Which file(s) to open?\n";
//    cout << "1. friendlyships.shp" << endl;
//    cout << "2. enemyships.shp" << endl;
//    cout << "3. Both files" << endl;
    int option;
    cin >> option;

    vector<Spaceship> ships ;

    if (option == 1) {
        ships = readFile("friendlyships.shp") ;
    } else if (option == 2) {
        ships = readFile("enemyships.shp") ;
    } else if (option == 3) {
        ships = readFile("friendlyships.shp") ;
        vector<Spaceship> temp ;
        temp = readFile("enemyships.shp") ;
        ships.insert(ships.end(), temp.begin(), temp.end()) ;
    }

//    cout << "1. Print all ships" << endl;
//    cout << "2. Starship with the strongest weapon" << endl;
//    cout << "3. Strongest starship overall" << endl;
//    cout << "4. Weakest ship (ignoring unarmed)" << endl;
//    cout << "5. Unarmed ships" << endl;

    cin >> option;

    if (option == 1) {
        printAllShips(ships) ;
    } else if (option == 2) {
        printStrongestWeapon(ships) ;
    } else if (option == 3) {
        printStrongestShip(ships) ;
    } else if (option == 4) {
        printWeakestShip(ships) ;
    } else if (option == 5) {
        printUnarmedShips(ships) ;
    }

    return 0 ;
}
