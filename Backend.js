var rooms = [];

/* Creating Rooms */

	// Room 1
	var Temperatures1 = [2700, 3500, 5000, 6000, 4200, 3500, 2200];
	var Lumens1 = [1500, 1700, 2000, 2200, 1900, 1700, 20];
	var Percentages1 = [0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.4];


	let room1 = new Room(1, Temperatures1, Lumens1, Percentages1);
	rooms.push(room1);

	// Room 2
	var Temperatures1 = [2700, 2700, 2700, 2700, 2700, 2700, 2700];
	var Lumens1 = [1700, 1700, 1700, 1700, 1700, 1700, 1700];
	var Percentages1 = [0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.4];


	let room1 = new Room(1, Temperatures1, Lumens1, Percentages1);
	rooms.push(room2);

/* ------------- */

/*var EMAIL = "ALYSSA";

function SendText()
{
    nocache = "&nocache=" + Math.random() * 1000000;
    var request = new XMLHttpRequest();
    
    strText = "&txt=" + EMAIL + "&end=end";
    
    request.open("GET", "ajax_inputs" + strText + nocache, true);
    request.send(null);
}*/
