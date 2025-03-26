#include <algorithm>
#include <array>
#include <cctype>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>
using namespace std;

// Project: ADVANCED AUTOMATED VEHICLE STORAGE AND SIMULATION SYSTEM (AAVSSS)
// Author: dreyyan
// Creation Date: 03/07/2025

// INSTRUCTIONS:
// * Set terminal launch size to 50 x 46
enum VehicleColor {
    UNKNOWN_COLOR = -1,
    RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, VIOLET,    // WHEEL COLORS
    BEIGE, GRAY, SILVER, WHITE, BLACK,                          // MONOCHROMATIC COLORS
    VEHICLE_COLOR_COUNT                                         // OTHER COLORS
};

map<VehicleColor, string> color_codes = {
    {RED, "\033[31m"},
    {ORANGE, "\033[91m"},
    {YELLOW, "\033[33m"},
    {GREEN, "\033[32m"},
    {CYAN, "\033[36m"},
    {BLUE, "\033[34m"},
    {MAGENTA, "\033[35m"},
    {VIOLET, "\033[95m"},
    {BEIGE, "\033[93m"},
    {GRAY, "\033[90m"},
    {SILVER, "\033[37m"},
    {WHITE, "\033[97m"},
    {BLACK, "\033[30;47m"},
    {UNKNOWN_COLOR, "\033[0m"} // Reset
};


enum VehicleMake {
    UNKNOWN_MAKE = -1,
    TOYOTA, HONDA, NISSAN, MAZDA, SUBARU, MITSUBISHI, SUZUKI,   // JAPANESE MAKES
    FORD, CHEVROLET, DODGE, JEEP, TESLA,                        // U.S. MAKES
    VOLKSWAGEN, BMW, MERCEDES_BENZ, AUDI, PORSCHE,              // GERMAN MAKES
    MASERATI, FERRARI, LAMBORGHINI,                             // ITALIAN MAKES
    HYUNDAI, KIA,                                               // KOREAN MAKES
    VEHICLE_MAKE_COUNT
};

const vector<pair<string_view, VehicleColor>> vehicle_color_list = {
    {"Red", VehicleColor::RED}, {"Orange", VehicleColor::ORANGE},
    {"Yellow", VehicleColor::YELLOW}, {"Green", VehicleColor::GREEN},
    {"Cyan", VehicleColor::CYAN}, {"Blue", VehicleColor::BLUE},
    {"Magenta", VehicleColor::MAGENTA}, {"Violet", VehicleColor::VIOLET},
    {"Beige", VehicleColor::BEIGE}, {"Gray", VehicleColor::GRAY},
    {"Silver", VehicleColor::SILVER}, {"White", VehicleColor::WHITE},
    {"Black", VehicleColor::BLACK}
};

const unordered_map<string_view, VehicleMake> vehicle_make_list = {
    {"Toyota", VehicleMake::TOYOTA}, {"Honda", VehicleMake::HONDA},
    {"Nissan", VehicleMake::NISSAN}, {"Mazda", VehicleMake::MAZDA},
    {"Subaru", VehicleMake::SUBARU}, {"Mitsubishi", VehicleMake::MITSUBISHI},
    {"Suzuki", VehicleMake::SUZUKI}, {"Ford", VehicleMake::FORD},
    {"Chevrolet", VehicleMake::CHEVROLET}, {"Dodge", VehicleMake::DODGE},
    {"Jeep", VehicleMake::JEEP}, {"Tesla", VehicleMake::TESLA},
    {"Volkswagen", VehicleMake::VOLKSWAGEN}, {"Bmw", VehicleMake::BMW},
    {"Mercedes-Benz", VehicleMake::MERCEDES_BENZ}, {"Audi", VehicleMake::AUDI},
    {"Porsche", VehicleMake::PORSCHE}, {"Ferrari", VehicleMake::FERRARI},
    {"Lamborghini", VehicleMake::LAMBORGHINI}, {"Hyundai", VehicleMake::HYUNDAI},
    {"Kia", VehicleMake::KIA}
};

