var express = require("express");
var app = express();
var router = express.Router();

// paths
var database = require(__dirname + '/Database.js');
var path = __dirname + '/views/';

app.use(express.static(path));

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