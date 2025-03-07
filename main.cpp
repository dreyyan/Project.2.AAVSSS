#include <algorithm>
#include <array>
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
// * Set launch size to 50 x 46
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
    {"RED", VehicleColor::RED}, {"ORANGE", VehicleColor::ORANGE},
    {"YELLOW", VehicleColor::YELLOW}, {"GREEN", VehicleColor::GREEN},
    {"CYAN", VehicleColor::CYAN}, {"BLUE", VehicleColor::BLUE},
    {"MAGENTA", VehicleColor::MAGENTA}, {"VIOLET", VehicleColor::VIOLET},
    {"BEIGE", VehicleColor::BEIGE}, {"GRAY", VehicleColor::GRAY},
    {"SILVER", VehicleColor::SILVER}, {"WHITE", VehicleColor::WHITE},
    {"BLACK", VehicleColor::BLACK}
};

const unordered_map<string_view, VehicleMake> vehicle_make_list = {
    {"TOYOTA", VehicleMake::TOYOTA}, {"HONDA", VehicleMake::HONDA},
    {"NISSAN", VehicleMake::NISSAN}, {"MAZDA", VehicleMake::MAZDA},
    {"SUBARU", VehicleMake::SUBARU}, {"MITSUBISHI", VehicleMake::MITSUBISHI},
    {"SUZUKI", VehicleMake::SUZUKI}, {"FORD", VehicleMake::FORD},
    {"CHEVROLET", VehicleMake::CHEVROLET}, {"DODGE", VehicleMake::DODGE},
    {"JEEP", VehicleMake::JEEP}, {"TESLA", VehicleMake::TESLA},
    {"VOLKSWAGEN", VehicleMake::VOLKSWAGEN}, {"BMW", VehicleMake::BMW},
    {"MERCEDES_BENZ", VehicleMake::MERCEDES_BENZ}, {"AUDI", VehicleMake::AUDI},
    {"PORSCHE", VehicleMake::PORSCHE}, {"MASERATI", VehicleMake::MASERATI},
    {"FERRARI", VehicleMake::FERRARI}, {"LAMBORGHINI", VehicleMake::LAMBORGHINI},
    {"HYUNDAI", VehicleMake::HYUNDAI}, {"KIA", VehicleMake::KIA}
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
    {MASERATI, {{"Ghibli", 1967}, {"Levante", 2016}, {"Quattroporte", 1963}}},
    {FERRARI, {{"488 GTB", 2015}, {"SF90 Stradale", 2019}, {"F8 Tributo", 2019}}},
    {LAMBORGHINI, {{"Huracán", 2014}, {"Aventador", 2011}, {"Urus", 2018}}},
    {HYUNDAI, {{"Elantra", 1990}, {"Santa Fe", 2000}, {"Tucson", 2004}}},
    {KIA, {{"Sportage", 1993}, {"Seltos", 2019}, {"Stinger", 2017}}}
};

struct Utilities {
    void display_format(size_t length) {
        for (size_t i = 0; i < length; ++i) {
            cout << '-';
        } cout << '\n';
    } void space(size_t n) {
        for (size_t i = 0; i < n; ++i) { cout << '\n'; }
    } void delay(int ms) {
        this_thread::sleep_for(chrono::milliseconds(ms));
    } void press_enter_to_continue() {
        cout << "Press 'Enter' to continue...\n";
        cin.get();
    } void display_error_message(const string& error_message) {
        cout << "[ ERROR: " << error_message << " ]\n";
    } void trim(string& string_input) {
        string_input.erase(0, string_input.find_first_not_of(" ")); // Remove leading whitespaces
        string_input.erase(string_input.find_last_not_of(" ") + 1); // Remove trailing whitespaces
    } void to_lowercase(string& string_input) {
        transform(string_input.begin(), string_input.end(), string_input.begin(), ::tolower);
    } void to_uppercase(string& string_input) {
        transform(string_input.begin(), string_input.end(), string_input.begin(), ::toupper);
    } void move_cursor(int up, int down, int right, int left) {
        cout << "\033[" << up << 'A';
        cout << "\033[" << down << 'B';
        cout << "\033[" << right << 'C';
        cout << "\033[" << left << 'D';
    } void clear_screen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
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
    Vehicle(Utilities &utility) : Vehicle(utility, "", VehicleColor::UNKNOWN_COLOR, VehicleMake::UNKNOWN_MAKE, "", "", 0) {}
    Vehicle(Utilities &utility, string id, VehicleColor color, VehicleMake make, string model, string license_plate, unsigned int year)
    : utility(utility),
    id(make_unique<string>(move(id))),
    color(color),
    make(make),
    model(make_unique<string>(move(model))),
    license_plate(make_unique<string>(move(license_plate))),
    year(year) {} // Parameterized Constructor