unordered_map<VehicleMake, unordered_map<string, unsigned int>> vehicle_models = {
    {TOYOTA, {{"Corolla", 1966}, {"Camry", 1982}, {"Land Cruiser", 1951}}},
    {HONDA, {{"Civic", 1972}, {"Accord", 1976}, {"CR-V", 1995}}},
    {NISSAN, {{"Altima", 1992}, {"GT-R", 1969}, {"Rogue", 2007}}},
    {MAZDA, {{"Mazda3", 2003}, {"CX-5", 2012}, {"MX-5 Miata", 1989}}},
    {SUBARU, {{"Impreza", 1992}, {"Outback", 1994}, {"WRX", 1992}}},
    {MITSUBISHI, {{"Lancer", 1973}, {"Outlander", 2001}, {"Pajero", 1982}}},
    {SUZUKI, {{"Swift", 1983}, {"Vitara", 1988}, {"Jimny", 1970}}},
    {FORD, {{"Mustang", 1964}, {"F-150", 1975}, {"Explorer", 1990}}},
    {CHEVROLET, {{"Camaro", 1966}, {"Silverado", 1999}, {"Malibu", 1964}}},
    {DODGE, {{"Charger", 1966}, {"Challenger", 1970}, {"Durango", 1997}}},
    {JEEP, {{"Wrangler", 1986}, {"Grand Cherokee", 1992}, {"Renegade", 2014}}},
    {TESLA, {{"Model S", 2012}, {"Model 3", 2017}, {"Model X", 2015}}},
    {VOLKSWAGEN, {{"Golf", 1974}, {"Passat", 1973}, {"Tiguan", 2007}}},
    {BMW, {{"3 Series", 1975}, {"X5", 1999}, {"M4", 2014}}},
    {MERCEDES_BENZ, {{"C-Class", 1993}, {"E-Class", 1953}, {"GLE", 2015}}},
    {AUDI, {{"A4", 1994}, {"Q5", 2008}, {"R8", 2006}}},
    {PORSCHE, {{"911", 1964}, {"Cayenne", 2002}, {"Panamera", 2009}}},
    {FERRARI, {{"488 GTB", 2015}, {"SF90 Stradale", 2019}, {"F8 Tributo", 2019}}},
    {LAMBORGHINI, {{"Huracán", 2014}, {"Aventador", 2011}, {"Urus", 2018}}},
    {HYUNDAI, {{"Elantra", 1990}, {"Santa Fe", 2000}, {"Tucson", 2004}}},
    {KIA, {{"Sportage", 1993}, {"Seltos", 2019}, {"Stinger", 2017}}}
};

struct Utilities {
    void display_format(size_t length) {
        for (size_t i = 0; i < length; ++i) {
            cout << '-';
        } delay(100); cout << '\n';
    } void display_alternate_format(size_t length, char symbol) {
        for (size_t i = 0; i < length; ++i) {
            cout << (i % 2 == 0 ? '-' : symbol);
        } delay(100); cout << '\n';
    } void space(size_t n) {
        for (size_t i = 0; i < n; ++i) { cout << '\n'; }
    } void delay(int ms) {
        this_thread::sleep_for(chrono::milliseconds(ms));
    } void character_delay_animation(string string_input, int ms) {
        for (int i = 0; i < string_input.length(); ++i) {
            cout << string_input[i];
            this_thread::sleep_for(chrono::milliseconds(ms));
        }
    } void press_enter(string process) {
        character_delay_animation("Press 'Enter' to " + process + "...", 25);
        cin.get();
        clear_screen();
    } void display_error_message(const string& error_message) {
        cout << "[ ERROR: " << error_message << " ]\n";
        delay(100);
    } void trim(string& string_input) {
        string_input.erase(0, string_input.find_first_not_of(" ")); // Remove leading whitespaces
        string_input.erase(string_input.find_last_not_of(" ") + 1); // Remove trailing whitespaces
    } void to_lowercase(string& string_input) {
        transform(string_input.begin(), string_input.end(), string_input.begin(), ::tolower);
    } void to_uppercase(string& string_input) {
        transform(string_input.begin(), string_input.end(), string_input.begin(), ::toupper);
    } void capitalize(string& string_input) {
        string_input[0] = toupper(string_input[0]);
        for (int i = 1; i < string_input.length(); ++i) {
            string_input[i] = tolower(string_input[i]);
        }
    } void move_cursor(int up, int down, int right, int left) {
        cout << "\033[" << up << 'A';
        cout << "\033[" << down << 'B';
        cout << "\033[" << right << 'C';
        cout << "\033[" << left << 'D';
    } void go_to(int row, int column) {
        cout << "\033[" << row << ';' << column << 'H';
    } void clear_screen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    } void spinning_animation(const string& to_generate, int x) {
        char spinner_characters[4] = {'|', '/', '-', '\\'};
        cout << "...generating " << to_generate << " [ ]";
        go_to(1, x);
        for (int i = 0; i < 40; ++i) {
            cout << spinner_characters[i % 4];
            delay(50);
            go_to(1, x);
        } go_to(1, x + 2); space(1);
    }
    void loading_bar_animation(const string& to_generate, int x) {
        int percentage = 0;
        cout << " [          ] " << percentage << '\n';
        for (int i = 0; i < 10; ++i) {
            go_to(1, x);
            cout << '#';
        }
    }
};

