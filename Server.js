var express = require("express");
var app = express();
var router = express.Router();

// paths
var database = require(__dirname + '/Database.js');
var path = __dirname + '/views/';

//for parsing of request body
var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: true }));

//for color temperature conversions
var colorTemp = require('color-temp');	

app.use(express.static(path));

// ejs
//app.engine('ejs', require('ejs').renderFile);
app.set('view engine', 'ejs');

/***************************** User Interface Requests *****************************/
app.get('/userview_lobby', function(req, res){ 
	console.log(path + 'userview_lobby');

	res.render(path + 'userview_lobby', {
 		name: "Rooms", 
 		rooms: database.rooms});
});

app.post('/userview_lobby', function(req, res){ 
	console.log(path + 'userview_lobby');

	res.render(path + 'userview_lobby', {
 		name: "Rooms", 
 		rooms: database.rooms});
});




app.get('/userview/:id', function(req, res){ 
	console.log(path + 'userview');

	var reqID = req.params.id;

	// get rid of the ":" from the parameter of the req
	if(reqID.charAt(0) === ':')
	{
		reqID = reqID.slice( 1 );
	}

	var indexOfRoom = parseInt(reqID) - 1;

	// making sure we are getting something that exists 
	if(indexOfRoom < 0 || indexOfRoom >= database.rooms.length)
	{
		res.render(path + 'error', {
			name: "ERROR", 
			errorCode: "Not a valid room!"});
	}
	else
	{
		var title = "Room " + reqID;
		console.log("INDEX: " + indexOfRoom);
		
		res.render(path + '/userview', {
	 		name: title, 
	 		room: database.rooms[indexOfRoom]});
	}
});









/***************************** Admin Interface Requests *****************************/

app.get('/', function(req, res){ 
 	res.render(path + 'index', {
 		name: "Adminstrator Dashboard", 
 		numOfRooms: database.rooms.length});
});

app.get('/room/:id', function(req, res){ 

	var reqID = req.params.id;

	// get rid of the ":" from the parameter of the req
	if(reqID.charAt(0) === ':')
	{
		reqID = reqID.slice( 1 );
	}

	var indexOfRoom = parseInt(reqID) - 1;

	// making sure we are getting something that exists 
	if(indexOfRoom < 0 || indexOfRoom >= database.rooms.length)
	{
		res.render(path + 'error', {
			name: "ERROR", 
			errorCode: "Not a valid room!"});
	}
	else
	{
		var title = "Room " + reqID;
		console.log("INDEX: " + indexOfRoom);
		res.render(path + 'room', {
			name: title, 
			room: database.rooms[indexOfRoom], 
			phases: database.phases, 
			numOfRooms: database.rooms.length});
	}
});

//receives brightness value from webpage
app.post('/new-brightness', function(req, res){
	let i = req.body.room;
	let b = req.body.brightness;

	// convert the brightness 
	let convertedValue = parseInt(b)*0.01;

	if(convertedValue <= 0.0)
	{
		convertedValue = 0.01;
	}

	// make sure it is only 0.00 place values
    var str_convertedValue_cut = convertedValue.toString().match(/^-?\d+(?:\.\d{0,2})?/)[0]
    convertedValue = parseFloat(str_convertedValue_cut);

	//console.log(database.rooms[i-1].brightness);
	database.rooms[i-1].changeBrightness(i, convertedValue);
	//console.log(database.rooms[i-1].brightness);
	
	//brightness has been changed since last ping
	database.rooms[i-1].changeBrightnessChanged(true);

	// render page again
	var title = "Room " + i;
	
	res.render(path + '/userview', {
 		name: title, 
 		room: database.rooms[i-1]});

});

// turn on / off power
app.post('/onoffpower', function(req, res){
	let i = req.body.room;
	let b = req.body.newValue;

	//console.log(database.rooms[i-1].lightOn);
	if(b == "0")
	{
		database.rooms[i-1].changeLightOn(false);
	}
	else
	{
		database.rooms[i-1].changeLightOn(true);
	}
	//console.log(database.rooms[i-1].lightOn);

	// render page again
	var title = "Room " + i;
	
	res.render(path + '/userview', {
 		name: title, 
 		room: database.rooms[i-1]});

});

/***************************** Arduino Requests *****************************/

//returns current hour 0-23 (concatenated, not rounded)
app.get('/current-time', function(req, res){
	console.log(path + 'current-time');
	
	let date = new Date();
	let current_hour = date.getHours();
	res.send(":body:\n" + current_hour.toString() + "\n:/body:");
});

