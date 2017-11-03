var express = require("express");
var app = express();
var router = express.Router();

// paths
var database = require(__dirname + '/Database.js');
var path = __dirname + '/views/';

//for parsing of request body
var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: true }));	

app.use(express.static(path));

// ejs
//app.engine('ejs', require('ejs').renderFile);
app.set('view engine', 'ejs');

/***************************** User Interface Requests *****************************/
app.get('/userview', function(req, res){ 
	console.log(path + 'userview');

	res.render(path + 'userview', {
 		name: "User View", 
 		room: database.rooms[0]});
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

//receives 
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
//1 is dimmest
//255 is second-brightest
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
	let room = JSON.stringify({	T:database.rooms[i-1].tempValues, 
								L:database.rooms[i-1].lumensValues, 
								S:database.rooms[i-1].startValues, 
								E:database.rooms[i-1].endValues});
	res.send(room);
	
	//room has no longer been changed since last ping
	database.rooms[i-1].changeRoomChanged(false);
});

/***************************** Sensor Requests *****************************/

app.post('/', function (req, res) {
	
	//convert lumens to rgb
	//find current cycle
	//for each phase in cycle
		//convert desired lumens to desired rgb
		//add difference from current rgb to give corrected rgb
		//convert corrected rgb to corrected lumens
		//update corrected lumens variable

	//update change variable
	
})


// body parser
/*var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: true }));	//for parsing of request body

router.get("/",function(req,res){
  res.sendFile(path + "dashboard.html");
});

router.get("/room1",function(req,res){
 res.sendFile(path + "room1.html");

});


app.use("/",router);*/

app.listen(3000,function(){
  console.log("Live at Port 3000");
});