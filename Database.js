/***************************** Prerequisites *****************************/
var Room = require("./Definitions.js");

/***************************** Global Variables *****************************/
var rooms = [];
var phases = ["Sunset", "Dawn", "Overcast", "Daylight", "Mid-Afternoon", "Dusk", "Night"];

/***************************** Creating Rooms *****************************/

	// Room 1
	var Temperatures1 = [2700, 3500, 5000, 6000, 4200, 3500, 2200];
	var Lumens1 = [1500, 1700, 2000, 2200, 1900, 1700, 20];
	var Start1 = [0, 3, 6, 9, 12, 15, 18];
	var End1 = [3, 6, 9, 12, 15, 18, 0];
	var Brightness1 = 256;

	let room1 = new Room(1, Temperatures1, Lumens1, Start1, End1, Brightness1, Temperatures1);
	rooms.push(room1);

	// Room 2
	var Temperatures2 = [2700, 2700, 2700, 2700, 2700, 2700, 2700];
	var Lumens2 = [1700, 1700, 1700, 1700, 1700, 1700, 1700];
	var Start2 = [0, 3, 6, 9, 12, 15, 18];
	var End2 = [3, 6, 9, 12, 15, 18, 0];
	var Brightness2 = 140;

	let room2 = new Room(2, Temperatures2, Lumens2, Start2, End2, Brightness2, Temperatures2);
	rooms.push(room2);

/***************************** Exports *****************************/
exports.rooms = rooms;
exports.phases = phases;