//indicates whether the brightness has been changed since last ping
app.get('/changed-brightness', function(req, res){
	console.log(path + 'changed-brightness');
	
	let i = req.query.r;
	res.send(":body:\n" + database.rooms[i-1].brightnessChanged.toString() + "\n:/body:");
});

//returns current brightness 0-255
app.get('/current-brightness', function(req, res){
	console.log(path + 'current-brightness');
	
	let i = req.query.r;
	let on = database.rooms[i-1].lightOn;
	let b = database.rooms[i-1].brightness;
	
	if (on) {
		res.send(":body:\n" + b.toString() + "\n:/body:");
	} else {
		res.send(":body:\n" + 0 + "\n:/body:");
	}
	
	//brightness has no longer been changed since last ping
	database.rooms[i-1].changeBrightnessChanged(false);
});

//indicates whether the room has been changed since last ping
app.get('/changed-room', function(req, res){
	console.log(path + 'changed-room');
	
	let i = req.query.r;
	res.send(":body:\n" + database.rooms[i-1].roomChanged.toString() + "\n:/body:");
});

//returns current room object
app.get('/current-room', function(req, res){
	console.log(path + 'current-room');
	
	let i = req.query.r;
	var cycle = [];
	for(var x = 0; x < database.rooms[i-1].startValues.length; x++){
		cycle.push({	T:database.rooms[i-1].correctedTempValues[x], 
						L:database.rooms[i-1].lumensValues[x], 
						S:database.rooms[i-1].startValues[x], 
						E:database.rooms[i-1].endValues[x]});
	}
	
	res.send(":body:\n" + JSON.stringify({Cycles: cycle}) + "\n:/body:");
	
	//room has no longer been changed since last ping
	database.rooms[i-1].changeRoomChanged(false);
});

/***************************** Sensor Requests *****************************/

app.post('/sensor-data', function (req, res) {
	console.log(path + 'sensor-data');
	
	let i = req.body.r;
	let r1 = req.body.red;
	let g1 = req.body.green;
	let b1 = req.body.blue;
	let t1 = req.body.temp;
	
	//if temperature was not provided, calculate manually
	if (!t1) {
		t1 = colorTemp.rgb2temp([r1, g1, b1]);
	}
	
	//flags for room status
	let warning = database.rooms[i-1].lightsCompensating;
	let error = database.rooms[i-1].lightsDegraded;
	
	//find currently desired temperature in cycle
	let date = new Date();
	let current_hour = date.getHours();
	let t3;
	let current_index;
	for (x = 0; x < database.rooms[i-1].startValues.length; x++) {
		if (current_hour >= database.rooms[i-1].startValues[x] && current_hour <= database.rooms[i-1].endValues[x]) {
			t3 = database.rooms[i-1].tempValues[x];
			current_index = x;
			break;
		}
	}
	
	//check threshold for compensation
	//if below threshold, do nothing
	if (Math.abs(t3 - t1) < 50) {
		res.render(path + '/sensor-simulator', {
	 		room: null, 
	 		red: null,
			green: null,
			blue: null,
			temp: null,
			newtemp: null		
			});
		return;
	}
	
	//flag warning because the lights are being compensated
	warning = true;
	
	//Calculate rgb difference
	let rgb3 = colorTemp.temp2rgb(t3);
	let r2 = rgb3[0] - r1;
	let g2 = rgb3[1] - g1;
	let b2 = rgb3[2] - b1;
	
	let rgbx;
	
	//for each phase in the cycle
	for (x = 0; x < database.rooms[i-1].startValues.length; x++) {
		//convert desired temperature to desired rgb
		let tx = database.rooms[i-1].correctedTempValues[x];
		rgbx = colorTemp.temp2rgb(tx);
		
		//add difference from current rgb to give corrected rgb
		rgbx[0] += r2;
		rgbx[1] += g2;
		rgbx[2] += b2;
		
		//adjust for bounds
		//flag error because lights are beyond available compensation
		if (rgbx[0] > 255) {
			rgbx[0] = 255;
			error = true;
		} else if (rgbx[0] < 0) {
			rgbx[0] = 0;
			error = true;
		}
		if (rgbx[1] > 255) {
			rgbx[1] = 255;
			error = true;
		} else if (rgbx[1] < 0) {
			rgbx[1] = 0;
			error = true;
		}
		if (rgbx[2] > 255) {
			rgbx[2] = 255;
			error = true;
		} else if (rgbx[2] < 0) {
			rgbx[2] = 0;
			error = true;
		}
		
		//convert corrected rgb to corrected temperature and update variable
		database.rooms[i-1].changeCorrectedTempValueAtIndex(x, colorTemp.rgb2temp(rgbx));
	}
	
	//update flags
	database.rooms[i-1].changeLightsCompensating(warning);
	database.rooms[i-1].changeLightsDegraded(error);
	
	//room has been changed since last ping
	database.rooms[i-1].changeRoomChanged(true);
	
	res.render(path + '/sensor-simulator', {
	 		room: i, 
	 		red: r2,
			green: g2,
			blue: b2,
			temp: t3,
			newtemp: database.rooms[i-1].correctedTempValues[current_index]	
			});
})

