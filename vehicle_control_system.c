/*
 ============================================================================
 Name        : vehicle_control_system.c
 Author      : Ahmed Mahmoud Hafny
 Version     : 1.0
 Description : Vehicle Control system with the specifications in the following link
               https://drive.google.com/file/d/120JmLMbLLQxeQtQQO4lXdfTtKbNixM_L/view?usp=sharing
 ============================================================================
 */

#include <stdio.h>

/********************************      Preprocessor directives (MACRO's)         *****************************************/
#define WITH_ENGINE_TEMP_CONTROLLER 1 // Macro to enable/disable engine temperature controller
#define GREEN_LIGHT_V   100 // Speed when traffic light is green
#define RED_LIGHT_V     0   // Speed when traffic light is red
#define ORANGE_LIGHT_V  30  // Speed when traffic light is orange

/********************************     User Defined DATATYPES      *****************************************/

/********* bool defined *********/
typedef enum {
    false, true
} bool;

/********* DataTypes defined *********/
typedef unsigned char             uint8;
typedef signed long               sint32;

/********* Vehicle structure to hold system state *********/
typedef struct {
    bool engine_status;                 // Engine status (ON/OFF)
    bool ac_status;                     // Air conditioner status (ON/OFF)
    bool engine_temp_controller_status; // Engine temperature controller status (ON/OFF)
    sint32 vehicle_speed;               // Vehicle speed in km/hr
    sint32 room_temperature;            // Room temperature in Celsius
    sint32 engine_temperature;          // Engine temperature in Celsius
} Vehicle;

/*******************      Functions Declaration (Prototype) Section     ********************/
void display_menu();
void display_system_state(Vehicle *vehicle);
void handle_engine_sensor_menu(Vehicle *vehicle);
void handle_traffic_light(Vehicle *vehicle);
void handle_room_temperature(Vehicle *vehicle);
void handle_engine_temperature(Vehicle *vehicle);

/***********************************************************      Main Section     ********************************************************************/
int main() {

    setbuf(stdout, NULL); // Disable output buffering

    uint8 input;
    Vehicle vehicle = { false, false, false, 0, 0, 0 }; // Initialize vehicle state

    // Main loop to display menu and handle user input
    do {
        display_menu();           // Display the main menu
        scanf(" %c", &input);     // Take the user choice

        switch (input) {
            case 'a': // Turn on the vehicle engine
                if (!vehicle.engine_status) {
                    vehicle.engine_status = true; // Set engine status to ON
                    while (vehicle.engine_status) {
                        handle_engine_sensor_menu(&vehicle); // Enter engine sensor menu loop
                    }
                } else {
                    printf("\n##->Engine is already ON.\n"); // Notify if engine is already ON
                }
                break;

            case 'b': // Turn off the vehicle engine
                if (vehicle.engine_status) {
                    vehicle.engine_status = false; // Set engine status to OFF
                    printf("Engine turned OFF.\n");
                } else {
                    printf("\n##->Engine is already OFF.\n"); // Notify if engine is already OFF
                }
                break;

            case 'c': // Quit the system
                printf("\n##->System Is OFF\n");
                return 0;

            default:
                printf("\n####Invalid choice. Please enter a, b, or c.\n"); // Prompt for valid input
        }

        display_system_state(&vehicle); // Display system state after each iteration

    } while ('c' != input); // Continue loop until 'c' is entered

    return 0;
}


/********************************      Functions Section     *****************************************/

/********* function to display menu *********/
void display_menu() {
	// Display the main menu options
	printf("\n \t ****The Menu****\n");
	printf("\na. Turn on the vehicle engine\n");
	printf("b. Turn off the vehicle engine\n");
	printf("c. Quit the system\n");
	printf("\nEnter your choice: ");
}

/********* function to display System State *********/
void display_system_state(Vehicle *vehicle) {
	// Display the current system state
	printf("\n\t **System State**\n");
	printf("Engine state: %s\n", vehicle->engine_status ? "ON" : "OFF");    // Display engine status
	printf("AC: %s\n", vehicle->ac_status ? "ON" : "OFF");                  // Display AC status
	printf("Vehicle Speed: %ld km/hr\n", vehicle->vehicle_speed);           // Display vehicle speed
	printf("Room Temperature: %ld°C\n", vehicle->room_temperature);         // Display room temperature
	printf("Engine Temperature Controller State: %s\n",
			vehicle->engine_temp_controller_status ? "ON" : "OFF");        // Display engine temperature controller status
	printf("Engine Temperature: %ld°C\n", vehicle->engine_temperature);    // Display engine temperature
}