struct VehicleSpecifications {
    string vehicle_name;
    /* FORM SPECIFICATIONS */
    int num_of_doors;
    int num_of_wheels;
    int maximum_passenger_capacity;
    /* TECHNICAL SPECIFICATIONS */
    int rpm;
    int max_speed;
    double max_gas_tank_capacity;
    bool air_conditioned;
    /* CONSTRUCTORS */
    // Default Constructor
    VehicleSpecifications() : vehicle_name(""), num_of_doors(0), num_of_wheels(0), maximum_passenger_capacity(0), rpm(0), max_speed(0), max_gas_tank_capacity(0), air_conditioned(0) {}
    // Parameterized Constructor
    VehicleSpecifications(string vehicle_name, int num_of_doors, int num_of_wheels, int maximum_passenger_capacity, int rpm, int max_speed, double max_gas_tank_capacity, bool air_conditioned)
        : vehicle_name(vehicle_name), num_of_doors(num_of_doors), num_of_wheels(num_of_wheels), maximum_passenger_capacity(maximum_passenger_capacity), rpm(rpm), max_speed(max_speed), max_gas_tank_capacity(max_gas_tank_capacity), air_conditioned(air_conditioned) {}

    /* METHODS */
    static void run_vehicle_simulation() {}
    static void test_vehicle_functionality() {}
    static void tweak_vehicle_parts() {}
    static void display_vehicle_specifications() {}
    static void return_to_main_menu() {

    }
};

class Vehicle {
private:
protected:
    /* ATTRIBUTES */
    unique_ptr<string> id, model, license_plate;
    unsigned int year;
    VehicleColor color;
    VehicleMake make;
public:
    /* REFERENCE */
    Utilities &utility;

    /* CONSTRUCTOR */
    // Default Constructor
    Vehicle(Utilities &utility) : Vehicle(utility, "", VehicleColor::UNKNOWN_COLOR, VehicleMake::UNKNOWN_MAKE, "", "", 0) {}
    // Parameterized Constructor
    Vehicle(Utilities &utility, string id, VehicleColor color, VehicleMake make, string model, string license_plate, unsigned int year)
    : utility(utility),
    id(make_unique<string>(move(id))),
    color(color),
    make(make),
    model(make_unique<string>(move(model))),
    license_plate(make_unique<string>(move(license_plate))),
    year(year) {}

    /* GETTERS */
    string get_id() const { return *id; }
    VehicleColor get_color() const { return static_cast<VehicleColor>(color); }
    VehicleMake get_make() const { return static_cast<VehicleMake>(make); }
    string get_model() const { return *model; }
    string get_license_plate() const { return *license_plate; }
    unsigned int get_year() const { return year; }

    /* SETTERS */
    void set_color(VehicleColor new_color) {
        color = new_color;
    } void set_license_plate(const string& new_license_plate) {
        *license_plate = new_license_plate;
    }

    /* HASHING */
    hash<string> hasher;
    string generate_hash() {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);
        ostringstream hash;

