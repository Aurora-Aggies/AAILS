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

/***************************** Interface Requests *****************************/

// ejs
//app.engine('ejs', require('ejs').renderFile);
app.set('view engine', 'ejs');

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
	database.rooms[i-1].changeBrightness(b.parseInt());
	
	//brightness has been changed since last ping
	database.rooms[i-1].changeBrightnessChanged(true);
	
	res.send("Success");
});

/***************************** Arduino Requests *****************************/

//returns current hour 0-23 (concatenated, not rounded)
app.get('/current-time', function(req, res){
	let date = new Date();
	let current_hour = date.getHours();
	res.send(current_hour.toString());
});

//indicates whether the brightness has been changed since last ping
app.get('/changed-brightness', function(req, res){
	let i = req.query.r;
	res.send(database.rooms[i-1].brightnessChanged.toString());
});

//returns current brightness 0-255
//0 is brightest
//255 is second-brightest
//1 is dimmest
app.get('/current-brightness', function(req, res){
	let i = req.query.r;
	let b = database.rooms[i-1].brightness;
	
	//modulo to keep in bounds and to make 256 go to 0
	res.send((b % 256).toString());
	
	//brightness has no longer been changed since last ping
	database.rooms[i-1].changeBrightnessChanged(false);
});

//indicates whether the room has been changed since last ping
app.get('/changed-room', function(req, res){
	let i = req.query.r;
	res.send(database.rooms[i-1].roomChanged.toString());
});

//returns current room object
app.get('/current-room', function(req, res){
	let i = req.query.r;
	var cycle = [];
	for(var x = 0; x < database.rooms[i-1].startValues.length; x++){
		cycle.push({	T:database.rooms[i-1].correctedTempValues[x], 
						L:database.rooms[i-1].lumensValues[x], 
						S:database.rooms[i-1].startValues[x], 
						E:database.rooms[i-1].endValues[x]});
	}
	
	res.send(JSON.stringify({Cycles: cycle}));
	
	//room has no longer been changed since last ping
	database.rooms[i-1].changeRoomChanged(false);
});

/***************************** Sensor Requests *****************************/

app.post('/sensor-data', function (req, res) {
	let i = req.body.r;
	let r1 = req.body.red;
	let g1 = req.body.green;
	let b1 = req.body.blue;
	
	//TODO: add brightness and threshold
	
	//find currently desired temperature in cycle
	let date = new Date();
	let current_hour = date.getHours();
	let t3;
	for (x = 0; x < database.rooms[i-1].startValues.length; x++) {
		if (current_hour >= database.rooms[i-1].startValues[x]) {
			t3 = database.rooms[i-1].tempValues[x];
			break;
		}
	}
	
	console.log(t3);
	
	//Calculate rgb difference
	let rgb3 = colorTemp.temp2rgb(t3);
	let r2 = rgb3[0] - r1;
	let g2 = rgb3[1] - g1;
	let b2 = rgb3[2] - b1;
	
	//console.log("");
	//console.log(rgb3);
	//console.log([r1, g1, b1]);
	//console.log([r2, g2, b2]);
	
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
		rgbx[2] += bx + b2;
		
		//adjust for bounds 
		if (rgbx[0] > 255) {
			rgbx[0] = 255;
		} else if (rgbx[0] < 0) {
			rgbx[0] = 0;
		}
		if (rgbx[1] > 255) {
			rgbx[1] = 255;
		} else if (rgbx[1] < 0) {
			rgbx[1] = 0;
		}
		if (rgbx[2] > 255) {
			rgbx[2] = 255;
		} else if (rgbx[2] < 0) {
			rgbx[2] = 0;
		}
		
		//convert corrected rgb to corrected temperature and update variable
		//console.log(colorTemp.rgb2temp(rgbx));
		database.rooms[i-1].changeCorrectedTempValueAtIndex(x, colorTemp.rgb2temp(rgbx));
	}
	
	//room has been changed since last ping
	database.rooms[i-1].changeRoomChanged(true);
	
	res.render(path + 'sensor-simulator');
})

//simulator for testing
app.get('/sensor-simulator', function(req, res){ 
 	res.render(path + 'sensor-simulator');
});

/*
router.get("/",function(req,res){
  res.sendFile(path + "dashboard.html");
});

router.get("/room1",function(req,res){
 res.sendFile(path + "room1.html");

});

app.use("/",router);
*/

app.listen(3000,function(){
  console.log("Live at Port 3000");
});