#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <openssl/sha.h>
using namespace std;

// Project: ADVANCED AUTOMATED VEHICLE STORAGE AND SIMULATION SYSTEM (AAVSSS)
// Author: dreyyan
// Creation Date: 03/07/2025

enum VehicleColor {
    UNKNOWN = -1,                                               // CUSTOM COLORS
    BLACK, SILVER, WHITE, GRAY, BEIGE,                          // MONOCHROMATIC COLORS
    RED, ORANGE, YELLOW, GREEN, BLUE, VIOLET,                   // BASIC COLORS
    VEHICLE_COLOR_COUNT
};

enum VehicleMake {
    TOYOTA, HONDA, NISSAN, MAZDA, SUBARU, MITSUBISHI, SUZUKI,   // JAPANESE MAKES
    FORD, CHEVROLET, DODGE, JEEP, TESLA,                        // U.S. MAKES
    VOLKSWAGEN, BMW, MERCEDES_BENZ, AUDI, PORSCHE,              // GERMAN MAKES
    MASERATI, FERRARI, LAMBORGHINI,                             // ITALIAN MAKES
    HYUNDAI, KIA,                                               // KOREAN MAKES
    VEHICLE_MAKE_COUNT
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

class Vehicle {
protected:
    /* ATTRIBUTES */
    unique_ptr<string> id, model, license_plate;
    unsigned int year;
    VehicleColor color;
    VehicleMake make;
public:
    /* CONSTRUCTOR */
    Vehicle() {} // Default Constructor
    Vehicle(string id, VehicleColor color, VehicleMake make, string model, string license_plate, unsigned int year)
    : id(make_unique<string>(move(id))),
    color(color),
    make(make),
    model(make_unique<string>(move(model))),
    license_plate(make_unique<string>(move(license_plate))),
    year(year) {} // Parameterized Constructor

    /* UTILITIES */
    void display_format(size_t length) {
        for (size_t i = 0; i < length; ++i) {
            cout << '-';
        } cout << '\n';
    } void delay(int ms) {
        this_thread::sleep_for(chrono::milliseconds(ms));
    } void press_enter_to_continue() {
        cout << "Press 'Enter' to continue...\n";
        cin.get();
    } void display_error_message(const string& error_message) {
        cout << "[ ERROR: " << error_message << " ]\n";
    } void trim(string& string_input) {
        string_input.erase(0, string_input.find_first_not_of(" ")); // Remove leading whitespaces
        string_input.erase(find_last_not_of(" ") + 1); // Remove trailing whitespaces
    } void to_lowercase(string& string_input) {
        transform(string_input.begin(), string_input.end(), string_input.begin(), ::tolower);
    } void ask_yes_no(const string& question) {
        string user_input;
        while (true) {
            cout << question << "(yes/no):\n";
            display_format(question.length());
            getline(cin, user_input);

            if (user_input == "") display_error_message("Empty input");
            else {
                trim(user_input); // Remove leading & trailing whitespaces
                to_lowercase(user_input); // Convert to lowercase
                if (user_input == "yes" || user_input == "no") break;
                else display_error_message("Invalid input");
            }
        }

        if (user_input == "yes")
    }

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
    string sha256(const string& input) { // Hash a string using SHA-256
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)input.c_str(), input.size(), hash);

        stringstream ss;
        for (unsigned char i : hash) {
            ss << hex << setw(2) << setfill('0') << (int)i;
        }
        return ss.str();
    }

    string generate_secure_hash() { // Generate secure hash using SHA256
        random_device rd;
        mt19937_64 generator(rd());
        uniform_int_distribution<uint64_t> dist;

        uint64_t random_number = dist(generator); // Generate a secure random number
        return sha256(to_string(random_number));  // Hash the random number
    }

    /* METHODS */
    string generate_vehicle_id() {
        return generate_secure_hash();
    }
    VehicleColor choose_vehicle_color() {
        while (true) {
            for (int i = 0; i < VehicleColor.)
        }
    }
    void choose_vehicle_make() {}
    void choose_vehicle_model() {}
    void generate_license_plate() {}

    void create_vehicle() { cout << "creating vehicle...\n";
        string vehicle_id;
        VehicleColor vehicle_color;
        VehicleMake vehicle_make;

        // Generate unique & secure(hashed) vehicle id
        vehicle_id = generate_vehicle_id();
        // Prompt user to enter vehicle color
        vehicle_color = choose_vehicle_color();
        // Prompt user to enter vehicle make
        // Prompt user to enter vehicle model based on make
        // Assign the license plate based on id + make + model + year

    }

    const void display_vehicle_details() {
        // ERROR: Uninitialized vehicle details
        if (!id || !make || !model || !license_plate) {
        display_error_message("Vehicle details are not initialized");
            ask_yes_no("Would you like to add one?");
        }
        cout << "[ VEHICLE DETAILS ]\n";
        cout << "ID: " << get_id() << '\n';
        cout << "Color: " << get_color() << '\n';
        cout << "Make: " << get_make() << '\n';
        cout << "Model: " << get_model() << '\n';
        cout << "License Plate: " << get_license_plate() << '\n';
        cout << "Year: " << get_year() << '\n';
    }
};
// id, color, make, model, license plate, year
int main() {
    Vehicle vehicle;
    vehicle.display_vehicle_details();
    /*Vehicle(
    "1HGCM82633A123456",
    VehicleColor::BLACK,
    VehicleMake::HONDA,
    "Ci",
    "ABA-5146",
    2003);*/
    return 0;
}
