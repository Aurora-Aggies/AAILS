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
	let convertedValue = (parseInt(b)/100) * 255;

	if(convertedValue == 0)
	{
		convertedValue = 1;
	}

	console.log(database.rooms[i-1].brightness);
	database.rooms[i-1].changeBrightness(i, convertedValue);
	console.log(database.rooms[i-1].brightness);
	
	//brightness has been changed since last ping
	database.rooms[i-1].changeBrightnessChanged(true);

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
//0 is brightest
//255 is second-brightest
//1 is dimmest
app.get('/current-brightness', function(req, res){
	console.log(path + 'current-brightness');
	
	let i = req.query.r;
	let b = database.rooms[i-1].brightness;
	
	//modulo to keep in bounds and to make 256 go to 0
	res.send(":body:\n" + (b % 256).toString() + "\n:/body:");
	
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


//simulator for testing
app.get('/brightness-simulator', function(req, res){ 
	console.log(path + 'brightness-simulator');
	
 	res.render(path + 'brightness-simulator');
});


//receives brightness value from webpage
app.post('/new-brightness-simulator', function(req, res){
	let i = req.body.room;
	let b = req.body.brightness;
	database.rooms[i-1].changeBrightness(b);
	
	//brightness has been changed since last ping
	database.rooms[i-1].changeBrightnessChanged(true);
	
	res.send("Success");
});

/***************************** Sensor Requests *****************************/

app.post('/sensor-data', function (req, res) {
	console.log(path + 'sensor-data');
	
	let i = req.body.r;
	let r1 = req.body.red;
	let g1 = req.body.green;
	let b1 = req.body.blue;
	let t1 = req.body.temp;
	let l1 = req.body.lux;
	
	//if temperature was not provided, calculate manually
	if (!t1) {
		t1 = colorTemp.rgb2temp([r1, g1, b1]);
	}
	
	//flags for room status
	let error = false;
	let warning = false;
	
	//find currently desired temperature in cycle
	let date = new Date();
	let current_hour = date.getHours();
	let t3;
	for (x = 0; x < database.rooms[i-1].startValues.length; x++) {
		console.log(database.rooms[i-1].tempValues[x]);
		if (current_hour <= database.rooms[i-1].startValues[x]) {
			t3 = database.rooms[i-1].tempValues[x];
			break;
		}
	}
	
	//check threshold for compensation
	//if below threshold, do nothing
	if (Math.abs(t3 - t1) < 50) {
		//return;
		res.render(path + '/sensor-simulator', {
	 		room: null, 
	 		red: null,
			green: null,
			blue: null,
			temp: null,
			newtemp: null,
			lux: null			
			});
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
		rgbx = colorTemp.temp2rgb(database.rooms[i-1].tempValues[x]);
		rx = rgbx[0];
		gx = rgbx[1];
		bx = rgbx[2];
		
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
	
	//room has been changed since last ping
	database.rooms[i-1].changeRoomChanged(true);
	
	res.render(path + '/sensor-simulator', {
	 		room: i, 
	 		red: r2,
			green: g2,
			blue: b2,
			temp: database.rooms[i-1].tempValues[2],
			newtemp: colorTemp.rgb2temp([r2 + rgb3[0],g2 + rgb3[1],b2 + rgb3[2]]),
			lux: l1			
			});
})

//simulator for testing
app.get('/sensor-simulator', function(req, res){ 
	console.log(path + 'sensor-simulator');
	
 	res.render(path + '/sensor-simulator', {
	 		room: null, 
	 		red: null,
			green: null,
			blue: null,
			temp: null,
			newtemp: null,
			lux: null			
			});
});

/***************************** Server Setup *****************************/

app.listen(3000, function(){
  console.log("Live at Port 3000");
});