/***************************** Test Pages *****************************/

//simulator for testing
app.get('/sensor-simulator', function(req, res){ 
	console.log(path + 'sensor-simulator');
	
 	res.render(path + '/sensor-simulator', {
	 		room: null, 
	 		red: null,
			green: null,
			blue: null,
			temp: null,
			newtemp: null	
			});
});

//simulator for testing
app.get('/master-test', function(req, res){ 
	console.log(path + 'master-test');
	
	let i = req.query.r;
 	res.render(path + '/master-test', {
			database: database,
			rooms: database.rooms,
	 		room: i
			});
});

app.post('/master-test-light', function(req, res){
	let i = req.body.r;
	
	//toggle brightness
	if (database.rooms[i-1].lightOn) {
		database.rooms[i-1].changeLightOn(false);
	} else {
		database.rooms[i-1].changeLightOn(true);
	}
	
	//brightness has been changed since last ping
	database.rooms[i-1].changeBrightnessChanged(true);
	
	res.render(path + '/master-test', {
			database: database,
			rooms: database.rooms,
	 		room: i
			});

});

app.post('/master-test-brightness', function(req, res){
	let i = req.body.r;
	let b = req.body.brightness;
	
	if (b > 1) {
		b = 1;
	} else if (b < 0) {
		b = 0;
	}

	database.rooms[i-1].changeBrightness(i-1, b);
	
	database.rooms[i-1].changeBrightnessChanged(true);
	
	res.render(path + '/master-test', {
			database: database,
			rooms: database.rooms,
	 		room: i
			});

});

app.post('/master-test-room', function(req, res){
	let i = req.body.r;
	let t = req.body.temp;

	database.rooms[i-1].changeCorrectedTempValueAtIndex(i-1, t);
	
	database.rooms[i-1].changeRoomChanged(true);
	
	res.render(path + '/master-test', {
			database: database,
			rooms: database.rooms,
	 		room: i
			});

});

/***************************** Kevin RFID *****************************/
app.post('/astronaut-detected', function(req, res){
	let i = req.body.r;
	let nameTest = req.body.n; //Gets array of names
	
	function dumbo(element) {
		return element == nameTest;
	}
	
	let ind = -1;
	for(j = 0; j < database.rooms.length; j++){
		if(j == (i - 1)) {
			ind = database.rooms[j].names.findIndex(dumbo);
			if (ind > -1) database.rooms[j].names.splice(ind,1);
			else database.rooms[i-1].names.push(nameTest);
		}
		else {
			ind = database.rooms[j].names.findIndex(dumbo);
			if (ind > -1)
				database.rooms[j].names.splice(ind,1);
		}
		
		if (database.powerSaveMode == true){
			if (database.rooms[j].names.length == 0) {
				database.rooms[j].lightOn = false;
				database.rooms[j].brightnessChanged = true;
			}
		}
	}
	
	if (database.rooms[i-1].names.length > 0 && database.rooms[i-1].lightOn == false) {
			database.rooms[i-1].lightOn = true;
			database.rooms[i-1].brightnessChanged = true;
	}
	
	for (j = 0; j < database.rooms.length; j++)
		console.log('People in room ' + (j+1) + ': ', database.rooms[j].names);
	
	res.render(path + '/master-test', {
			database: database,
			rooms: database.rooms,
	 		room: i
			});
});

app.post('/master-test-powersave', function(req, res){
	let i = req.body.r;
	
	//Toggles powerSaveMode
	if (database.powerSaveMode == false)
		database.powerSaveMode = true;
	else if (database.powerSaveMode == true)
		database.powerSaveMode = false;		
	
	//Turns off all rooms with no one in it if true
	if (database.powerSaveMode == true){
		for (j = 0; j < database.rooms.length; j++){
			if (database.rooms[j].names.length == 0) {
				database.rooms[j].lightOn = false;
				database.rooms[j].brightnessChanged = true;
			}
		}
	}
	
	res.render(path + '/master-test', {
			database: database,
			rooms: database.rooms,
	 		room: i
			});
});

/***************************** Server Setup *****************************/

app.listen(3000, function(){
  console.log("Live at Port 3000");
});