        for (int i = 0; i < 7; i++)
            hash << hex << setfill('0') << setw(16) << dist(gen);
        return hash.str();
    }


    /* METHODS */
    void ask_to_create_vehicle(const string& question) {
        string user_input;
        while (true) {
            cout << question << "(yes/no):\n";
            utility.display_format(question.length());
            getline(cin, user_input);

            if (user_input == "") {
                utility.display_error_message("Empty input");
                utility.press_enter("restart");
            }
            else {
                utility.trim(user_input); // Remove leading & trailing whitespaces
                utility.to_lowercase(user_input); // Convert to lowercase
                if (user_input == "yes" || user_input == "no") break;
                else {
                    utility.display_error_message("Invalid input");
                    utility.press_enter("restart");
                }
            }
        }
        if (user_input == "yes") create_vehicle();
        else return;
    }

    void generate_vehicle_id() {
        utility.spinning_animation("vehicle ID", 27);
        utility.loading_bar_animation("vehicle ID", 1);
        string hash = generate_hash();
        utility.to_uppercase(hash);
        id = make_unique<string>(hash.substr(0, 10));
        utility.clear_screen();
        utility.character_delay_animation(">>> vehicle ID generated!\n", 25);
        string id = "[ ID: " + get_id() + " ]\n";
        utility.character_delay_animation(id, 25);
        utility.press_enter("continue");
    }

    void choose_vehicle_color() {
        string input_color_str;

        while (true) {
            // Display colors list
            cout << setw(3) << ' ' << "[ COLORS ]\n";
            utility.display_alternate_format(16, '*');

            int counter = 0;
            for (const auto& color : vehicle_color_list) {
                if (color.first == "Black") {
                    cout << color_codes[color.second] << left << color.first << "\033[0m";
                } else if (counter++ % 2 == 0) {
                    cout << color_codes[color.second] << setw(9) << left << color.first << "\033[0m";
                } else {
                    cout << color_codes[color.second] << setw(9) << left << color.first << "\033[0m" << '\n';
                } utility.delay(100);
                } utility.space(1); utility.display_alternate_format(16, '*');

            // Prompt user to enter vehicle color
            utility.character_delay_animation("Choose: ", 50);
            getline(cin, input_color_str);

            if (input_color_str == "") {
                utility.display_error_message("Empty input");
                utility.press_enter("restart");
            }
            else {
                utility.trim(input_color_str); // Remove leading & trailing whitespaces
                utility.capitalize(input_color_str); // Convert to uppercase
                string_view input_color(input_color_str); // Convert to view-only

                // Search if color exists in the unordered map
                auto it = find_if(vehicle_color_list.begin(), vehicle_color_list.end(),
                    [&](const auto& pair) { return pair.first == input_color; });

                if (it == vehicle_color_list.end()) { // If vehicle is not found
                    utility.display_error_message("Color does not exist");
                    utility.press_enter("restart");
                } else {
                    set_color(it->second); // Set vehicle color
                    cout << "Vehicle color set to '" << it->first << "'\n";
                    utility.delay(100);
                    break;
                }
            }
        }
        utility.press_enter("continue");
    }

    void choose_vehicle_make() {
        string input_make_str;

        while (true) {
            // Display makes list
            cout << setw(8) << ' ' << "[ MAKES ]\n"; utility.delay(100);
            utility.display_alternate_format(26, '*');

            int counter = 1;
            for (const auto& make : vehicle_make_list) {
                if (counter++ % 2 == 0) {
                    cout << left << setw(14) << make.first << '\n';
                } else {
                    cout << left << setw(14) << make.first;
                } utility.delay(100);
            } utility.space(1); utility.display_alternate_format(26, '*');

            // Prompt user to enter vehicle make
            cout << "Choose: ";
            getline(cin, input_make_str);

            if (input_make_str == "") {
                utility.display_error_message("Empty input");
                utility.press_enter("restart");
            }
            else {
                utility.trim(input_make_str); // Remove leading & trailing whitespaces
                utility.capitalize(input_make_str); // Convert to uppercase
                string_view input_make(input_make_str); // Convert to view-only

                // Check if make exists
                auto it = vehicle_make_list.find(input_make);

                if (it == vehicle_make_list.end()) { // ERROR: Non-existing make
                    utility.display_error_message("make does not exist");
                    utility.press_enter("restart");
                } else {
                    make = it->second; // Set vehicle make
                    cout << "Vehicle make set to '" << it->first << "'\n";
                    utility.delay(100);
                    break;
                }
            }
        }
        utility.press_enter("continue");
    }

    void choose_vehicle_model_and_year() {
        string input_model;
        bool model_exists;

        while (true) {
            model_exists = false;
            auto make_models = vehicle_models.find(make);
            if (make_models != vehicle_models.end()) { // If a vehicle model is found in a make
                // Display model list
                cout << setw(2) << ' ' << "[ MODEL ]\n"; utility.delay(100);
                utility.display_alternate_format(14, '*');
                for (const auto& model : make_models->second) {
                    cout << " > " << model.first << '\n'; utility.delay(100);
                } utility.display_alternate_format(14, '*');
            } else {
                utility.display_error_message("No models for this make"); utility.delay(100);
                utility.press_enter("continue");
                return;
            }

            // Prompt user to enter model
            cout << "Choose: "; utility.delay(100);
            getline(cin, input_model);

            if (input_model == "") {
                utility.display_error_message("Empty input");
                utility.press_enter("restart");
            }
            else {
                utility.trim(input_model); // Remove leading & trailing whitespaces
                utility.capitalize(input_model); // Convert to uppercase

                // Check if model exists
                for (const auto& make_model : make_models->second) {
                    if (input_model == make_model.first) { // If existing model
                        model_exists = true;
                        model = make_unique<string>(make_model.first); // Set model
                        year = make_model.second; // Set year
                        cout << "Vehicle model set to '" << *model << "'\n";
                        utility.delay(100);
                        break;
                    }
                }

                if (!model_exists) { // ERROR: Non-existing model
                    utility.display_error_message("Model does not exist");
                    utility.press_enter("restart");
                } else break;
            }
        }
        utility.press_enter("continue");
    }

    void generate_vehicle_license_plate() {
        utility.spinning_animation("license plate", 30);
        string id_as_string = *id;
        string sub_id1 = id_as_string.substr(0, 4), sub_id2 = id_as_string.substr(4, 4);
        string sub_id = sub_id1 + '-' + sub_id2;
        set_license_plate(sub_id);
        utility.character_delay_animation(">>> license plate generated!\n", 25);
        string license_plate = "[ License Plate #: " + get_license_plate() + " ]\n";
        utility.character_delay_animation(license_plate, 25);
        utility.press_enter("continue");
    }

    /* COMMAND FUNCTIONS */
    void create_vehicle() {
        // Generate unique & secure(hashed) vehicle id
        generate_vehicle_id(); utility.clear_screen();
        // Prompt user to enter vehicle color
        choose_vehicle_color(); utility.clear_screen();
        // Prompt user to enter vehicle make
        choose_vehicle_make(); utility.clear_screen();
        // Prompt user to enter vehicle model based on make and set the model's year
        choose_vehicle_model_and_year(); utility.clear_screen();
        // Assign the license plate based on id + make + model + year
        generate_vehicle_license_plate(); utility.clear_screen();

        Vehicle new_vehicle = Vehicle(utility, *id, color, make, *model, *license_plate, year); // Create object vehicle
        //garage.push_back(new_vehicle);
        utility.character_delay_animation(">>> vehicle created successfully!\n", 25);
        display_vehicle_details();
    }

    void goto_garage() {
        while (true) {
            utility.character_delay_animation("[ GARAGE ]\n", 25);
            utility.display_format(30);
            utility.display_format(30);
            cin.get();
        }

    }
    const void view_makes() {}
    const void view_models() {}
    const void display_vehicle_details() {
        // ERROR: Uninitialized vehicle details
        if (!id || !make || !model || !license_plate) {
            utility.display_error_message("Vehicle details are not initialized");
            ask_to_create_vehicle("Would you like to add one?");
        }
        utility.character_delay_animation("[ VEHICLE DETAILS ]\n", 25);
        utility.display_format(24);
        cout << "ID: " << get_id() << '\n'; utility.delay(200);
        cout << "Color: " << get_color() << '\n'; utility.delay(200);
        cout << "Make: " << get_make() << '\n'; utility.delay(200);
        cout << "Model: " << get_model() << '\n'; utility.delay(200);
        cout << "License Plate: " << get_license_plate() << '\n'; utility.delay(200);
        cout << "Year: " << get_year() << '\n'; utility.delay(200);
        utility.display_format(24);
    }

    void exit_menu() {
        cout << "exiting AAVSSS...\n";
        exit(0);
    }
};

