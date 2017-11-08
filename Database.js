/***************************** Prerequisites *****************************/
var Room = require("./Definitions.js");

/***************************** Global Variables *****************************/
var rooms = [];
var phases = ["Night", "Dawn", "Sunrise", "Overcast", "Daylight", "Mid-Afternoon", "Dusk", "Night"];

/***************************** Creating Rooms *****************************/

	// Room 1
	var Temperatures1 = [2200, 3500, 4500, 5000, 6000, 4200, 3300, 2200];
	var Lumens1 = [41, 135, 184, 204, 237, 171, 124, 41];
	var Start1 = [0, 6, 8, 9, 12, 15, 18, 20];
	var End1 = [6, 8, 9, 12, 15, 18, 20, 24];
	var Brightness1 = 1.0;

	let room1 = new Room(1, Temperatures1, Lumens1, Start1, End1, Brightness1, Temperatures1);
	rooms.push(room1);

	// Room 2
	var Temperatures2 = [2200, 3300, 4500, 5000, 4200, 3300, 2100, 2100];
	var Lumens2 = [80, 125, 200, 235, 210, 130, 40, 40];
	var Start2 = [0, 3, 6, 9, 12, 15, 18, 20];
	var End2 = [3, 6, 9, 12, 15, 18, 20, 24];
	var Brightness2 = 0.5;

	let room2 = new Room(2, Temperatures2, Lumens2, Start2, End2, Brightness2, Temperatures2);
	rooms.push(room2);

/***************************** Exports *****************************/
exports.rooms = rooms;
exports.phases = phases;