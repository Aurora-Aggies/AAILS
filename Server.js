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
 	res.render(path + 'index', {name: "Adminstrator Dashboard"});
});

app.get('/room1', function(req, res){ 
 	res.render(path + 'room1', {name: "Room 1", room: database.rooms[0], phases: database.phases});
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