class VehicleUI {
    Vehicle &vehicle;
public:
    /* CONSTRUCTOR */
    VehicleUI(Vehicle &vehicle)
    : vehicle(vehicle) {} // Parameterized Constructor

    static constexpr int num_of_menu_commands = 6; // Edit to change # of commands

    // To store command-descriptions
    array<array<string, 2>, num_of_menu_commands> menu_command_to_description = {
        array<string, 2>{"create", "Create vehicle"},
        array<string, 2>{"garage", "Manage garage"},
        array<string, 2>{"makes", "Display available vehicle makes"},
        array<string, 2>{"models", "Display available vehicle models"},
        array<string, 2>{"details", "Display vehicle details"},
        array<string, 2>{"exit", "Exit"}
    };

    // To store command-functions
    unordered_map<string, function<void()>> menu_command_to_function = {
        {"create", [&]() { vehicle.create_vehicle(); }},
        {"garage", [&]() { vehicle.goto_garage(); }},
        {"makes", [&]() { vehicle.view_makes(); }},
        {"models", [&]() { vehicle.view_models(); }},
        {"details", [&]() { vehicle.display_vehicle_details(); }},
        {"exit", [&]() { vehicle.exit_menu(); }}
    };

    /* METHODS */
    void display_menu() {
        string command;

        while (true) {
            // Display header
            vehicle.utility.display_format(50); vehicle.utility.delay(100);
            cout << setw(3) << ' ' << "[*]-[*]-[*]-[*] A.A.V.S.S.S. [*]-[*]-[*]-[*]\n"; vehicle.utility.delay(100);
            vehicle.utility.space(1); vehicle.utility.delay(100); vehicle.utility.display_format(50); vehicle.utility.delay(100); vehicle.utility.space(1); vehicle.utility.delay(100); vehicle.utility.space(1); vehicle.utility.delay(100);

            // Display commands
            for (int i = 0; i < menu_command_to_description.size(); ++i) {
                cout << setw(2) << ' ' << setw(8) << left << '.' + menu_command_to_description[i][0] << setw(2) << ' ' << "|   ";
                cout << left << setw(34) << menu_command_to_description[i][1] << '\n';
                vehicle.utility.delay(100);
            } vehicle.utility.space(1); vehicle.utility.delay(100); vehicle.utility.display_format(50); vehicle.utility.delay(100);

            vehicle.utility.space(2); vehicle.utility.delay(100); vehicle.utility.display_format(50); vehicle.utility.delay(100);
            // Prompt user to enter command
            vehicle.utility.move_cursor(5, 0, 12, 0);
            vehicle.utility.character_delay_animation(" Enter command: .", 25);
            getline(cin, command);

            if (command == "") {
                vehicle.utility.display_error_message("Empty input");
                vehicle.utility.press_enter("restart");
            }
            else {
                vehicle.utility.trim(command); // Remove leading & trailing whitespaces
                vehicle.utility.to_lowercase(command); // Convert to uppercase

                if (menu_command_to_function.find(command) != menu_command_to_function.end()) {
                    vehicle.utility.clear_screen(); // Clear screen before invoking function
                    menu_command_to_function[command](); // Invoke function
                    vehicle.utility.press_enter("continue");
                } else {
                    vehicle.utility.display_error_message("Invalid command");
                    vehicle.utility.press_enter("restart");
                }
            }
        }
    }
};

static constexpr int num_of_garage_commands = 6; // Edit to change # of commands

// To store command-descriptions
array<array<string, 2>, num_of_garage_commands> garage_command_to_description = {
    array<string, 2>{"run", "Run a vehicle simulation"},
    array<string, 2>{"test", "Test functionality of vehicle"},
    array<string, 2>{"tweak", "Upgrade or modify vehicle parts"},
    array<string, 2>{"specs", "Display vehicle specifications"},
    array<string, 2>{"return", "Return to main menu"}
};

// To store command-functions
unordered_map<string, function<void()>> garage_command_to_function = {
    {"run", []() { VehicleSpecifications::run_vehicle_simulation(); }},
    {"test", []() { VehicleSpecifications::test_vehicle_functionality(); }},
    {"tweak", []() { VehicleSpecifications::tweak_vehicle_parts(); }},
    {"specs", []() { VehicleSpecifications::display_vehicle_specifications(); }},
    {"return", []() { VehicleSpecifications::return_to_main_menu(); }}
};

// id, color, make, model, license plate, year
int main() {
    Utilities utility;
    Vehicle vehicle(utility);
    VehicleUI vehicle_ui(vehicle);
    vehicle_ui.display_menu();
    //vehicle.create_vehicle();
    return 0;
}