/********* function to handle engine sensor *********/
void handle_engine_sensor_menu(Vehicle *vehicle) {
	uint8 input;

	// Display engine sensor options
	printf("\n\n\t **Sensors set menu**\n");
	printf("a. Turn off the engine\n");
	printf("b. Set the traffic light color\n");
	printf("c. Set the room temperature\n");
	printf("d. Set the engine temperature\n\n");
	printf("Enter your choice: ");
	scanf(" %c", &input);

	// Handle user choice for engine sensor menu
	switch (input) {
	case 'a': // Turn off the engine
		vehicle->engine_status = false;
		printf("Engine turned OFF.\n");
		break;
	case 'b': // Set the traffic light color
		handle_traffic_light(vehicle);
		break;
	case 'c': // Set the room temperature
		handle_room_temperature(vehicle);
		break;
	case 'd': // Set the engine temperature
		handle_engine_temperature(vehicle);
		break;
	default:
		printf("Invalid choice. Please enter a, b, c, or d.\n"); // Prompt for valid input
	}

	// If vehicle speed is 30 km/hr, adjust AC and engine temperature controller
	if (30 == vehicle->vehicle_speed) {
		// Turn ON AC and adjust room temperature
		if (!vehicle->ac_status) {
			vehicle->ac_status = true;
			vehicle->room_temperature = (vehicle->room_temperature * (5 / 4)) + 1;
		}

		// Turn ON engine temperature controller and adjust engine temperature
		if (!vehicle->engine_temp_controller_status) {
			vehicle->engine_temp_controller_status = true;
			vehicle->engine_temperature = (vehicle->engine_temperature * (5 / 4)) + 1;
		}
	}

	// Display updated system state after handling engine sensor menu
	display_system_state(vehicle);
}

/********* function to handle traffic lights  *********/
void handle_traffic_light(Vehicle *vehicle) {
	uint8 traffic_light;

	// Prompt user to enter traffic light color
	printf("\n\n\t **traffic light** \n");
	printf("Enter traffic light color (G/O/R): ");
	scanf(" %c", &traffic_light);

	// Set vehicle speed based on traffic light color
	switch (traffic_light) {
	case 'G':
		vehicle->vehicle_speed = GREEN_LIGHT_V;
		break;
	case 'O':
		vehicle->vehicle_speed = ORANGE_LIGHT_V;
		break;
	case 'R':
		vehicle->vehicle_speed = RED_LIGHT_V;
		break;
	default:
		printf("Invalid traffic light color.\n"); // Prompt for valid input
	}
}

/********* function to handle room temperature *********/
void handle_room_temperature(Vehicle *vehicle) {
	sint32 temp;

	// Prompt user to enter room temperature
	printf("Enter room temperature: ");
	scanf("%ld", &temp);

	// Set room temperature and adjust AC status accordingly
	vehicle->room_temperature = temp;
	if (temp < 10 || temp > 30) {
		vehicle->ac_status = true; // Turn ON AC
		vehicle->room_temperature = 20; // Set room temperature to default value
	} else {
		vehicle->ac_status = false; // Turn OFF AC
	}
}

/********* function to handle engine temperature *********/
void handle_engine_temperature(Vehicle *vehicle) {
	sint32 temp;

	// Prompt user to enter engine temperature
	printf("Enter engine temperature: ");
	scanf("%ld", &temp);

	// Set engine temperature and adjust engine temperature controller status accordingly
	vehicle->engine_temperature = temp;
#if WITH_ENGINE_TEMP_CONTROLLER
	if (temp < 100 || temp > 150) {
		vehicle->engine_temp_controller_status = true; // Turn ON engine temperature controller
		vehicle->engine_temperature = 125; // Set engine temperature to default value
	} else {
		vehicle->engine_temp_controller_status = false; // Turn OFF engine temperature controller
	}
#endif
}