    /* GETTERS */
    string get_id() const { return *id; }
    VehicleColor get_color() const { return color; }
    VehicleMake get_make() const { return make; }
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

        for (int i = 0; i < 4; i++)  // Generate 256 bits (4 × 64-bit numbers)
            hash << std::hex << std::setw(16) << std::setfill('0') << dist(gen);

        return hash.str();
    }


    /* METHODS */
    void ask_to_create_vehicle(const string& question) {
        string user_input;
        while (true) {
            cout << question << "(yes/no):\n";
            utility.display_format(question.length());
            getline(cin, user_input);

            if (user_input == "") utility.display_error_message("Empty input");
            else {
                utility.trim(user_input); // Remove leading & trailing whitespaces
                utility.to_lowercase(user_input); // Convert to lowercase
                if (user_input == "yes" || user_input == "no") break;
                else utility.display_error_message("Invalid input");
            }
        }
        if (user_input == "yes") create_vehicle();
        else return;
    }

    string generate_vehicle_id() {
        return generate_hash();
    }

    void choose_vehicle_color() {
        string input_color_str;

        while (true) {
            // Display colors list
            cout << "[ COLORS ]\n";
            utility.display_format(10);

            for (const auto& color : vehicle_color_list) {
                cout << color_codes[color.second] << color.first << "\033[0m" << '\n';
            } utility.display_format(10);

            // Prompt user to enter vehicle color
            cout << "Choose color: ";
            getline(cin, input_color_str);

            if (input_color_str == "") utility.display_error_message("Empty input");
            else {
                utility.trim(input_color_str); // Remove leading & trailing whitespaces
                utility.to_uppercase(input_color_str); // Convert to uppercase
                string_view input_color(input_color_str); // Convert to view-only

                // Search if color exists in the unordered map
                auto it = find_if(vehicle_color_list.begin(), vehicle_color_list.end(),
                    [&](const auto& pair) { return pair.first == input_color; });

                if (it == vehicle_color_list.end()) { // If vehicle is not found
                    utility.display_error_message("Color does not exist");
                } else {
                    set_color(it->second); // Set vehicle color
                    cout << "Vehicle color set to '" << it->first << "'\n";
                    break;
                }
            }
        }
    }

    void choose_vehicle_make() {
        string input_make_str;

        while (true) {
            // Display makes list
            for (const auto& make : vehicle_make_list) {
                cout << make.first << '\n';
            }

            // Prompt user to enter vehicle make
            cout << "Choose make: ";
            getline(cin, input_make_str);

            if (input_make_str == "") utility.display_error_message("Empty input");
            else {
                utility.trim(input_make_str); // Remove leading & trailing whitespaces
                utility.to_uppercase(input_make_str); // Convert to uppercase
                string_view input_make(input_make_str); // Convert to view-only

                // Check if make exists
                auto it = vehicle_make_list.find(input_make);

                if (it == vehicle_make_list.end()) { // ERROR: Non-existing make
                    utility.display_error_message("make does not exist");
                } else {
                    make = it->second; // Set vehicle make
                    cout << "Vehicle make set to '" << it->first << "'\n";
                    break;
                }
            }
        }
    }

    void choose_vehicle_model() {
        string input_model;
        bool model_exists;

        while (true) {
            model_exists = false;
            auto make_models = vehicle_models.find(make);
            if (make_models != vehicle_models.end()) { // If a vehicle model is found in a make
                // Display vehicle models
                for (const auto& model : make_models->second) {
                    cout << model.first << '\n';
                }
            } else {
                utility.display_error_message("No models for this make");
                return;
            }

            // Prompt user to enter model
            cout << "Choose model: ";
            getline(cin, input_model);

            if (input_model == "") utility.display_error_message("Empty input");
            else {
                utility.trim(input_model); // Remove leading & trailing whitespaces
                utility.to_uppercase(input_model); // Convert to uppercase

                // Check if model exists
                for (const auto& make_model : make_models->second) {
                    if (input_model == make_model.first) { // If existing model
                        model_exists = true;
                        model = make_unique<string>(make_model.first); // Set model
                        break;
                    }
                }

                if (!model_exists) { // ERROR: Non-existing model
                    utility.display_error_message("Model does not exist");
                }
            }
        }
    }

    void generate_license_plate(string vehicle_id) {
        string sub_id = vehicle_id.substr(0, 4);
        set_license_plate(sub_id);
    }

    /* COMMAND FUNCTIONS */
    void create_vehicle() {
        string vehicle_id;
        // Generate unique & secure(hashed) vehicle id
        vehicle_id = generate_hash();
        // Prompt user to enter vehicle color
        choose_vehicle_color();
        // Prompt user to enter vehicle make
        choose_vehicle_make();
        // Prompt user to enter vehicle model based on make
        choose_vehicle_model();
        // Assign the license plate based on id + make + model + year
        generate_license_plate(vehicle_id);

        Vehicle(utility, vehicle_id, color, make, *model, *license_plate, year); // Create object vehicle
    }

    void goto_garage() {}
    const void view_makes() {}
    const void view_models() {}
    const void display_vehicle_details() {
        // ERROR: Uninitialized vehicle details
        if (!id || !make || !model || !license_plate) {
        utility.display_error_message("Vehicle details are not initialized");
            ask_to_create_vehicle("Would you like to add one?");
        }
        cout << "[ VEHICLE DETAILS ]\n";
        cout << "ID: " << get_id() << '\n';
        cout << "Color: " << get_color() << '\n';
        cout << "Make: " << get_make() << '\n';
        cout << "Model: " << get_model() << '\n';
        cout << "License Plate: " << get_license_plate() << '\n';
        cout << "Year: " << get_year() << '\n';
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

    static constexpr int num_of_commands = 6; // Edit to change # of commands

    // To store command-descriptions
    array<array<string, 2>, num_of_commands> command_to_description = {
        array<string, 2>{"create", "Create vehicle"},
        array<string, 2>{"garage", "Manage garage"},
        array<string, 2>{"makes", "Display available vehicle makes"},
        array<string, 2>{"models", "Display available vehicle models"},
        array<string, 2>{"details", "Display vehicle details"},
        array<string, 2>{"exit", "Exit"}
    };

    // To store command-functions
    unordered_map<string, function<void()>> command_to_function = {
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
            vehicle.utility.display_format(50); vehicle.utility.delay(100); vehicle.utility.space(1); vehicle.utility.delay(100);
            cout << setw(3) << ' ' << "[*]-[*]-[*]-[*] A.A.V.S.S.S. [*]-[*]-[*]-[*]\n"; vehicle.utility.delay(100);
            vehicle.utility.space(1); vehicle.utility.delay(100); vehicle.utility.display_format(50); vehicle.utility.delay(100); vehicle.utility.space(1); vehicle.utility.delay(100);

            // Display commands
            for (int i = 0; i < command_to_description.size(); ++i) {
                cout << setw(2) << ' ' << setw(8) << left << '.' + command_to_description[i][0] << setw(2) << ' ' << "|   ";
                cout << left << setw(34) << command_to_description[i][1] << '\n';
                vehicle.utility.delay(100);
            } vehicle.utility.space(1); vehicle.utility.delay(100); vehicle.utility.display_format(50); vehicle.utility.delay(100); vehicle.utility.space(1); vehicle.utility.delay(100);

            // Prompt user to enter command
            cout << " Enter command: .\n";
            vehicle.utility.space(1); vehicle.utility.delay(100); vehicle.utility.display_format(50); vehicle.utility.delay(100);
            vehicle.utility.move_cursor(4, 0, 18, 0);
            getline(cin, command);

            if (command == "") vehicle.utility.display_error_message("Empty input");
            else {
                vehicle.utility.trim(command); // Remove leading & trailing whitespaces
                vehicle.utility.to_lowercase(command); // Convert to uppercase

                if (command_to_function.find(command) != command_to_function.end()) {
                    vehicle.utility.clear_screen(); // Clear screen before invoking function
                    command_to_function[command](); // Invoke function
                    vehicle.utility.press_enter_to_continue();
                } else {
                    vehicle.utility.display_error_message("Invalid command");
                }
            }
        }
    }
};

// id, color, make, model, license plate, year
int main() {
    Utilities utility;
    Vehicle vehicle(utility);
    VehicleUI vehicle_ui(vehicle);
    vehicle_ui.display_menu();
    return 0;
}
